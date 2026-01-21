#pragma once

#include <vector>
#include <memory>
#include "ECS/Entity.h"

class Scene
{
public:
	// this should be world chunks, NOT entities
	std::vector<std::shared_ptr<Entity>> entities;

	Scene();
	void update();
	void render();
private:
};