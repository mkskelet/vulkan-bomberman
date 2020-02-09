#pragma once

#include "Map.h"
#include <string>
#include <vector>

class MapLoader
{
private:
	bool sp;
	std::string path;
	Map map;							// potentially unused
	std::vector<std::string> spMaps;
	std::vector<std::string> mpMaps;
	void RefreshMapLists();
public:
	MapLoader();
	std::vector<std::string>& GetMapList(bool singlePlayer);		///< Returns list of single player / multi player maps depending on provided argument.
	Map LoadMap(std::string name);							///< Loads and returns map with given name.
};