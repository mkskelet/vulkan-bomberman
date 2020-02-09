#include "MapLoader.h"

#include "MapLoader.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using string = std::string;

MapLoader::MapLoader()
{
	path = "../examples/maps/";
	sp = true;
}

void MapLoader::RefreshMapLists()
{
	std::ifstream file;
	string spPath = path + "sp/mapList.txt";
	string mpPath = path + "mp/mapList.txt";

	spMaps.clear();
	file.open(spPath.c_str(), std::ios::in);
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			spMaps.push_back(line);
		}
		file.close();
	}

	mpMaps.clear();
	file.open(mpPath.c_str(), std::ios::in);
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			mpMaps.push_back(line);
		}
		file.close();
	}
}

std::vector<string>& MapLoader::GetMapList(bool singlePlayer)
{
	sp = singlePlayer;
	RefreshMapLists();
	return (singlePlayer ? spMaps : mpMaps);
}

Map MapLoader::LoadMap(string name)
{
	string fullPath = path + name;
	std::ifstream mapFile;
	Map m;
	std::vector<int> blocks;
	string params[5] = { "BP", "SP", "FP", "RP", "T" };
	int pl = 0;

	mapFile.open(fullPath.c_str(), std::ios::in);
	if (mapFile.is_open())
	{
		char block;
		string line;
		bool mapEnd = false;
		int x = -1, y = 0;				// variables for counting rows and collumns
		bool xCounted = false;

		while (mapFile.get(block) && !mapEnd)
		{
			if (!xCounted) x++;		// count x size

			switch (block)
			{
			case '#':
				blocks.push_back(BLOCK_SOLID);
				break;
			case '*':
				blocks.push_back(BLOCK_EXPLODABLE);
				break;
			case 'A':
				pl++;
				blocks.push_back(BLOCK_PLAYER);
				break;
			case ' ':
				blocks.push_back(BLOCK_SPACE);
				break;
			case '\n':
				xCounted = true;
				y++;		// count y size
				break;
			default:
				if (block >= '0' && block <= '9')
				{
					blocks.push_back(block - 48);
				}
				else
				{
					mapFile.unget();
					mapFile.unget();
					mapEnd = true;
				}
				break;
			}
		}

		m.SetName(name);
		m.SetX(x);
		m.SetY(y);
		m.SetMapBlocks(blocks);
		m.SetSinglePlayer(pl > 1 ? false : true);

		while (getline(mapFile, line, '='))
		{
			for (int i = 0; i < 5; i++)
			{
				if (line.compare(params[i]) == 0)
				{
					string n;			// number to load
					getline(mapFile, n);

					switch (i)
					{		// set map parameter based on its string representation
					case 0:
						m.SetBombPowerups(atoi(n.c_str()));
						break;
					case 1:
						m.SetSpeedPowerups(atoi(n.c_str()));
						break;
					case 2:
						m.SetFlamePowerups(atoi(n.c_str()));
						break;
					case 3:
						m.SetRemotePowerups(atoi(n.c_str()));
						break;
					case 4:
						m.SetTimeLimit(atoi(n.c_str()));
						break;
					default: break;
					}
				}
			}

		}
		mapFile.close();
	}

	return m;
}