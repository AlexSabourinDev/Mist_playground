
set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2_LIBRARIES "${CMAKE_BINARY_DIR}/../../Lib/SDL2.lib;${CMAKE_BINARY_DIR}/../../Lib/SDL2main.lib")
else ()
  set(SDL2_LIBRARIES "${CMAKE_BINARY_DIR}/../../Lib/SDL2.lib;${CMAKE_BINARY_DIR}/../../Lib/SDL2main.lib")
endif ()

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

# include("${CMAKE_CURRENT_LIST_DIR}/SDL2Targets.cmake")
