project( Utility )

set ( SOURCE_FILES )
ListFiles( ${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES ) 

AddLibrary( Utility STATIC "${SOURCE_FILES}" )