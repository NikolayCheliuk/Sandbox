#
# Most Frequent Element / Top K Frequent Elements
#

# executables
add_executable(
  MostFrequentElement
  source/main.cpp
)

target_include_directories(
  MostFrequentElement
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
