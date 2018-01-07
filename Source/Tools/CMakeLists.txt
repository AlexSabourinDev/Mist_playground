project( AllTools )

set ( SUBDIRECTORIES )
ListSubdirectories( ${CMAKE_CURRENT_SOURCE_DIR} SUBDIRECTORIES )

foreach( directory ${SUBDIRECTORIES} )
	add_subdirectory( ${directory} )
endforeach()