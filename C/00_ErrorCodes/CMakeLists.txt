#
# Error Codes
#

#
# Error Codes Library
#

add_library(
  ErrorCodesLib INTERFACE
)

target_include_directories(
  ErrorCodesLib
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
