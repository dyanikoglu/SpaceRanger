#pragma once
#include "Angel/Angel.h"
#include <vector>
#include "SpaceShip.h"

class Surface {
private:
	struct LightingParams {
		vec4 light_ambient;
		vec4 light_diffuse;
		vec4 light_specular;
		vec4 material_ambient;
		vec4 material_diffuse;
		vec4 material_specular;
		GLfloat material_shininess;
	} params;

	vec4 ambient_product;
	vec4 diffuse_product;
	vec4 specular_product;

	// Model matrices of each square
	std::vector<mat4> models;

	void calculateLighting();
	void buildSurfaceMatrix();
	
	GLuint arrayOffset;
	GLuint arraySize;

	// Bounds for surface matrix
	GLint xBound1, xBound2;
	GLint yBound1, yBound2;

	GLint squareCountX, squareCountY, squareSize;

	// Pointer to current player
	SpaceShip* actor;

public:
	Surface(GLuint arrayOffset, SpaceShip* player, GLint squareCountX, GLint squareCountY, GLint squareSize);
	void checkBounds();
	void draw(mat4 view, GLuint program, GLuint modelView_ptr);
	void changeActor(SpaceShip* player);
	std::vector<vec4> points;
	std::vector<vec3> normals;
};