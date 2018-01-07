#include <Mist_Common\include\UtilityMacros.h>

#include <Utility\FileIO.h>

#include <cstdio>

MistNamespace

String ReadFile(const char* filePath)
{
	FILE* file = fopen(filePath, "r");
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	String fileString;
	Reserve(&fileString, fileSize + 1);
	fread(fileString.stringBuffer, fileSize, 1, file);
	
	fclose(file);

	return fileString;
}

void ReadFile(const char* filePath, uint8_t** data, size_t* dataSize)
{
	FILE* file = fopen(filePath, "rb");
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	*data = (uint8_t*)malloc(fileSize);
	*dataSize = fileSize;
	fread(*data, fileSize, 1, file);

	fclose(file);
}

void WriteFile(const char* filePath, uint8_t* data, size_t dataSize)
{
	FILE* file = fopen(filePath, "wb");
	fwrite(data, sizeof(uint8_t), dataSize, file);
	
	fclose(file);
}

MistNamespaceEnd
