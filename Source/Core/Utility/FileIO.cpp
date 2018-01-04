#include <Mist_Common\include\UtilityMacros.h>

#include <Utility\FileIO.h>

#include <cstdio>

MistNamespace

__declspec(dllexport) String ReadFile(const char* filePath)
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

MistNamespaceEnd
