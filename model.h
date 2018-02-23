#ifndef MODEL_H
#define MODEL_H

#define GLM_ENABLE_EXPERIMENTAL
#include "./glm/gtx/transform.hpp"
#include "./vertex.h"
#include <vector>

struct model
{
	std::vector<struct vertex> vertices;
	std::vector<int> indices;
};
#endif
