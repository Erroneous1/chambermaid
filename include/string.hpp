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

#ifndef BREWPIPP_STRING_HPP
#define BREWPIPP_STRING_HPP

#include <string>

namespace brewpipp {
#ifdef __windows__
	#include <Windows.h>
	typedef wchar_t char_t;
	typedef std::basic_string<char_t> string;
	#define _UNICODE
	#define BC(x) L ## x
	std::string to_string(const string& s);
	#define BREWPIPP_WIDE_CHAR
#else
	typedef char char_t;
	typedef std::basic_string<char_t> string;
	#define BC(x) u8 ## x
	inline std::string to_string(const string& s){ return s; }
#endif
	
}

#endif //BREWPIPP_STRING_HPP