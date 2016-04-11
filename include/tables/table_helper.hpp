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

#ifndef BREWPIPP_TABLES_TABLE_HELPER_HPP
#define BREWPIPP_TABLES_TABLE_HELPER_HPP

#define SQLPP_COLUMN_NAMED(x) struct _alias_t {\
	static constexpr const char _literal[] = #x;\
	using _name_t = sqlpp::make_char_sequence<sizeof(_literal),_literal>;\
	template<typename T>\
	struct _member_t {\
		T x;\
		T& operator()(){ return x; }\
		const T& operator()() const { return x; }\
	};\
};

#define SQLPP_TABLE_NAMED(x) struct _alias_t {\
	static constexpr const char _literal[] = #x;\
	using _name_t = sqlpp::make_char_sequence<sizeof(_literal),_literal>;\
	template<typename T>\
	struct _member_t {\
		T x;\
		T& operator()(){ return x; }\
		const T& operator()() const { return x; }\
	};\
};

#endif //BREWPIPP_TABLES_TABLE_HELPER_HPP