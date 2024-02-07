#pragma once
#include <Bindu.h>

const BINDU::Vec2i		TILE_SIZE = { 32,16 };

inline std::string RelativeResourcePath(const char* filepath = nullptr)
{
	std::string ResourcePath(MAX_PATH, '\0');

	GetModuleFileNameA(nullptr, const_cast<LPSTR>(ResourcePath.data()), MAX_PATH);

	std::string delimeter = "BrickBreaker";

	size_t t = ResourcePath.find(delimeter);
	ResourcePath = ResourcePath.substr(0, t + delimeter.size() + 1);

	if(filepath)
	ResourcePath += filepath;

	return ResourcePath;
}

