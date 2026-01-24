#include "World/World.h"

#include <corecrt_malloc.h>
#include <random>
#include <iostream>

// built-in functions
#include "Utils/Random.h"

World::World()
{
	m_terrain.resize(64);
}

// use FileSys.h to read in a .berp file
void World::readWorld()
{

}

// need to generate a mesh from this or something
void World::generateTerrain()
{
	// generate terrain here (no smoothing just yet)
	for (size_t i = 0; i < 8; ++i) {
		for (size_t j = 0; j < 8; ++j) {
			m_terrain[i * 8 + j] = randomF(0.f, 1.f);
			std::cout << m_terrain[i * 8 + j] << ", ";
		}
		std::cout << std::endl;
	}
}