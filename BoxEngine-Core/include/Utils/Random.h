#pragma once

#include <random>

static float randomF(float min, float max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dist(min, max);

	return dist(rng);
}