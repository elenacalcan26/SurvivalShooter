#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#define NUM_TRIANGLES 40
#define TWO_PI 6.284f

namespace createObjects2D 
{
	Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateCircle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateHealthBar(const std::string& name, glm::vec3 leftBottomCorner, float health, glm::vec3 color, bool fill = false);
}
