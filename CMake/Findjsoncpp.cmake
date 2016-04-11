# - Try to Find libjsoncpp
# Once done this will define
#  jsoncpp_FOUND
#  jsoncpp_INCLUDE_DIRS
#  jsoncpp_LIBRARIES

# Look for the header file.
find_path(jsoncpp_INCLUDE_DIR NAMES json/json.h)

# Look for the library.
find_library(jsoncpp_LIBRARY NAMES jsoncpp)

# Handle the QUIETLY and REQUIRED arguments and set jsoncpp_FOUND to TRUE if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(jsoncpp DEFAULT_MSG jsoncpp_INCLUDE_DIR jsoncpp_LIBRARY)

# Copy the results to the output variables.
if(jsoncpp_FOUND)
  set(jsoncpp_LIBRARIES ${jsoncpp_LIBRARY})
  set(jsoncpp_INCLUDE_DIRS ${jsoncpp_INCLUDE_DIR})
endif()

mark_as_advanced(jsoncpp_INCLUDE_DIR jsoncpp_LIBRARY)

