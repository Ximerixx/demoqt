# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/demoqt_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/demoqt_autogen.dir/ParseCache.txt"
  "demoqt_autogen"
  )
endif()
