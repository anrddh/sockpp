// test_unix_address.cpp
//
// Unit tests for the `unix_address` class.
//

// --------------------------------------------------------------------------
// This file is part of the "sockpp" C++ socket library.
//
// Copyright (c) 2018 Frank Pagliughi
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// --------------------------------------------------------------------------
//

#include "catch2/catch.hpp"
#include "sockpp/unix_address.h"
#include <string>

using namespace sockpp;

std::string PATH { "/tmp/sock" };

TEST_CASE("unix_address default constructor", "[address]") {
    unix_address addr;

    REQUIRE(!addr.is_set());
    REQUIRE(addr.path().empty());
    REQUIRE(sizeof(sockaddr_un) == addr.size());

    // TODO: Do we have a unix_address::create() method yet?
}

TEST_CASE("unix_address path constructor", "[address]") {
    unix_address addr(PATH);

    REQUIRE(addr.is_set());
    REQUIRE(PATH == addr.path());
    REQUIRE(sizeof(sockaddr_un) == addr.size());

    // Check the low-level struct
    REQUIRE(AF_UNIX == addr.sun_family);
    REQUIRE(0 == strcmp(PATH.c_str(), (const char*) &addr.sun_path));

    SECTION("copy constructor") {
        unix_address addr2(addr);

        REQUIRE(addr2.is_set());
        REQUIRE(PATH == addr2.path());
        REQUIRE(sizeof(sockaddr_un) == addr2.size());

        // Check the low-level struct
        REQUIRE(AF_UNIX == addr2.sun_family);
        REQUIRE(0 == strcmp(PATH.c_str(), (const char*) &addr2.sun_path));
    }

    SECTION("sockaddr conversions") {
        auto sa = addr.sockaddr_ptr();
        unix_address addr2(*sa);

        REQUIRE(addr2.is_set());
        REQUIRE(PATH == addr2.path());
        REQUIRE(sizeof(sockaddr_un) == addr2.size());

        // Check the low-level struct
        REQUIRE(AF_UNIX == addr2.sun_family);
        REQUIRE(0 == strcmp(PATH.c_str(), (const char*) &addr2.sun_path));
    }
}

TEST_CASE("unix_address sockaddr_un constructor", "[address]") {
    sockaddr_un unaddr;
    unaddr.sun_family = AF_UNIX;
    strcpy(unaddr.sun_path, PATH.c_str());

    unix_address addr(unaddr);

    REQUIRE(addr.is_set());
    REQUIRE(PATH == addr.path());
    REQUIRE(sizeof(sockaddr_un) == addr.size());

    // Check the low-level struct
    REQUIRE(AF_UNIX == addr.sun_family);
    REQUIRE(0 == strcmp(PATH.c_str(), (const char*) &addr.sun_path));

    SECTION("reject bad sockaddr_un") {
        unaddr.sun_family = AF_INET;
        REQUIRE_THROWS_AS([&] {
            unix_address addr2(unaddr);
        }(), std::invalid_argument);
    }
}


