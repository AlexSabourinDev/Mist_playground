project( RenderingPlayground )

set( SOURCE_FILES )
ListFilesRecursive( ${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES )

AddLibrary( RenderingPlayground STATIC "${SOURCE_FILES}" )

set( RENDERING_PLAYGROUND_DEPS cJson Utility Math Rendering )

target_link_libraries( RenderingPlayground PUBLIC ${RENDERING_PLAYGROUND_DEPS} )