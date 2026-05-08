# FIXEngine

FIXEngine is a learning project for building a small FIX engine in modern C++.

The goal is not to compete with QuickFIX or produce production trading
infrastructure. The goal is to understand how FIX messages are represented,
validated, typed, sequenced, and eventually exchanged over a session.

At the moment this project is closer to a parser and dictionary validator than a
full engine. That is intentional: the engine will be built up in layers.

## Current Status

The current codebase implements the foundation of a FIX engine: message parsing,
wire-level validation, dictionary-backed metadata, and typed wrappers for core
admin messages.

- Parse raw FIX wire messages in `tag=value<SOH>` format.
- Preserve field order while also supporting tag lookup.
- Validate `BodyLength` / `CheckSum`.
- Serialize outbound messages with recalculated `BodyLength` / `CheckSum`.
- Load a FIX 4.2 XML dictionary.
- Resolve tag names and enum descriptions from the dictionary.
- Validate required header, trailer, and message fields for a `MsgType`.
- Provide basic typed wrappers for common admin messages:
  - `Heartbeat`
  - `Logon`
  - `Logout`
  - `TestRequest`
  - `ResendRequest`
  - `SequenceReset`
- Provide an initial session layer for admin message construction, session
  state, sequence-number tracking, TestRequest responses, Logout handling, and
  resend requests for sequence gaps.
- Unit tests for parser, dictionary, heartbeat, and session behaviour.

The next stage is to make the session layer more complete: heartbeat
scheduling, resend/gap-fill replay, persistence, and transport.

## Why This Exists

FIX looks simple at first because the wire format is just repeated
`tag=value` fields. The difficult part is everything around the message:

- Is the message structurally valid?
- Are required fields present for this message type?
- Are sequence numbers correct?
- What happens when a message is missing?
- When should a session send a heartbeat?
- How do logon, logout, resend, and gap-fill flows work?

This repository is for learning those pieces by implementing them directly.

## Requirements

- CMake 3.15 or newer
- A C++20 compiler

Dependencies are fetched by CMake:

- [Catch2](https://github.com/catchorg/Catch2) for tests
- [pugixml](https://github.com/zeux/pugixml) for XML parsing

## Build

```sh
cmake -S . -B build
cmake --build build -j
```

## Run Tests

```sh
ctest --test-dir build --output-on-failure
```

You can also run the test binaries directly:

```sh
./build/tests
./build/dict_tests
./build/heartbeat_tests
./build/session_tests
```

## Run Demo

```sh
./build/main
```

## Example

FIX messages use ASCII `SOH` (`\x01`) as the field separator.

```cpp
#include "FIXDictionary.h"
#include "Logon.h"

std::string raw =
    "8=FIX.4.2\x01"
    "9=76\x01"
    "35=A\x01"
    "49=BuySide\x01"
    "56=SellSide\x01"
    "34=1\x01"
    "52=20190605-11:27:06.897\x01"
    "98=0\x01"
    "108=30\x01"
    "141=Y\x01"
    "10=008\x01";

Logon logon(raw);
FIXDictionary dictionary;

bool wireValid = logon.validate();
bool dictionaryValid = dictionary.validate(logon);

std::string heartbeatInterval = logon.getHeartBtInt();
```

## Development Roadmap

The intended direction is to grow the project from a parser into a small
educational FIX engine.

1. Clean up outbound message construction.
   - Expand outbound builders beyond the initial admin/session helpers.

2. Strengthen dictionary validation.
   - Validate field types such as `INT`, `BOOLEAN`, `CHAR`, `UTCTIMESTAMP`.
   - Validate enum domains.
   - Reject unknown fields where appropriate.
   - Improve error reporting instead of returning only `true` / `false`.

3. Model admin messages properly.
   - Add stronger tests for `Logon`, `Logout`, `TestRequest`,
     `ResendRequest`, and `SequenceReset`.
   - Separate required and optional fields clearly.
   - Add constructors/builders for outbound admin messages.

4. Extend session state.
   - Track sender and target sequence numbers across reconnects.
   - Handle low sequence numbers and duplicate messages more explicitly.
   - Add richer session errors instead of silently returning no responses.

5. Implement heartbeat flow.
   - Send heartbeats on an interval.
   - Send test requests when the peer is silent.
   - Disconnect or mark the session unhealthy after timeout.

6. Implement resend and gap-fill flow.
   - Detect sequence gaps.
   - Send `ResendRequest`.
   - Process `SequenceReset`.
   - Store sent messages so they can be resent.

7. Add transport.
   - Start with a simple TCP initiator.
   - Add acceptor support later.
   - Keep parsing/framing separate from socket code.

8. Add application messages (Application layer):
   - Start with a small subset such as `NewOrderSingle` and
     `ExecutionReport`.
   - Keep application logic separate from session logic.


