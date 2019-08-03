/**
 * @file stream_socket.h
 *
 * Classes for stream sockets.
 *
 * @author Frank Pagliughi
 * @author SoRo Systems, Inc.
 * @author www.sorosys.com
 *
 * @date December 2014
 */

// --------------------------------------------------------------------------
// This file is part of the "sockpp" C++ socket library.
//
// Copyright (c) 2014-2017 Frank Pagliughi
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

#ifndef __sockpp_stream_socket_h
#define __sockpp_stream_socket_h

#include "sockpp/socket.h"
#include "sockpp/inet_address.h"
#include "sockpp/inet6_address.h"

namespace sockpp {

/////////////////////////////////////////////////////////////////////////////

/**
 * Base class for streaming sockets, such as TCP and Unix Domain.
 * This is the streaming connection between two peers. It looks like a
 * readable/writeable device.
 */
class stream_socket : public socket
{
protected:
	/**
	 * Creates a streaming socket.
	 * @return An OS handle to a TCP socket.
	 */
	static socket_t create(int domain=AF_INET) {
		return (socket_t) ::socket(domain, SOCK_STREAM, 0);
	}

public:
	/**
	 * Creates an unconnected streaming socket.
	 */
	stream_socket() {}
	/**
     * Creates a streaming socket from an existing OS socket handle and
     * claims ownership of the handle.
	 * @param sock A socket handle from the operating system.
	 */
	explicit stream_socket(socket_t sock) : socket(sock) {}
	/**
	 * Creates a stream socket by copying the socket handle from the
	 * specified socket object and transfers ownership of the socket.
	 */
	stream_socket(stream_socket&& sock) : socket(std::move(sock)) {}

	/**
	 * Open the socket.
	 * @return @em true on success, @em false on failure.
	 */
	bool open();

	/**
	 * Reads from the port
	 * @param buf Buffer to get the incoming data.
	 * @param n The number of bytes to try to read.
	 * @return The number of bytes read on success, or @em -1 on error.
	 */
	virtual ssize_t read(void *buf, size_t n);
	/**
	 * Best effort attempts to read the specified number of bytes.
	 * This will make repeated read attempts until all the bytes are read in
	 * or until an error occurs.
	 * @param buf Buffer to get the incoming data.
	 * @param n The number of bytes to try to read.
	 * @return The number of bytes read on success, or @em -1 on error. If
	 *  	   successful, the number of bytes read should always be 'n'.
	 */
	virtual ssize_t read_n(void *buf, size_t n);
	/**
	 * Set a timeout for read operations.
	 * Sets the timeout that the device uses for read operations. Not all
	 * devices support timeouts, so the caller should prepare for failure.
	 * @param to The amount of time to wait for the operation to complete.
	 * @return @em true on success, @em false on failure.
	 */
	virtual bool read_timeout(const std::chrono::microseconds& to);
	/**
	 * Set a timeout for read operations.
	 * Sets the timout that the device uses for read operations. Not all
	 * devices support timouts, so the caller should prepare for failure.
	 * @param to The amount of time to wait for the operation to complete.
	 * @return @em true on success, @em false on failure.
	 */
	template<class Rep, class Period>
	bool read_timeout(const std::chrono::duration<Rep,Period>& to) {
		return read_timeout(std::chrono::duration_cast<std::chrono::microseconds>(to));
	}
	/**
	 * Writes the buffer to the socket.
	 * @param buf The buffer to write
	 * @param n The number of bytes in the buffer.
	 * @return The number of bytes written, or @em -1 on error.
	 */
	virtual ssize_t write(const void *buf, size_t n);
	/**
	 * Best effort attempt to write the whole buffer to the socket.
	 * @param buf The buffer to write
	 * @param n The number of bytes in the buffer.
	 * @return The number of bytes written, or @em -1 on error. If
	 *  	   successful, the number of bytes written should always be 'n'.
	 */
	virtual ssize_t write_n(const void *buf, size_t n);
	/**
	 * Best effort attempt to write a string to the socket.
	 * @param s The string to write.
	 * @return The number of bytes written, or @em 1 on error. On success,
	 *  	   the number of bytes written should always be the length of
	 *  	   the string.
	 */
	virtual int write(const std::string& s) {
		return write_n(s.data(), s.size());
	}
	/**
	 * Set a timeout for write operations.
	 * Sets the timout that the device uses for write operations. Not all
	 * devices support timouts, so the caller should prepare for failure.
	 * @param to The amount of time to wait for the operation to complete.
	 * @return @em true on success, @em false on failure.
	 */
	virtual bool write_timeout(const std::chrono::microseconds& to);
	/**
	 * Set a timeout for write operations.
	 * Sets the timout that the device uses for write operations. Not all
	 * devices support timouts, so the caller should prepare for failure.
	 * @param to The amount of time to wait for the operation to complete.
	 * @return @em true on success, @em false on failure.
	 */
	template<class Rep, class Period>
	bool write_timeout(const std::chrono::duration<Rep,Period>& to) {
		return write_timeout(std::chrono::duration_cast<std::chrono::microseconds>(to));
	}
};

/**
 * Socket for IPv4 stream.
 */
class tcp_socket : public stream_socket
{
public:
	/**
     * Creates a streaming socket from an existing OS socket handle and
     * claims ownership of the handle.
	 * @param sock A socket handle from the operating system.
	 */
	explicit tcp_socket(socket_t sock) : stream_socket(sock) {}
	/**
	 * Creates a stream socket by copying the socket handle from the
	 * specified socket object and transfers ownership of the socket.
	 */
	tcp_socket(tcp_socket&& sock) : stream_socket(std::move(sock)) {}
	/**
	 * Gets the local address to which the socket is bound.
	 * @return The local address to which the socket is bound.
	 * @throw sys_error on error
	 */
	inet_address address() const {
        return inet_address(socket::address());
    }
	/**
	 * Gets the address of the remote peer, if this socket is connected.
	 * @return The address of the remote peer, if this socket is connected.
	 * @throw sys_error on error
	 */
	inet_address peer_address() const {
        return inet_address(socket::peer_address());
    }
};

/**
 * Socket for IPv6 stream.
 */
class tcp6_socket : public stream_socket
{
public:
	/**
     * Creates a streaming socket from an existing OS socket handle and
     * claims ownership of the handle.
	 * @param sock A socket handle from the operating system.
	 */
	explicit tcp6_socket(socket_t sock) : stream_socket(sock) {}
	/**
	 * Creates a stream socket by copying the socket handle from the
	 * specified socket object and transfers ownership of the socket.
	 */
	tcp6_socket(tcp6_socket&& sock) : stream_socket(std::move(sock)) {}
	/**
	 * Gets the local address to which the socket is bound.
	 * @return The local address to which the socket is bound.
	 * @throw sys_error on error
	 */
	inet6_address address() const {
        return inet6_address(socket::address());
    }
	/**
	 * Gets the address of the remote peer, if this socket is connected.
	 * @return The address of the remote peer, if this socket is connected.
	 * @throw sys_error on error
	 */
	inet6_address peer_address() const {
        return inet6_address(socket::peer_address());
    }
};

/**
 * Stream for Unix Sockets
 */
using unix_socket = stream_socket;

/////////////////////////////////////////////////////////////////////////////
// end namespace sockpp
}

#endif		// __sockpp_socket_h
