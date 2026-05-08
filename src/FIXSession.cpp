#include "FIXSession.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace {
std::string utcTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

    std::tm utc{};
    gmtime_r(&nowTime, &utc);

    std::ostringstream oss;
    oss << std::put_time(&utc, "%Y%m%d-%H:%M:%S");
    return oss.str();
}

int parseSeqNum(const FIXMessage& message) {
    try {
        return std::stoi(message.getValue("34"));
    } catch (const std::exception&) {
        return 0;
    }
}
} // namespace

FIXSession::FIXSession(SessionConfig config) : m_config(std::move(config)) {
    if (m_config.senderCompID.empty() || m_config.targetCompID.empty()) {
        throw std::invalid_argument(
            "SessionConfig senderCompID and targetCompID are required");
    }
}

FIXMessage FIXSession::createLogon() {
    FIXMessage message = makeAdminMessage("A");
    message.addField("98", "0");
    message.addField("108", std::to_string(m_config.heartBtInt));
    finalizeOutgoing(message);
    m_state = SessionState::LogonSent;
    return message;
}

FIXMessage FIXSession::createHeartbeat(const std::string& testReqID) {
    FIXMessage message = makeAdminMessage("0");
    if (!testReqID.empty()) {
        message.addField("112", testReqID);
    }
    finalizeOutgoing(message);
    return message;
}

FIXMessage FIXSession::createTestRequest(const std::string& testReqID) {
    FIXMessage message = makeAdminMessage("1");
    message.addField("112", testReqID);
    finalizeOutgoing(message);
    return message;
}

FIXMessage FIXSession::createLogout(const std::string& text) {
    FIXMessage message = makeAdminMessage("5");
    if (!text.empty()) {
        message.addField("58", text);
    }
    finalizeOutgoing(message);
    m_state = SessionState::LogoutSent;
    return message;
}

FIXMessage FIXSession::createResendRequest(int beginSeqNo, int endSeqNo) {
    FIXMessage message = makeAdminMessage("2");
    message.addField("7", std::to_string(beginSeqNo));
    message.addField("16", std::to_string(endSeqNo));
    finalizeOutgoing(message);
    return message;
}

std::vector<FIXMessage> FIXSession::onIncoming(const FIXMessage& message) {
    std::vector<FIXMessage> responses;

    if (!message.validate() || !m_dictionary.validate(message) ||
        !isExpectedCounterparty(message)) {
        return responses;
    }

    const int incomingSeqNum = parseSeqNum(message);
    if (incomingSeqNum <= 0) {
        return responses;
    }

    if (incomingSeqNum > m_nextExpectedIncomingSeqNum) {
        responses.push_back(
            createResendRequest(m_nextExpectedIncomingSeqNum, incomingSeqNum - 1));
        return responses;
    }

    if (incomingSeqNum < m_nextExpectedIncomingSeqNum) {
        return responses;
    }

    ++m_nextExpectedIncomingSeqNum;

    const std::string msgType = message.getValue("35");
    if (msgType == "A") {
        m_state = SessionState::Active;
    } else if (msgType == "1") {
        responses.push_back(createHeartbeat(message.getValue("112")));
    } else if (msgType == "5") {
        if (m_state != SessionState::LogoutSent) {
            responses.push_back(createLogout());
        }
        m_state = SessionState::Disconnected;
    }

    return responses;
}

SessionState FIXSession::state() const {
    return m_state;
}

int FIXSession::nextOutgoingSeqNum() const {
    return m_nextOutgoingSeqNum;
}

int FIXSession::nextExpectedIncomingSeqNum() const {
    return m_nextExpectedIncomingSeqNum;
}

FIXMessage FIXSession::makeAdminMessage(const std::string& msgType) {
    FIXMessage message;
    message.addField("8", m_config.beginString);
    message.addField("35", msgType);
    message.addField("49", m_config.senderCompID);
    message.addField("56", m_config.targetCompID);
    message.addField("34", std::to_string(m_nextOutgoingSeqNum));
    message.addField("52", utcTimestamp());
    return message;
}

void FIXSession::finalizeOutgoing(FIXMessage& message) {
    const std::string serialized = message.serialize();
    message = FIXMessage(serialized);
    ++m_nextOutgoingSeqNum;
}

bool FIXSession::isExpectedCounterparty(const FIXMessage& message) const {
    return message.getValue("49") == m_config.targetCompID &&
           message.getValue("56") == m_config.senderCompID;
}
