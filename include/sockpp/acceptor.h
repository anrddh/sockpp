/// @file acceptor.h
///
/// Class for a TCP server to accept incoming connections.
///
/// @author	Frank Pagliughi
///	@author	SoRo Systems, Inc.
///	@author	www.sorosys.com
///
/// @date	December 2014

// --------------------------------------------------------------------------
// This file is part of the "sockpp" C++ socket library.
//
// Copyright (c) 2014-2019 Frank Pagliughi
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

#ifndef __sockpp_acceptor_h
#define __sockpp_acceptor_h

#include "sockpp/inet_address.h"
#include "sockpp/stream_socket.h"

namespace sockpp {

/////////////////////////////////////////////////////////////////////////////

/// Class for creating a streaming server.
/// Objects of this class bind and listen on streaming ports for incoming
/// connections. Normally, a server thread creates one of these and blocks
/// on the call to accept incoming connections. The call to accept creates
/// and returns a @ref stream_socket which can then be used for the actual
/// communications.

class acceptor : public socket
{
	// Non-copyable
	acceptor(const acceptor&) =delete;
	acceptor& operator=(const acceptor&) =delete;

protected:
	/**
	 * The default listener queue size.
	 */
	static const int DFLT_QUE_SIZE = 4;
	/**
	 * The local address to which the acceptor is bound.
	 */
	sock_address addr_;
	/**
	 * Binds the socket to the specified address.
	 * @param addr The address to which we get bound.
	 * @return @em true on success, @em false on error
	 */
	bool bind(const sockaddr* addr, socklen_t len);
	/**
	 * Sets the socket listening on the address to which it is bound.
	 * @param queSize The listener queue size.
	 * @return @em true on success, @em false on error
	 */
	bool listen(int queSize) {
		return check_ret_bool(::listen(handle(), queSize));
	};

public:
	/**
	 * Creates an unconnected acceptor.
	 */
	acceptor() {}
    /**
     * Creates an acceptor socket and starts it listening to the specified
     * address.
     * @param addr The address to which this server should be bound.
	 * @param queSize The listener queue size.
	 */
    acceptor(sock_address_ref addr, int queSize=DFLT_QUE_SIZE) {
        open(addr.sockaddr_ptr(), addr.size(), queSize);
    }
	/**
	 * Gets the local address to which we are bound.
	 * @return The local address to which we are bound.
	 */
	sock_address addr() const { return addr_; }
	/**
	 * Opens the acceptor socket and binds it to the specified address.
	 * @param addr The address to which this server should be bound.
	 * @param queSize The listener queue size.
	 * @return @em true on success, @em false on error
	 */
	bool open(const sockaddr* addr, socklen_t len, int queSize=DFLT_QUE_SIZE);
	/**
	 * Opens the acceptor socket and binds it to the specified address.
	 * @param addr The address to which this server should be bound.
	 * @param queSize The listener queue size.
	 * @return @em true on success, @em false on error
	 */
	bool open(const sock_address& addr, int queSize=DFLT_QUE_SIZE) {
		return open(addr.sockaddr_ptr(), addr.size(), queSize);
	}
	/**
	 * Opens the acceptor socket and binds it to the specified address.
	 * @param addr The address to which this server should be bound.
	 * @param queSize The listener queue size.
	 * @return @em true on success, @em false on error
	 */
	bool open(const sock_address_ref& addr, int queSize=DFLT_QUE_SIZE) {
		return open(addr.sockaddr_ptr(), addr.size(), queSize);
	}
	/**
	 * Accepts an incoming TCP connection and gets the address of the client.
	 * @param clientAddr Pointer to the variable that will get the
	 *  				 address of a client when it connects.
	 * @return A socket to the remote client.
	 */
	stream_socket accept(sock_address* clientAddr=nullptr);
};

/////////////////////////////////////////////////////////////////////////////
// end namespace sockpp
};

#endif		// __sockpp_acceptor_h

