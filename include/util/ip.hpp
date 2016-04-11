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

#ifndef BREWPIPP_UTIL_IP
#define BREWPIPP_UTIL_IP

#include <string>
#include <array>
#include <cstdint>

namespace brewpipp {
	namespace util {
		class ip{
		public:
			typedef std::array<uint8_t,16> ip_data_t;
			ip(const char* s) throw(std::exception);
			explicit ip(const std::string& d) throw(std::exception);
			ip(const ip_data_t& d) : data(d) {}
			ip(const ip& copy) = default;
			ip(ip&& move) = default;
			
			ip& operator=(const ip& copy) = default;
			ip& operator=(ip&& move) = default;
			
			std::string to_string() const;
			std::string value() const;
			
			ip operator&(const ip& rhs) const;
			bool operator==(const ip& rhs) const;
		private:
			ip_data_t data;
			
		};
		
		class netmask {
		public:
			netmask (const ip& ip, const uint8_t& cidr);
			
			bool operator==(const ip& rhs) const;
			
			static netmask ipv4_map() { return netmask(ip(ip::ip_data_t({0,0,0,0,0,0,0,0,0,0,0xFF,0xFF,0,0,0,0})),96); }
		private:
			const ip& m_ip;
			const ip& m_nm;
		};
	}
}

#endif //BREWPIPP_UTIL_IP