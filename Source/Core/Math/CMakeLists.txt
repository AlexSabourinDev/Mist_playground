project( Math )

set ( SOURCE_FILES )
ListFiles( ${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES ) 

AddLibrary( Math SHARED "${SOURCE_FILES}" )

set( MATH_DEPS cJson )
target_link_libraries( Math PUBLIC ${MATH_DEPS} )
