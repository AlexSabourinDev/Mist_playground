macro ( SetOutputDirectory directory )
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${directory}/Lib")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${directory}/Lib")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${directory}/Bin")
	
	# Assure that we don't get the configuration postfix to the build path
	foreach( cfg ${CMAKE_CONFIGURATION_TYPES} )
		string( TOUPPER ${cfg} OUTPUTCONFIG )
		set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${directory}/${cfg}/Bin" )
		set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${directory}/${cfg}/Lib" )
		set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${directory}/${cfg}/Lib" )
	endforeach()
	
endmacro ( SetOutputDirectory )

function ( SetTargetOutputDirectory targetName directory )
	
	# Assure that we don't get the configuration postfix to the build path
	foreach( cfg ${CMAKE_CONFIGURATION_TYPES} )
		string( TOUPPER ${cfg} OUTPUTCONFIG )
		set_target_properties( ${targetName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${directory}" )
	endforeach()
	
endfunction ( SetTargetOutputDirectory )


function( ListSubdirectories currentDir result )
  file(GLOB children RELATIVE ${currentDir} ${currentDir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${currentDir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist} PARENT_SCOPE)
endfunction( ListSubdirectories )


function( ListFiles currentDir result )
	file(GLOB fileList
		"${currentDir}/*.hpp"
		"${currentDir}/*.cpp"
		"${currentDir}/*.h"
		"${currentDir}/*.c")
	set( ${result} ${fileList} PARENT_SCOPE )
endfunction( ListFiles )


function( ListFilesRecursive currentDir result )
	file(GLOB_RECURSE fileList
		"${currentDir}/*.hpp"
		"${currentDir}/*.cpp"
		"${currentDir}/*.h"
		"${currentDir}/*.c")
	set( ${result} ${fileList} PARENT_SCOPE)
endfunction( ListFilesRecursive )


function( AddLibrary targetName linkage sourceFiles )
	add_library( ${targetName} ${linkage} ${sourceFiles} )
	set_target_properties( ${targetName} PROPERTIES LINKER_LANGUAGE CXX )
	target_compile_definitions( ${targetName} PRIVATE _CRT_SECURE_NO_WARNINGS )
	target_include_directories( ${targetName} PUBLIC ${MIST_INCLUDE_DIRS} )
	
	if( "${linkage}" EQUAL "SHARED" )
		set_target_properties(${targetName} PROPERTIES COMPILE_DEFINITIONS "DLL_EXPORT")
	endif()
endfunction( AddLibrary )


function( AddExecutable targetName sourceFiles )
	add_executable( ${targetName} ${sourceFiles} )
	set_target_properties( ${targetName} PROPERTIES LINKER_LANGUAGE CXX )
	target_compile_definitions( ${targetName} PRIVATE _CRT_SECURE_NO_WARNINGS _CRTDBG_MAP_ALLOC )
	target_include_directories( ${targetName} PUBLIC ${MIST_INCLUDE_DIRS} )
endfunction( AddExecutable )


function( CopyDataToOutput targetName)
	set( COPY_SOURCE ${CMAKE_SOURCE_DIR}/../Data )
	set( DEST_SOURCE $<TARGET_FILE_DIR:${targetName}> )
	add_custom_command( TARGET ${targetName} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${COPY_SOURCE}" "${DEST_SOURCE}" )
endfunction( CopyDataToOutput )