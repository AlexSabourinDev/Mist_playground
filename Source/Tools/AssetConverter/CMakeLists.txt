project( AssetConverter )

set ( SOURCE_FILES )
ListFiles( ${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES ) 

AddExecutable( AssetConverter "${SOURCE_FILES}" )

set( ASSET_CONVERTER_DEPS Utility Rendering Systems Math assimp )
target_link_libraries( AssetConverter PUBLIC ${ASSET_CONVERTER_DEPS} )