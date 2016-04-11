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

#ifndef BREWPIPP_AUTHORITIES_HPP
#define BREWPIPP_AUTHORITIES_HPP

#include <bitset>
#include <sqlpp11/odbc/connection.h>

namespace brewpipp {
	namespace util {
		class QEntry;
	}
	constexpr size_t AuthoritiesCount = 4;
	class Authorities : public std::bitset<AuthoritiesCount> {
	public:
		Authorities(const int64_t& v) : std::bitset<AuthoritiesCount>(v) {}
		Authorities(util::QEntry& req, sqlpp::odbc::connection& db);
		
		int64_t value() const { return to_ullong(); }
		
		bool viewStatus() const { return operator[](0); }
		reference viewStatus() { return operator[](0); }
		
		bool viewHistory() const { return operator[](1); }
		reference viewHistory() { return operator[](1); }
		
		bool viewSettings() const { return operator[](2); }
		reference viewSettings() { return operator[](2); }
		
		bool editSettings() const { return operator[](3); }
		reference editSettings() { return operator[](3); }
	};
}

#endif //BREWPIPP_AUTHORITIES_HPP