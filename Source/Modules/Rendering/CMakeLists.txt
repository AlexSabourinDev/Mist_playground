project( Rendering )

set( SOURCE_FILES )
ListFilesRecursive( ${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES )

AddLibrary( Rendering STATIC "${SOURCE_FILES}" )

set( RENDERING_DEPS cJson Utility Math )

find_package(OpenGL REQUIRED)
target_link_libraries( Rendering PUBLIC ${RENDERING_DEPS} ${OPENGL_LIBRARIES} "${CMAKE_SOURCE_DIR}/../3rdParty/glew-2.1.0/lib/Release/Win32/glew32.lib" )

LinkGLEW( Rendering )