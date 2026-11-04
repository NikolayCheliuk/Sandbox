#
# Best Time to Buy and Sell Stock
#

# executables
add_executable(
  BestTimeToBuyAndSellStock
  source/main.cpp
)

target_include_directories(
  BestTimeToBuyAndSellStock
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
