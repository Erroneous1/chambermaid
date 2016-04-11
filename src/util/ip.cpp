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

#include "util/ip.hpp"
#include <stdexcept>
#include <cassert>
#include <cstdio>
#include <boost/algorithm/string.hpp>


namespace brewpipp {
	namespace util {
		namespace {
			void input_hex_pair(uint8_t& a, uint8_t& b, const std::string& s) {
				unsigned int p;
				if(s.empty()) {
					a = b = 0;
				}else{
					std::sscanf(s.c_str(),"%x",&p);
					a = static_cast<uint8_t>((p >> 8) & 0xFF);
					b = static_cast<uint8_t>(p & 0xFF);
				}
			}
			
			ip::ip_data_t ip_from_cidr(uint8_t cidr) {
				ip::ip_data_t ret;
				size_t i(0);
				while(i < 16 && cidr > 8){
					ret[i++] = 0xFF;
					cidr -= 8;
				}
				if(i < 16) {
					ret[i++] = ~cidr;
				}
				while(i < 16) {
					ret[i++] = 0;
				}
				return ret;
			}
		}
		
		ip::ip(const char* s) throw(std::exception) : data({0}) {
			std::vector<const char*> dot_locations;
			std::vector<const char*> colon_locations;
			dot_locations.reserve(3);
			colon_locations.reserve(7);
			for(const char* c = s; *c != 0; c++) {
				switch(*c) {
					case ':':
						colon_locations.push_back(c);
						break;
					case '.':
						dot_locations.push_back(c);
						break;
					default: break;
				}
			}
			if(dot_locations.size() == 3) { //IPv4 address
				data[10] = data[11] = 0xFF;
				if(colon_locations.size() >= 2) { //IPv6 mapped IPv4 (::ffff:XXX.xxx.XXX.xxx)
					data[12] = static_cast<uint8_t>(atoi(colon_locations.back()+1));
				} else {
					data[12] = static_cast<uint8_t>(atoi(s));
				}
				for(size_t i = 0; i < 3; i++) {
					data[13+i] = static_cast<uint8_t>(atoi(dot_locations[i]+1));
				}
			} else if(colon_locations.size() >= 2 && colon_locations.size() <= 8) { //IPv6 address
				size_t i = 0, j = 0;
				std::vector<std::string> hex_pairs;
				hex_pairs.reserve(8);
				hex_pairs.push_back(std::string(s,colon_locations.front()));
				bool has_inserted = false;
				for(auto itr = colon_locations.begin(); itr != colon_locations.end(); itr++) {
					const auto itr2 = itr+1;
					std::string p;
					if(itr2 == colon_locations.end()) {
						p = std::string((*itr)+1);
					} else {
						p = std::string(*itr+1,*itr2);
					}
					if(p.empty()) {
						if(has_inserted) {
							hex_pairs.push_back(std::string());
						} else {
							hex_pairs.insert(hex_pairs.end(), 8-colon_locations.size(), std::string());
							has_inserted = true;
						}
					} else {
						hex_pairs.push_back(p);
					}
				}
				for(size_t i = 0; i < 8; i++) {
					input_hex_pair(data[i*2], data[i*2+1], hex_pairs[i]);
				}
			} else {
				throw std::invalid_argument("Invalid number of .("+std::to_string(dot_locations.size())+") or :("+std::to_string(colon_locations.size())+") from ip "+std::string(s));
			}
		}
        ip::ip ( const std::string& d ) throw(std::exception) : data({0}) {
			if(d.length() == data.size()) {
				for(size_t i = 0; i < data.size(); i++) {
					data[i] = (uint8_t)d[i];
				}
			} else if(d.length() == 4) {
				data[10] = data[11]= 0xFF;
				for(size_t i = 0; i < 4; i++) {
					data[i+12] = (uint8_t)d[i];
				}
			} else {
				throw std::invalid_argument("Invalid length("+std::to_string(d.length())+") for util::ip(const std::string&). Expected 4 or 16.");
			}
        }

		std::string ip::to_string() const {
			if(netmask::ipv4_map() == *this) {
				std::unique_ptr<char[]> buffer(new char[16]);
				return std::string(buffer.get(), std::snprintf(buffer.get(),16,"%u.%u.%u.%u",data[12],data[13],data[14],data[15]));
			} else {
				size_t longest_run_start(0), longest_run_end(0), current_run_start(16);
				
				for(size_t i = 0; i < 16; i+=2) {
					if(current_run_start < 16) { //run in progress
						if(data[i] != 0 || data[i+1] != 0) {//run finished
							size_t current_run_end = i;
							if(current_run_end - current_run_start - 1 > longest_run_end - longest_run_start) {
								longest_run_start = current_run_start;
								longest_run_end = current_run_end;
							}
							current_run_start = 16;
						}
					} else if(data[i] == 0 && data[i+1] == 0) { //run started
						current_run_start = i;
					}
				}
				if(current_run_start < 16) {
					longest_run_start = current_run_start;
					longest_run_end = 16;
				}
				std::vector<std::string> segments;
				std::unique_ptr<char[]> buffer(new char[5]);
				segments.reserve(8);
				for(size_t i = 0; i < longest_run_start; i+=2) {
					uint16_t n = (static_cast<uint16_t>(data[i]) << 8) | static_cast<uint16_t>(data[i+1]);
					segments.push_back(std::string(buffer.get(),std::snprintf(buffer.get(), 5, "%x", n)));
				}
				if(longest_run_start != longest_run_end) {
					segments.push_back(std::string());
					if(longest_run_start == 0) {
						segments.push_back(std::string());
					}
					if(longest_run_end == 16) {
						segments.push_back(std::string());
					}
				}
				for(size_t i = longest_run_end; i < 16; i+=2) {
					uint16_t n = (static_cast<uint16_t>(data[i]) << 8) | static_cast<uint16_t>(data[i+1]);
					segments.push_back(std::string(buffer.get(),std::snprintf(buffer.get(), 5, "%x", n)));
				}
				return boost::algorithm::join(segments,":");
			}
		}
		
		ip ip::operator&(const ip& rhs) const {
			ip_data_t ret_data;
			for(size_t i = 0; i < data.size(); i++) {
				ret_data[i] = (data[i] & rhs.data[i]);
			}
			return ip(ret_data);
		}
		bool ip::operator==(const ip& rhs) const {
			for(auto litr = data.begin(), ritr = rhs.data.begin(); litr != data.end(); ) {
				if(*litr != *ritr) {
					return false;
				}
				litr++;
				ritr++;
			}
			return true;
		}
		
		std::string ip::value() const {
			std::string ret;
			ret.reserve(data.size());
			for(const auto& c : data) {
				ret.push_back(static_cast<const char>(c));
			}
			return ret;
		}
		
		netmask::netmask ( const ip& _ip, const uint8_t& cidr ) : m_ip ( _ip ), m_nm(ip_from_cidr(cidr)) {}
		
		bool netmask::operator== ( const ip& rhs ) const {
			std::string d_nm = m_nm.value();
			std::string d_rhs = rhs.value();
			std::string d_lhs = m_ip.value();
			for(size_t i = 0; i < d_nm.size(); i++) {
				if((static_cast<uint8_t>(d_nm[i]) & static_cast<uint8_t>(d_lhs[i]))
					!=
				   (static_cast<uint8_t>(d_nm[i]) & static_cast<uint8_t>(d_rhs[i]))) {
					return false;
				}
			}
			return true;
		}

	}
}