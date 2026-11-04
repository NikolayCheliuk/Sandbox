#
# Magic Spells
#

# executables
add_executable(
  MagicSpells
  source/main.cpp
)

target_include_directories(
  MagicSpells
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
