project( Platform )

set( SOURCE_FILES )
ListFilesRecursive( ${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES )

AddLibrary( Platform SHARED "${SOURCE_FILES}" )

LinkSDL2( Platform )

set( PLATFORM_DEPS Systems cJson Utility Math )
target_link_libraries( Platform PUBLIC ${PLATFORM_DEPS} )

SetTargetOutputDirectory( Platform ${MIST_DATA_OUTPUT}/Plugins )