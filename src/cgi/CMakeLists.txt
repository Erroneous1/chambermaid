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

find_package(FCGI REQUIRED)
find_package(qdecoder REQUIRED)
find_package(jsoncpp REQUIRED)

set(CMAKE_EXECUTABLE_SUFFIX cgi)

add_executable( panel panel.cpp )

set(CGI_LINK
	${FCGI_LIBRARIES}
	${qdecoder_LIBRARIES}
	${jsoncpp_LIBRARIES}
	${src_LIBRARIES}
	src
)

target_link_libraries(panel ${CGI_LINK})

include_directories(
	${FCGI_INCLUDE_DIR}
	${qdecoder_INCLUDE_DIR}
	${jsoncpp_INCLUDE_DIR}
)

install(TARGETS panel RUNTIME DESTINATION cgi-bin)