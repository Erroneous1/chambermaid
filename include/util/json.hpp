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

#ifndef BREWPIPP_UTIL_JSON_HPP
#define BREWPIPP_UTIL_JSON_HPP

#include <json/json.h>
#include <vector>
#include <string>
#include <iterator>

namespace Json {
	template<class InputIterable>
	Value FromValueList( const InputIterable begin, const InputIterable end ) {
		Value v;
		for( auto itr = begin; itr != end; itr++ ) {
			v.append(*itr);
		}
		return v;
	}
	
	template<class T>
	Value FromValueList( const T& t ) { return FromValueList( t.begin(), t.end() ); }
	
	template<class InputIterable>
	Value FromValueSet( const InputIterable begin, const InputIterable end ) {
		Value v;
		for( auto itr = begin; itr != end; itr++ ) {
			v[itr->first] = itr->second;
		}
		return v;
	}
	
	template<class T>
	Value FromValueSet( const T& t ) { return FromValueSet( t.begin(), t.end() ); }
	
	typedef std::vector<std::pair<std::string,Value> > ValueSet;
	typedef std::vector<Value> ValueList;
}

#endif //BREWPIPP_UTIL_JSON_HPP