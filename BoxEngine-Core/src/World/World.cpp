#include "World/World.h"

#include <corecrt_malloc.h>
#include <random>
#include <iostream>
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
	// setup random generation - TODO: Make a seperate class/function for handeling random number gen
	

	// generate terrain here (no smoothing just yet)
	for (size_t i = 0; i < 8; ++i) {
		for (size_t j = 0; j < 8; ++j) {
			m_terrain[i * 8 + j] = randomF(0.f, 1.f);
			std::cout << m_terrain[i * 8 + j] << ", ";
		}
		std::cout << std::endl;
	}
}
