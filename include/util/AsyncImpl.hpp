/*
 * Distrubuted under The MIT License (MIT)
 *
 * Copyright (c) 2016 Aaron Bishop
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* 
 * Much of this code was altered from AsyncSerial and BufferedAsyncSerial by 
 * Terraneo Federico which was distributed under the Boost Software License, Version 1.0.
 */


#ifndef BREWPIPP_UTIL_ASYNC_IMPL_HPP
#define BREWPIPP_UTIL_ASYNC_IMPL_HPP

#include <memory>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <array>
#include <vector>

namespace brewpipp {
	namespace util {
		class AsyncImpl {
		public:
			static constexpr size_t read_buffer_size = 512;
			AsyncImpl();
			void open() throw(boost::system::system_error);
			void close() throw(boost::system::system_error);
			
			void write(const char* data_begin, const char* data_end) throw(boost::system::system_error);
			std::shared_ptr<std::string> readStringUntil(const std::string& delim) throw(boost::system::system_error);
			void finishWrite(const boost::system::error_code& ec, const size_t& bytes_transferred);
			void finishRead(const boost::system::error_code& ec, const size_t& bytes_transferred);
		protected:
			boost::asio::io_service io;
			boost::thread io_runner;
			boost::mutex read_mutex;
			boost::mutex write_mutex;
			boost::atomics::atomic<boost::system::error_code> error;
			boost::atomics::atomic_bool is_open;
			std::vector<char> read_queue;
			std::array<char, read_buffer_size> read_buffer;
			std::vector<char> write_queue;
			std::unique_ptr<char[]> write_buffer;
			size_t write_buffer_size;
			
			typedef boost::lock_guard<boost::mutex> lock_t;
			
			virtual void _configure() throw(boost::system::system_error) = 0;
			virtual void _close() throw(boost::system::system_error) = 0;
			
			void startRead();
			void startWrite();
			
			virtual void _asyncWrite() = 0;
			virtual void _asyncRead() = 0;
			
		private:
			void run();
			boost::asio::io_service::work work;
		};
	}
}

#endif //BREWPIPP_UTIL_ASYNC_IMPL_HPP