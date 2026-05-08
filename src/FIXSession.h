#pragma once

#include "FIXDictionary.h"
#include "FIXMessage.h"
#include <string>
#include <vector>

enum class SessionState {
    Disconnected,
    LogonSent,
    Active,
    LogoutSent,
};

struct SessionConfig {
    std::string beginString = "FIX.4.2";
    std::string senderCompID;
    std::string targetCompID;
    int heartBtInt = 30;
};

class FIXSession {
public:
    explicit FIXSession(SessionConfig config);

    FIXMessage createLogon();
    FIXMessage createHeartbeat(const std::string& testReqID = "");
    FIXMessage createTestRequest(const std::string& testReqID);
    FIXMessage createLogout(const std::string& text = "");
    FIXMessage createResendRequest(int beginSeqNo, int endSeqNo);

    std::vector<FIXMessage> onIncoming(const FIXMessage& message);

    SessionState state() const;
    int nextOutgoingSeqNum() const;
    int nextExpectedIncomingSeqNum() const;

private:
    FIXMessage makeAdminMessage(const std::string& msgType);
    void finalizeOutgoing(FIXMessage& message);
    bool isExpectedCounterparty(const FIXMessage& message) const;

    SessionConfig m_config;
    SessionState m_state = SessionState::Disconnected;
    int m_nextOutgoingSeqNum = 1;
    int m_nextExpectedIncomingSeqNum = 1;
    FIXDictionary m_dictionary;
};
