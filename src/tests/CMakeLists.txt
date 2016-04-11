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

message("src_LIBRARIES: ${src_LIBRARIES}")

add_library(DeathHandler OBJECT DeathHandler/death_handler.cc)

add_executable(test_ip ip.cpp $<TARGET_OBJECTS:DeathHandler>)
target_link_libraries(test_ip ${src_LIBRARIES} src util dl)
add_test(ip test_ip)

add_executable(test_tty tty.cpp $<TARGET_OBJECTS:DeathHandler>)
target_link_libraries(test_tty ${src_LIBRARIES} src util dl)
add_test(tty test_tty)

add_executable(test_serial serial.cpp $<TARGET_OBJECTS:DeathHandler>)
target_link_libraries(test_serial ${src_LIBRARIES} src util dl)
add_test(serial test_serial "/dev/ACM0")