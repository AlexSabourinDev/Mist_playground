project( Platform )

set( SOURCE_FILES )
ListFilesRecursive( ${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES )

AddLibrary( Platform STATIC "${SOURCE_FILES}" )

LinkSDL2( Platform )

set( PLATFORM_DEPS cJson Utility Math )
target_link_libraries( Platform PUBLIC ${PLATFORM_DEPS} )