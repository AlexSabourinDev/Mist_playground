
function( LinkSDL2 targetName )
	target_link_libraries( ${targetName} PUBLIC ${SDL2_LIBRARIES} )
	target_include_directories( ${targetName} PUBLIC ${SDL2_INCLUDE_DIRS} )
endfunction( LinkSDL2 )

function( LinkGLEW targetName )
	set( GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../3rdParty/glew-2.1.0/include" )
	target_include_directories( ${targetName} PUBLIC ${GLEW_INCLUDE_DIR} )
endfunction( LinkGLEW )