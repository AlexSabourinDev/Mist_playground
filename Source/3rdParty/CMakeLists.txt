project( 3rdParty )

set( LISTED_3RD_PARTY_DIRECTORIES )
ListSubdirectories( ${CMAKE_CURRENT_SOURCE_DIR} LISTED_3RD_PARTY_DIRECTORIES )

# Add all of the 3rdParty source directories
foreach( directory ${LISTED_3RD_PARTY_DIRECTORIES} )

	if( NOT EXISTS "${directory}/CMakeLists.txt" ) 
		get_filename_component( ABSOLUTE_PATH ${directory} ABSOLUTE )

		set( SOURCE_FILES )
		ListFilesRecursive( ${ABSOLUTE_PATH} SOURCE_FILES )
	
		AddLibrary( ${directory} STATIC "${SOURCE_FILES}" )
	else()
		add_subdirectory( ${directory} )
	endif()
endforeach()