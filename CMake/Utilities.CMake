macro ( SetOutputDirectory directory )
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${directory}/lib")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${directory}/lib")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${directory}/bin")
	
	# Assure that we don't get the configuration postfix to the build path
	foreach( cfg ${CMAKE_CONFIGURATION_TYPES} )
		string( TOUPPER ${cfg} OUTPUTCONFIG )
		set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${directory}/${cfg}/lib" )
		set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${directory}/${cfg}/lib" )
		set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${directory}/${cfg}/bin" )
	endforeach()
	
endmacro ( SetOutputDirectory )


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
	file(GLOB fileList ${currentDir}
		"*.hpp"
		"*.cpp"
		"*.h"
		"*.c")
	set( ${result} ${fileList} PARENT_SCOPE )
endfunction( ListFiles )


function( ListFilesRecursive currentDir result )
	file(GLOB_RECURSE fileList ${currentDir}
		"*.hpp"
		"*.cpp"
		"*.h"
		"*.c")
	set( ${result} ${fileList} PARENT_SCOPE)
endfunction( ListFilesRecursive )


function( AddLibrary targetName linkage sourceFiles )
	add_library( ${targetName} ${linkage} ${sourceFiles} )
	set_target_properties( ${targetName} PROPERTIES LINKER_LANGUAGE CXX )
	target_include_directories( ${targetName} PUBLIC ${MIST_INCLUDE_DIRS} )
endfunction( AddLibrary )

