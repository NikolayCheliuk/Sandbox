#
# CoinChange
#

# executables
add_executable(
  CoinChange
  source/main.cpp
)

target_include_directories(
  CoinChange
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
