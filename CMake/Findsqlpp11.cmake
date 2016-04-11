# Modified from Findcppformat.cmake which is Copyright Gonzalo Brito Gadeschi 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
# Find the sqlpp11 include directory
# The following variables are set if sqlpp11 is found.
#  sqlpp11_FOUND            - True when the sqlpp11 include directory is found.
#  sqlpp11_mysql_FOUND      - True when the sqlpp11 include directory contains the mysql directory
#  sqlpp11_sqlite3_FOUND    - True when the sqlpp11 include directory contains the sqlite3 directory
#  sqlpp11_postgresql_FOUND - True when the sqlpp11 include directory contians the postgresql directory
#  sqlpp11_odbc_FOUND       - True when the sqlpp11 include directory contians the odbc directory
#  sqlpp11_INCLUDE_DIR      - The path to where the sqlpp11 include files are.
# If sqlpp11 is not found, sqlpp11_FOUND is set to false.

if(NOT EXISTS "${sqlpp11_INCLUDE_DIR}")
	find_path(sqlpp11_INCLUDE_DIR
		NAMES sqlpp11/sqlpp11.h
		DOC "sqlpp11 library header files"
		)
endif()


if(EXISTS "${sqlpp11_INCLUDE_DIR}")
	include(FindPackageHandleStandardArgs)
	mark_as_advanced(sqlpp11_INCLUDE_DIR)
endif()


if(EXISTS "${sqlpp11_INCLUDE_DIR}")
	set(sqlpp11_FOUND 1)
	set(sqlpp11_INCLUDE_DIRS ${sqlpp11_INCLUDE_DIR})
	message("Found sqlpp11 in ${sqlpp11_INCLUDE_DIR}")
	if(EXISTS "${sqlpp11_INCLUDE_DIR}/sqlpp11/mysql")
		message("Found sqlpp11-connector-mysql")
		set(sqlpp11_mysql_FOUND 1)
	endif()
	if(EXISTS "${sqlpp11_INCLUDE_DIR}/sqlpp11/sqlite3")
		message("Found sqlpp11-connector-sqlite3")
		set(sqlpp11_sqlite3_FOUND 1)
	endif()
	if(EXISTS "${sqlpp11_INCLUDE_DIR}/sqlpp11/postgresql")
		message("Found sqlpp11-connector-postgresql")
		set(sqlpp11_postgresql_FOUND 1)
	endif()
	if(EXISTS "${sqlpp11_INCLUDE_DIR}/sqlpp11/odbc")
		message("Found sqlpp11-connector-odbc")
		set(sqlpp11_odbc_FOUND 1)
		find_library(sqlpp11_odbc_LIBRARY NAMES sqlpp-odbc)
		find_package(ODBC)
		set(sqlpp11_odbc_LIBRARIES
			${sqlpp11_odbc_LIBRARY}
			${ODBC_LIBRARY}
			${ODBC_EXTRA_LIBRARIES}
		)
		set(sqlpp11_odbc_INCLUDE_DIRS
			${sqlpp11_INCLUDE_DIRS}
			${ODBC_INCLUDE_DIRECTORIES}
		)
	endif()
else()
	set(sqlpp11_FOUND 0)
endif()
if(EXISTS "${sqlpp11_INCLUDE_DIR}/sqlpp11/date.h")
	file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/date")
	file(WRITE "${PROJECT_SOURCE_DIR}/date/date.h" "#include <sqlpp11/date.h>\n")
	set(sqlpp11_INCLUDE_DIRS ${sqlpp11_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/date")
	message("date.h found in ${sqlpp11_INCLUDE_DIR}/sqlpp11/date.h")
endif()
