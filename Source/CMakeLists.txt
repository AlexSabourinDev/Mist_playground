cmake_minimum_required( VERSION 3.7.0 )

include (../CMake/Utilities.CMake)

set( MIST_INCLUDE_DIRS 
	 ${CMAKE_SOURCE_DIR}
	 ${CMAKE_SOURCE_DIR}/Core
	 ${CMAKE_SOURCE_DIR}/Modules
	 ${CMAKE_SOURCE_DIR}/3rdParty
	 ${CMAKE_SOURCE_DIR}/Tools
	 ${CMAKE_SOURCE_DIR}/3rdParty/assimp/include)
	 
set( MIST_OUTPUT_DIRECTORY 
	 ${CMAKE_BINARY_DIR}/../../Output )
message( "Setting Mist Output Directory to ${MIST_OUTPUT_DIRECTORY}" )
SetOutputDirectory( ${MIST_OUTPUT_DIRECTORY} )

set( MIST_DATA_OUTPUT 
	${CMAKE_BINARY_DIR}/../../Data )

# Include SDL2

set( SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2-2.0.6 )

find_package(SDL2 REQUIRED)
include (../CMake/3rdParty.CMake)

# Create the projects

project( All )

add_subdirectory( 3rdParty )
add_subdirectory( Core )
add_subdirectory( Modules )
add_subdirectory( Tools )
add_subdirectory( Game )