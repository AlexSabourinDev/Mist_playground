project( Game )

set ( SOURCE_FILES )
ListFiles( ${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES ) 

AddExecutable( Game "${SOURCE_FILES}" )
