#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <string>
#include <fstream>

MIST_NAMESPACE

// -API-

inline std::string ReadFile(const std::string& fileName);


// -Implementation-

inline std::string ReadFile(const std::string& fileName) {
	std::ifstream fileStream(fileName);
	std::string result;

	// Initialize string memory to file size
	fileStream.seekg(0, std::ios::end);
	result.reserve(fileStream.tellg);
	fileStream.seekg(0, std::ios::beg);

	result.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
}

MIST_NAMESPACE_END
