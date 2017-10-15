
function( LinkSDL2 targetName )
	target_link_libraries( ${targetName} PUBLIC ${SDL2_LIBRARIES} )
	target_include_directories( ${targetName} PUBLIC ${SDL2_INCLUDE_DIRS} )
	
	set( COPY_SOURCE ${CMAKE_SOURCE_DIR}/../Data )
	set( DEST_SOURCE $<TARGET_FILE_DIR:${targetName}> )
	add_custom_command( TARGET ${targetName} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${COPY_SOURCE}" "${DEST_SOURCE}" )
endfunction( LinkSDL2 )