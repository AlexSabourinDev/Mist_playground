cmake_minimum_required( VERSION 3.3.2 )

include (../CMake/Utilities.CMake)

set( MIST_INCLUDE_DIRS 
	 ${CMAKE_SOURCE_DIR}
	 ${CMAKE_SOURCE_DIR}/Core
	 ${CMAKE_SOURCE_DIR}/Modules
	 ${CMAKE_SOURCE_DIR}/3rdParty )
	 
set( MIST_OUTPUT_DIRECTORY 
	 ${CMAKE_BINARY_DIR}/../../Output )
message( "Setting Mist Output Directory to ${MIST_OUTPUT_DIRECTORY}" )
SetOutputDirectory( ${MIST_OUTPUT_DIRECTORY} )

project( All )

add_subdirectory( Modules )
add_subdirectory( Core )
add_subdirectory( 3rdParty )