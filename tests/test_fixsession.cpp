#include "FIXSession.h"
#include <catch2/catch_test_macros.hpp>

namespace {
SessionConfig testConfig() {
    SessionConfig config;
    config.senderCompID = "BUY";
    config.targetCompID = "SELL";
    config.heartBtInt = 30;
    return config;
}

FIXMessage incomingAdmin(const std::string& msgType, int seqNum) {
    FIXMessage message;
    message.addField("8", "FIX.4.2");
    message.addField("35", msgType);
    message.addField("49", "SELL");
    message.addField("56", "BUY");
    message.addField("34", std::to_string(seqNum));
    message.addField("52", "20260508-12:00:00");
    return FIXMessage(message.serialize());
}

FIXMessage incomingTestRequest(int seqNum, const std::string& testReqID) {
    FIXMessage message = incomingAdmin("1", seqNum);
    message.addField("112", testReqID);
    return FIXMessage(message.serialize());
}
} // namespace

TEST_CASE("FIXSession creates valid Logon and increments outgoing sequence",
          "[session][outbound][logon]") {
    FIXSession session(testConfig());

    FIXMessage logon = session.createLogon();

    CHECK(logon.validate());
    CHECK(logon.getValue("8") == "FIX.4.2");
    CHECK(logon.getValue("35") == "A");
    CHECK(logon.getValue("49") == "BUY");
    CHECK(logon.getValue("56") == "SELL");
    CHECK(logon.getValue("34") == "1");
    CHECK(logon.getValue("98") == "0");
    CHECK(logon.getValue("108") == "30");
    CHECK(session.state() == SessionState::LogonSent);
    CHECK(session.nextOutgoingSeqNum() == 2);
}

TEST_CASE("FIXSession outbound admin messages increment MsgSeqNum",
          "[session][outbound][seqnum]") {
    FIXSession session(testConfig());

    CHECK(session.createHeartbeat().getValue("34") == "1");
    CHECK(session.createTestRequest("REQ1").getValue("34") == "2");
    CHECK(session.createLogout("bye").getValue("34") == "3");
    CHECK(session.nextOutgoingSeqNum() == 4);
}

TEST_CASE("FIXSession accepts expected Logon and becomes active",
          "[session][incoming][logon]") {
    FIXSession session(testConfig());
    FIXMessage logon = incomingAdmin("A", 1);
    logon.addField("98", "0");
    logon.addField("108", "30");
    logon = FIXMessage(logon.serialize());

    std::vector<FIXMessage> responses = session.onIncoming(logon);

    CHECK(responses.empty());
    CHECK(session.state() == SessionState::Active);
    CHECK(session.nextExpectedIncomingSeqNum() == 2);
}

TEST_CASE("FIXSession responds to TestRequest with matching Heartbeat",
          "[session][incoming][test-request]") {
    FIXSession session(testConfig());
    FIXMessage testRequest = incomingTestRequest(1, "REQ123");

    std::vector<FIXMessage> responses = session.onIncoming(testRequest);

    REQUIRE(responses.size() == 1);
    CHECK(responses[0].validate());
    CHECK(responses[0].getValue("35") == "0");
    CHECK(responses[0].getValue("112") == "REQ123");
    CHECK(responses[0].getValue("34") == "1");
    CHECK(session.nextExpectedIncomingSeqNum() == 2);
    CHECK(session.nextOutgoingSeqNum() == 2);
}

TEST_CASE("FIXSession requests resend when incoming sequence has a gap",
          "[session][incoming][resend]") {
    FIXSession session(testConfig());
    FIXMessage heartbeat = incomingAdmin("0", 3);

    std::vector<FIXMessage> responses = session.onIncoming(heartbeat);

    REQUIRE(responses.size() == 1);
    CHECK(responses[0].validate());
    CHECK(responses[0].getValue("35") == "2");
    CHECK(responses[0].getValue("7") == "1");
    CHECK(responses[0].getValue("16") == "2");
    CHECK(session.nextExpectedIncomingSeqNum() == 1);
    CHECK(session.nextOutgoingSeqNum() == 2);
}
