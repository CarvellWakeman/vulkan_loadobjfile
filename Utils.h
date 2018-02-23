#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "./glm/gtx/transform.hpp"

#define RESTITUTION_CONSTANT 0.85f


float* Array3(float a, float b, float c);

// Math helper functions //
float Dot(glm::vec3 v1, glm::vec3 v2);

void Cross(glm::vec3 v1, glm::vec3 v2, glm::vec3 vout);

float Unit(glm::vec3 vin, glm::vec3 vout);
glm::vec3 Unit(glm::vec3 vin);

// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );
void HsvRgb(float hsv[3], float rgb[3]);

float Dist(glm::vec3 target, glm::vec3 position);
