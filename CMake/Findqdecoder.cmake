# - Try to Find libqdecoder
# Once done this will define
#  qdecoder_FOUND
#  qdecoder_INCLUDE_DIRS
#  qdecoder_LIBRARIES

# Look for the header file.
find_path(qdecoder_INCLUDE_DIR NAMES qdecoder.h)

# Look for the library.
find_library(qdecoder_LIBRARY NAMES qdecoder)

# Handle the QUIETLY and REQUIRED arguments and set qdecoder_FOUND to TRUE if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(qdecoder DEFAULT_MSG qdecoder_INCLUDE_DIR qdecoder_LIBRARY)

# Copy the results to the output variables.
if(qdecoder_FOUND)
  set(qdecoder_LIBRARIES ${qdecoder_LIBRARY})
  set(qdecoder_INCLUDE_DIRS ${qdecoder_INCLUDE_DIR})
endif()

mark_as_advanced(qdecoder_INCLUDE_DIR qdecoder_LIBRARY)

