#pragma once
#include "Angel/Angel.h"
#include "Materials.h"
#include <vector>

class Planet {
public:
	enum type {
		STAR = 0,
		SATELLITE = 1,
		ASTEROID = 2,
		PLANET = 3,
		FAR_STAR = 4
	};
	vec4 center;
	GLfloat radius;
	Planet(GLuint sphereArrayOffset, GLuint sphereArraySize, GLuint ringArrayOffset, GLuint ringArraySize, vec4 center, type planetType);
	void draw(mat4 view, GLuint program, GLuint modelView_ptr);
	
private:
	void calculateLighting();

	Material planetMat;
	Material ringMat;
	GLuint sphereArrayOffset;
	GLuint sphereArraySize;
	GLuint ringArrayOffset;
	GLuint ringArraySize;
	GLint ringXRot, ringYRot;
	GLint hasRing;
	vec4 p_ambient_product;
	vec4 p_diffuse_product;
	vec4 p_specular_product;
	vec4 r_ambient_product;
	vec4 r_diffuse_product;
	vec4 r_specular_product;
	type planetType;
	mat4 model;
};