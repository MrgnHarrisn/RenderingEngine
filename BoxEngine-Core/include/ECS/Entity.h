#pragma once

#include <vector>
#include <memory>

#include "ECS/Component.h"

class Entity
{
public:
	std::vector<std::shared_ptr<Component>> components;
private:
};