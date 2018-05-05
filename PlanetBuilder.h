#pragma once
#include "Angel/Angel.h"
#include <vector>

class PlanetBuilder {
public:
	std::vector<vec4> points;
	std::vector<vec3> normals;
	PlanetBuilder(GLint count, GLint flipNormals);
	
private:
	void triangle(const vec4 & a, const vec4 & b, const vec4 & c);
	vec4 unit(const vec4 & p);
	void divide_triangle(const vec4 & a, const vec4 & b, const vec4 & c, int count);
	GLint flipNormals;
};