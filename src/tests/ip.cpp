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
#include <iostream>
#include <vector>
#include <utility>

bool ipv6test(const char* text, const std::string& correct) {
	brewpipp::util::ip ip(text);
	if(ip.to_string().compare(correct)!=0) {
		std::cout << "Bad IPv6 conversion!" << std::endl
			<< "Input:  " << text << std::endl
			<< "Output: " << ip.to_string() << std::endl
			<< "Expect: " << correct << std::endl;
		std::string data = ip.value();
		std::cout << "Data:   ";
		for(const auto& c : data) {
			std::printf("%02x ",static_cast<uint8_t>(c));
		}
		std::cout << std::endl;
		std::cout << "IPv" << (brewpipp::util::netmask::ipv4_map() == ip ? '4' : '6') << std::endl;
		return false;
	}else{
		return true;
	}
}

int main() {
	std::vector<std::pair<std::string,std::string> > test_cases({
		{"::1","::1"},// loopback, compressed, non-routable 
		{"::","::"},// unspecified, compressed, non-routable 
		{"0:0:0:0:0:0:0:1","::1"},// loopback, full 
		{"0:0:0:0:0:0:0:0","::"},// unspecified, full 
		{"2001:DB8:0:0:8:800:200C:417A","2001:db8::8:800:200c:417a"},// unicast, full 
		{"FF01:0:0:0:0:0:0:101","ff01::101"},// multicast, full 
		{"2001:DB8::8:800:200C:417A","2001:db8::8:800:200c:417a"},// unicast, compressed 
		{"FF01::101","ff01::101"},// multicast, compressed 
		{"fe80::217:f2ff:fe07:ed62","fe80::217:f2ff:fe07:ed62"},
		{"2001:0000:1234:0000:0000:C1C0:ABCD:0876","2001:0:1234::c1c0:abcd:876"},
		{"3ffe:0b00:0000:0000:0001:0000:0000:000a","3ffe:b00::1:0:0:a"},
		{"FF02:0000:0000:0000:0000:0000:0000:0001","ff02::1"},
		{"0000:0000:0000:0000:0000:0000:0000:0001","::1"},
		{"0000:0000:0000:0000:0000:0000:0000:0000","::"},
		{"2::10","2::10"},
		{"ff02::1","ff02::1"},
		{"fe80::","fe80::"},
		{"2002::","2002::"},
		{"2001:db8::","2001:db8::"},
		{"2001:0db8:1234::","2001:db8:1234::"},
		{"::ffff:0:0","0.0.0.0"},
		{"1:2:3:4:5:6:7:8","1:2:3:4:5:6:7:8"},
		{"1:2:3:4:5:6::8","1:2:3:4:5:6::8"},
		{"1:2:3:4:5::8","1:2:3:4:5::8"},
		{"1:2:3:4::8","1:2:3:4::8"},
		{"1:2:3::8","1:2:3::8"},
		{"1:2::8","1:2::8"},
		{"1::8","1::8"},
		{"1::2:3:4:5:6:7","1::2:3:4:5:6:7"},
		{"1::2:3:4:5:6","1::2:3:4:5:6"},
		{"1::2:3:4:5","1::2:3:4:5"},
		{"1::2:3:4","1::2:3:4"},
		{"1::2:3","1::2:3"},
		{"1::8","1::8"},
		{"::2:3:4:5:6:7:8","::2:3:4:5:6:7:8"},
		{"::2:3:4:5:6:7","::2:3:4:5:6:7"},
		{"::2:3:4:5:6","::2:3:4:5:6"},
		{"::2:3:4:5","::2:3:4:5"},
		{"::2:3:4","::2:3:4"},
		{"::2:3","::2:3"},
		{"::8","::8"},
		{"1:2:3:4:5:6::","1:2:3:4:5:6::"},
		{"1:2:3:4:5::","1:2:3:4:5::"},
		{"1:2:3:4::","1:2:3:4::"},
		{"1:2:3::","1:2:3::"},
		{"1:2::","1:2::"},
		{"1::","1::"},
		{"1:2:3:4:5::7:8","1:2:3:4:5::7:8"},
		{"1:2:3:4::7:8","1:2:3:4::7:8"},
		{"1:2:3::7:8","1:2:3::7:8"},
		{"1:2::7:8","1:2::7:8"},
		{"1::7:8","1::7:8"},
		
		// IPv4 addresses as dotted-quads
		{"1:2:3:4:5:6:1.2.3.4","1.2.3.4"},
		{"1:2:3:4:5::1.2.3.4","1.2.3.4"},
		{"1:2:3:4::1.2.3.4","1.2.3.4"},
		{"1:2:3::1.2.3.4","1.2.3.4"},
		{"1:2::1.2.3.4","1.2.3.4"},
		{"1::1.2.3.4","1.2.3.4"},
		{"1:2:3:4::5:1.2.3.4","1.2.3.4"},
		{"1:2:3::5:1.2.3.4","1.2.3.4"},
		{"1:2::5:1.2.3.4","1.2.3.4"},
		{"1::5:1.2.3.4","1.2.3.4"},
		{"1::5:11.22.33.44","11.22.33.44"},
		{"fe80::217:f2ff:254.7.237.98","254.7.237.98"},
		{"::ffff:192.168.1.26","192.168.1.26"},
		{"::ffff:192.168.1.1","192.168.1.1"},
		{"0:0:0:0:0:0:13.1.68.3","13.1.68.3"},// IPv4-compatible IPv6 address, full, deprecated 
		{"0:0:0:0:0:FFFF:129.144.52.38","129.144.52.38"},// IPv4-mapped IPv6 address, full 
		{"::13.1.68.3","13.1.68.3"},// IPv4-compatible IPv6 address, compressed, deprecated 
		{"::FFFF:129.144.52.38","129.144.52.38"},// IPv4-mapped IPv6 address, compressed 
		{"fe80:0:0:0:204:61ff:254.157.241.86","254.157.241.86"},
		{"fe80::204:61ff:254.157.241.86","254.157.241.86"},
		{"::ffff:12.34.56.78","12.34.56.78"},
		{"::ffff:192.0.2.128","192.0.2.128"},   // but this is OK, since there's a single digit
		
		{"fe80:0000:0000:0000:0204:61ff:fe9d:f156","fe80::204:61ff:fe9d:f156"},
		{"fe80:0:0:0:204:61ff:fe9d:f156","fe80::204:61ff:fe9d:f156"},
		{"fe80::204:61ff:fe9d:f156","fe80::204:61ff:fe9d:f156"},
		{"fe80::","fe80::"},
		{"fe80::1","fe80::1"},
		{"::ffff:c000:280","192.0.2.128"},
		
		// Additional test cases
		// from http://rt.cpan.org/Public/Bug/Display.html?id=50693
		
		{"2001:0db8:85a3:0000:0000:8a2e:0370:7334","2001:db8:85a3::8a2e:370:7334"},
		{"2001:db8:85a3:0:0:8a2e:370:7334","2001:db8:85a3::8a2e:370:7334"},
		{"2001:db8:85a3::8a2e:370:7334","2001:db8:85a3::8a2e:370:7334"},
		{"2001:0db8:0000:0000:0000:0000:1428:57ab","2001:db8::1428:57ab"},
		{"2001:0db8:0000:0000:0000::1428:57ab","2001:db8::1428:57ab"},
		{"2001:0db8:0:0:0:0:1428:57ab","2001:db8::1428:57ab"},
		{"2001:0db8:0:0::1428:57ab","2001:db8::1428:57ab"},
		{"2001:0db8::1428:57ab","2001:db8::1428:57ab"},
		{"2001:db8::1428:57ab","2001:db8::1428:57ab"},
		{"0000:0000:0000:0000:0000:0000:0000:0001","::1"},
		{"::ffff:0c22:384e","12.34.56.78"},
		{"2001:0db8:1234:0000:0000:0000:0000:0000","2001:db8:1234::"},
		{"2001:0db8:1234:ffff:ffff:ffff:ffff:ffff","2001:db8:1234:ffff:ffff:ffff:ffff:ffff"},
		{"2001:db8:a::123","2001:db8:a::123"},
		{"fe80::","fe80::"},
		
		// New from Aeron 
		{"1111:2222:3333:4444:5555:6666:7777:8888","1111:2222:3333:4444:5555:6666:7777:8888"},
		{"1111:2222:3333:4444:5555:6666:7777::","1111:2222:3333:4444:5555:6666:7777::"},
		{"1111:2222:3333:4444:5555:6666::","1111:2222:3333:4444:5555:6666::"},
		{"1111:2222:3333:4444:5555::","1111:2222:3333:4444:5555::"},
		{"1111:2222:3333:4444::","1111:2222:3333:4444::"},
		{"1111:2222:3333::","1111:2222:3333::"},
		{"1111:2222::","1111:2222::"},
		{"1111::","1111::"},
		{"1111:2222:3333:4444:5555:6666::8888","1111:2222:3333:4444:5555:6666::8888"},
		{"1111:2222:3333:4444:5555::8888","1111:2222:3333:4444:5555::8888"},
		{"1111:2222:3333:4444::8888","1111:2222:3333:4444::8888"},
		{"1111:2222:3333::8888","1111:2222:3333::8888"},
		{"1111:2222::8888","1111:2222::8888"},
		{"1111::8888","1111::8888"},
		{"::8888","::8888"},
		{"1111:2222:3333:4444:5555::7777:8888","1111:2222:3333:4444:5555::7777:8888"},
		{"1111:2222:3333:4444::7777:8888","1111:2222:3333:4444::7777:8888"},
		{"1111:2222:3333::7777:8888","1111:2222:3333::7777:8888"},
		{"1111:2222::7777:8888","1111:2222::7777:8888"},
		{"1111::7777:8888","1111::7777:8888"},
		{"::7777:8888","::7777:8888"},
		{"1111:2222:3333:4444::6666:7777:8888","1111:2222:3333:4444::6666:7777:8888"},
		{"1111:2222:3333::6666:7777:8888","1111:2222:3333::6666:7777:8888"},
		{"1111:2222::6666:7777:8888","1111:2222::6666:7777:8888"},
		{"1111::6666:7777:8888","1111::6666:7777:8888"},
		{"::6666:7777:8888","::6666:7777:8888"},
		{"1111:2222:3333::5555:6666:7777:8888","1111:2222:3333::5555:6666:7777:8888"},
		{"1111:2222::5555:6666:7777:8888","1111:2222::5555:6666:7777:8888"},
		{"1111::5555:6666:7777:8888","1111::5555:6666:7777:8888"},
		{"::5555:6666:7777:8888","::5555:6666:7777:8888"},
		{"1111:2222::4444:5555:6666:7777:8888","1111:2222::4444:5555:6666:7777:8888"},
		{"1111::4444:5555:6666:7777:8888","1111::4444:5555:6666:7777:8888"},
		{"::4444:5555:6666:7777:8888","::4444:5555:6666:7777:8888"},
		{"1111::3333:4444:5555:6666:7777:8888","1111::3333:4444:5555:6666:7777:8888"},
		{"::3333:4444:5555:6666:7777:8888","::3333:4444:5555:6666:7777:8888"},
		{"::2222:3333:4444:5555:6666:7777:8888","::2222:3333:4444:5555:6666:7777:8888"},
		{"1111:2222:3333:4444:5555:6666:123.123.123.123","123.123.123.123"},
		{"1111:2222:3333:4444:5555::123.123.123.123","123.123.123.123"},
		{"1111:2222:3333:4444::123.123.123.123","123.123.123.123"},
		{"1111:2222:3333::123.123.123.123","123.123.123.123"},
		{"1111:2222::123.123.123.123","123.123.123.123"},
		{"1111::123.123.123.123","123.123.123.123"},
		{"::123.123.123.123","123.123.123.123"},
		{"1111:2222:3333:4444::6666:123.123.123.123","123.123.123.123"},
		{"1111:2222:3333::6666:123.123.123.123","123.123.123.123"},
		{"1111:2222::6666:123.123.123.123","123.123.123.123"},
		{"1111::6666:123.123.123.123","123.123.123.123"},
		{"::6666:123.123.123.123","123.123.123.123"},
		{"1111:2222:3333::5555:6666:123.123.123.123","123.123.123.123"},
		{"1111:2222::5555:6666:123.123.123.123","123.123.123.123"},
		{"1111::5555:6666:123.123.123.123","123.123.123.123"},
		{"::5555:6666:123.123.123.123","123.123.123.123"},
		{"1111:2222::4444:5555:6666:123.123.123.123","123.123.123.123"},
		{"1111::4444:5555:6666:123.123.123.123","123.123.123.123"},
		{"::4444:5555:6666:123.123.123.123","123.123.123.123"},
		{"1111::3333:4444:5555:6666:123.123.123.123","123.123.123.123"},
		{"::2222:3333:4444:5555:6666:123.123.123.123","123.123.123.123"},
		
		// Playing with combinations of "0" and "::"
		// NB: these are all sytactically correct, but are bad form 
		//   because "0" adjacent to "::" should be combined into "::"
		{"::0:0:0:0:0:0:0","::"},
		{"::0:0:0:0:0:0","::"},
		{"::0:0:0:0:0","::"},
		{"::0:0:0:0","::"},
		{"::0:0:0","::"},
		{"::0:0","::"},
		{"::0","::"},
		{"0:0:0:0:0:0:0::","::"},
		{"0:0:0:0:0:0::","::"},
		{"0:0:0:0:0::","::"},
		{"0:0:0:0::","::"},
		{"0:0:0::","::"},
		{"0:0::","::"},
		{"0::","::"},
		
		// Additional cases: http://crisp.tweakblogs.net/blog/2031/ipv6-validation-%28and-caveats%29.html
		{"0:a:b:c:d:e:f::","0:a:b:c:d:e:f::"},
		{"::0:a:b:c:d:e:f","::a:b:c:d:e:f"}, // syntactically correct, but bad form (::0:... could be combined)
		{"a:b:c:d:e:f:0::","a:b:c:d:e:f::"}
	});
	for(size_t i = 0; i < test_cases.size(); i++) {
		if(!ipv6test(test_cases[i].first.c_str(),test_cases[i].second)) {
			std::cout << "failed test case " << i << std::endl;
			return 1;
		}
	}
	std::cout << "Successfully passed " << test_cases.size() << " tests" << std::endl;
	return 0;
}