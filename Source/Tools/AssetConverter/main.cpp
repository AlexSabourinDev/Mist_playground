#include <Mist_common\include\UtilityMacros.h>

#include <Rendering/Renderer.h>

#include <Core/Utility/FileIO.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cstring>
#include <cstdint>
#include <cstdlib>

#include <vector>

struct BinaryAsset
{
	uint8_t* data;
	size_t dataSize;
};

BinaryAsset ConvertFBX(const char* assetPath)
{
	const aiScene* scene = aiImportFile(assetPath, aiProcessPreset_TargetRealtime_MaxQuality);
	MistAssert(scene != nullptr);

	std::vector<Mist::MeshVertex> vertices;

	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
	{
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
		{
			const aiFace& face = mesh->mFaces[faceIndex];
			for (unsigned int vertIndex = 0; vertIndex < face.mNumIndices; vertIndex++)
			{
				aiVector3D position = mesh->mVertices[face.mIndices[vertIndex]];
				vertices.push_back({ position.x, position.y, position.z });
			}
		}
	}

	BinaryAsset asset;

	asset.data = (uint8_t*)malloc(sizeof(Mist::MeshVertex) * vertices.size());
	memcpy(asset.data, vertices.data(), sizeof(Mist::MeshVertex) * vertices.size());
	asset.dataSize = sizeof(Mist::MeshVertex) * vertices.size();

	return asset;
}

int main(const int argc, const char** argv)
{
	MistAssert(argc == 3);

	const char* assetPath = argv[1];
	
	BinaryAsset asset;
	memset(&asset, 0, sizeof(BinaryAsset));
	if (strstr(assetPath, "fbx") != nullptr)
	{
		asset = ConvertFBX(assetPath);
	}

	if (asset.data != nullptr)
	{
		const char* writePath = argv[2];
		Mist::WriteFile(writePath, asset.data, asset.dataSize);
		free(asset.data);
	}
	return 0;
}
