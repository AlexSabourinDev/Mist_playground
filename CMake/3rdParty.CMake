
function( LinkSDL2 targetName )
	target_link_libraries( ${targetName} PUBLIC ${SDL2_LIBRARIES} )
	target_include_directories( ${targetName} PUBLIC ${SDL2_INCLUDE_DIRS} )
endfunction( LinkSDL2 )