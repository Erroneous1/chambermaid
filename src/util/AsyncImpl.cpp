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

#include "util/AsyncImpl.hpp"

namespace brewpipp {
	namespace util {
		AsyncImpl::AsyncImpl() : io(), io_runner(), read_mutex(), write_mutex(), error(), is_open(false), read_queue(), read_buffer(), write_buffer(), write_buffer_size(0), work(io) {}
		
		void AsyncImpl::open() throw(boost::system::system_error) {
			close();
			
			this->_configure();
			if(!is_open.load()) {
				auto ec = boost::system::error_code(-1, boost::system::generic_category());
				error = ec;
				throw boost::system::system_error(ec, "Could not open");
			}
			io.reset();
			io.post(boost::bind(&AsyncImpl::startRead, this));
			boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
			io_runner.swap(t);
		}
		
		void AsyncImpl::close() throw(boost::system::system_error) {
			if(!is_open.load())
				return;
			
			io.stop();
			{
				lock_t write_lock(write_mutex);
				write_buffer.reset();
				write_buffer_size = 0;
			}
			this->_close();
		}
		
		void AsyncImpl::write(const char* data_begin, const char* data_end) throw(boost::system::system_error) {
			auto ec = error.load();
			if(ec)
				throw boost::system::system_error(ec, "Unable to write due to error");
			
			lock_t write_lock(write_mutex);
			if(write_buffer_size == 0) {
				write_buffer_size = data_end - data_begin;
				write_buffer.reset(new char[write_buffer_size]);
				std::copy(data_begin, data_end, write_buffer.get());
				io.post(boost::bind(&AsyncImpl::startWrite, this));
			} else {
				write_queue.insert(write_queue.end(), data_begin, data_end);
			}
		}
		
		std::shared_ptr<std::string> AsyncImpl::readStringUntil(const std::string& delim) throw(boost::system::system_error) {
			auto ec = error.load();
			if(ec)
				throw boost::system::system_error(ec, "Unable to read due to error");
			
			lock_t read_lock(read_mutex);
			const size_t delim_size = delim.size();
			if(read_queue.size() < delim_size)
				return std::shared_ptr<std::string>(nullptr);
			
			auto end = read_queue.cend() - delim_size;
			auto begin = read_queue.begin();
			auto found = begin;
			
			for(; found != end; found++) {
				bool is_found(true);
				for(size_t i = 0; i < delim_size; i++) {
					if(*(found+i) != delim[i]) {
						is_found = false;
						break;
					}
				}
				if(is_found) {
					break;
				}
			}
			if(found == end) {
				return std::shared_ptr<std::string>(nullptr);
			} else {
				auto ret = std::make_shared<std::string>(begin, found);
				read_queue.erase(begin, found+delim_size);
				return ret;
			}
		}
		void AsyncImpl::startWrite() {
			this->_asyncWrite();
		}
		void AsyncImpl::startRead() {
			lock_t read_lock(read_mutex);
			std::memset(read_buffer.data(), 0, read_buffer_size);
			this->_asyncRead();
		}
		void AsyncImpl::finishWrite(const boost::system::error_code& ec, const size_t& bytes_transferred) {
			if(ec) {
				error.store(ec);
			} else {
				lock_t write_lock(write_mutex);
				if(write_queue.empty()) {
					write_buffer_size = 0;
				} else {
					write_buffer_size = write_queue.size();
					write_buffer.reset(new char[write_buffer_size]);
					std::copy(write_queue.begin(), write_queue.end(), write_buffer.get());
					write_queue.clear();
					io.post(boost::bind(&AsyncImpl::startWrite, this));
				}
			}
		}
		
		void AsyncImpl::finishRead(const boost::system::error_code& ec, const size_t& bytes_transferred) {
			if(ec) {
				error.store(ec);
			} else {
				lock_t read_lock(read_mutex);
				read_queue.insert(read_queue.end(), read_buffer.data(), read_buffer.data()+bytes_transferred);
				io.post(boost::bind(&AsyncImpl::startRead, this));
			}
		}

		void AsyncImpl::run() {
			for(; is_open.load();) {
				io.run_one();
			}
		}


	}
}