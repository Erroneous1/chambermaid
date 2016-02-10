cmake_minimum_required(VERSION 2.6)

project(brewpipp)

# set(CMAKE_MODULE_PATH 
# 	${CMAKE_MODULE_PATH}
# 	${PROJECT_SOURCE_DIR}
# 	"/usr/share/cmake-3.4/Modules"
# 	)
message("Prefix Path: ${CMAKE_PREFIX_PATH}")

find_package(Boost 1.60 COMPONENTS system filesystem thread REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(cppformat REQUIRED)

include_directories(cppformat_INCLUDE_DIR)
include_directories(OPENSSL_INCLUDE_DIR)

set(OBJECTS
	"${PROJECT_SOURCE_DIR}/src/DataType.cpp"
	)
set(LIBS
	${Boost_LIBRARIES}
	${OPENSSL_LIBRARIES}
	)
	
if(UNIX)
	set(LIBS ${LIBS} pthread)
endif(UNIX)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(ODBC REQUIRED)

if(ODBC_FOUND)
	include_directories(ODBC_INCLUDE_DIRECTORIES)
	set(LIBS ${LIBS} ${ODBC_LIBRARIES})
	set(OBJECTS 
		${OBJECTS}
		"${PROJECT_SOURCE_DIR}/src/Odbc.cpp"
		"${PROJECT_SOURCE_DIR}/src/SecureConnection.cpp"
		"${PROJECT_SOURCE_DIR}/src/SqlError.cpp"
		)
	set(BREWPIPP_ODBC 1)
endif(ODBC_FOUND)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_TWEAK 1)

set(SAVE_VERSION 0)

configure_file(
	brewpipp_config.h.in
	"${PROJECT_SOURCE_DIR}/include/brewpipp_config.h"
	)
	
message("libs: ${LIBS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

add_executable(brewpipp main.cpp ${OBJECTS})

target_compile_features(brewpipp PRIVATE cxx_range_for)

target_link_libraries(brewpipp ${LIBS})

include_directories("${PROJECT_SOURCE_DIR}/include")

install(TARGETS brewpipp RUNTIME DESTINATION bin)
