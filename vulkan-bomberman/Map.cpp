#include "Map.h"

const int Map::operator[](int index) const
{
	if (index < mapBlocks.size() && index >= 0)
	{
		return mapBlocks[index];
	}
	else return -1;
}