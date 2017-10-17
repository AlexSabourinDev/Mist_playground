project( Game )

set ( SOURCE_FILES )
ListFiles( ${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES ) 

AddExecutable( Game "${SOURCE_FILES}" )
CopyDataToOutput( Game )

set( GAME_DEPS Platform)
target_link_libraries( Game PUBLIC ${GAME_DEPS} )