# The MIT License (MIT)
# 
# Copyright (c) 2016 Aaron Bishop
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

add_library(util_ip OBJECT ip.cpp)
add_library(util_tty OBJECT tty.cpp)
add_library(util_serial OBJECT serial.cpp)
add_library(util_version OBJECT version.cpp)
add_library(util_buffered_io OBJECT BufferedIO.cpp)
add_library(util_async_impl OBJECT AsyncImpl.cpp)
add_library(util_buffered_serial OBJECT BufferedSerial.cpp)

add_library(util
	$<TARGET_OBJECTS:util_ip>
	$<TARGET_OBJECTS:util_tty>
	$<TARGET_OBJECTS:util_serial>
	$<TARGET_OBJECTS:util_version>
	$<TARGET_OBJECTS:util_buffered_io>
	$<TARGET_OBJECTS:util_async_impl>
	$<TARGET_OBJECTS:util_buffered_serial>
)
