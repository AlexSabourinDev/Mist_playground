project( Systems )

set ( SOURCE_FILES )
ListFiles( ${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES ) 

AddLibrary( Systems SHARED "${SOURCE_FILES}" )
