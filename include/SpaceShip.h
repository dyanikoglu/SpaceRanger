#pragma once
#include "Materials.h"
#include "Angel/Angel.h"
#include <vector>

class SpaceShip {
public:
	void draw(mat4 view, GLuint program, GLuint modelView_ptr);
	void move();
	SpaceShip(vec4 initPosition, GLfloat initHeading, GLfloat initLinearVel, GLfloat initAngularVel, GLuint lifespan);
	~SpaceShip();
	std::vector<vec4> points;
	std::vector<vec3> normals;
	vec4 position;
	vec4 FPCameraAttachPoint;
	vec4 TPCameraAttachPoint;
	GLfloat linearVelocity;
	GLfloat targetLinearVel;
	GLfloat targetAngularVel;
	GLfloat angularVelocity;
	GLfloat heading;
	GLuint lifespan;
	GLint isCrashed;
	GLint isHitByBeam;

	GLfloat crashR1X, crashR2X, crashR1Y, crashR2Y, crashR1Z, crashR2Z, crashR1Rot, crashR2Rot;

private:
	//************************************
	// Method:    lerp
	// FullName:  SpaceShip::lerp
	// Access:    private 
	// Returns:   GLfloat
	// Parameter: GLfloat a, Start from
	// Parameter: GLfloat b, Target
	// Parameter: GLfloat f, Delta
	// Linearly interpolates a to b with f as delta value. Required for smooth velocity transitions.
	//************************************
	inline GLfloat lerp(GLfloat a, GLfloat b, GLfloat f) { return (a * (1.0 - f)) + (b * f);}
	void calculateLighting();

	const long double TO_RADIANS = 3.141592653589793238L / 180.f;
	Material mat;
	vec4 ambient_product;
	vec4 diffuse_product;
	vec4 specular_product;
	GLuint bufferOffset;
	mat4 model;
};