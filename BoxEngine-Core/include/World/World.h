#pragma once

#include <vector>

class World
{
public:
	
	World();

	void readWorld();
	void generateTerrain();

private:
	std::vector<float> m_terrain;
};