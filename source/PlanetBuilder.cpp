#include "PlanetBuilder.h"

//************************************
// Method:    triangle
// FullName:  PlanetBuilder::triangle
// Access:    private 
// Returns:   void
// Parameter: const vec4 & a
// Parameter: const vec4 & b
// Parameter: const vec4 & c
// Creates a triangle with given parameters as a, b, c
//************************************
void PlanetBuilder::triangle(const vec4& a, const vec4& b, const vec4& c)
{
	vec3  normal;
	if (this->flipNormals == 1) {
		// Inner light is not visible on outside of planet (standard planet)
		normal = normalize(cross(c - b, b - a));
	}
	else {
		// Inner light is visible on outside of planet (sun)
		normal = normalize(cross(b - a, c - b));
	}

	normals.emplace_back(normal);
	normals.emplace_back(normal);
	normals.emplace_back(normal);

	points.emplace_back(a);
	points.emplace_back(b);
	points.emplace_back(c);
}

//************************************
// Method:    unit
// FullName:  PlanetBuilder::unit
// Access:    private 
// Returns:   Angel::vec4
// Parameter: const vec4 & p
// Calculates unit vector of parameter vector
//************************************
vec4 PlanetBuilder::unit(const vec4& p)
{
	float len = p.x*p.x + p.y*p.y + p.z*p.z;

	vec4 t;
	if (len > DivideByZeroTolerance) {
		t = p / sqrt(len);
		t.w = 1.0;
	}

	return t;
}

//************************************
// Method:    divide_triangle
// FullName:  PlanetBuilder::divide_triangle
// Access:    private 
// Returns:   void
// Parameter: const vec4 & a
// Parameter: const vec4 & b
// Parameter: const vec4 & c
// Parameter: int count
// Divides triangles recursively "count" times
//************************************
void PlanetBuilder::divide_triangle(const vec4& a, const vec4& b,
	const vec4& c, int count)
{
	if (count > 0) {
		vec4 v1 = unit(a + b);
		vec4 v2 = unit(a + c);
		vec4 v3 = unit(b + c);
		divide_triangle(a, v1, v2, count - 1);
		divide_triangle(c, v2, v3, count - 1);
		divide_triangle(b, v3, v1, count - 1);
		divide_triangle(v1, v3, v2, count - 1);
	}
	else {
		triangle(a, b, c);
	}
}

//************************************
// Method:    PlanetBuilder
// FullName:  PlanetBuilder::PlanetBuilder
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: GLint smoothness
// Parameter: GLint flipNormals, Flipping normals is required for light source spheres. Their surfaces will be lit by this way
// since the light source position is inside this sphere.
//************************************
PlanetBuilder::PlanetBuilder(GLint smoothness, GLint flipNormals) {
	this->flipNormals = flipNormals;

	vec4 v[4] = {
		vec4(0.0, 0.0, 1.0, 1.0),
		vec4(0.0, 0.942809, -0.333333, 1.0),
		vec4(-0.816497, -0.471405, -0.333333, 1.0),
		vec4(0.816497, -0.471405, -0.333333, 1.0)
	};

	divide_triangle(v[0], v[1], v[2], smoothness);
	divide_triangle(v[3], v[2], v[1], smoothness);
	divide_triangle(v[0], v[3], v[1], smoothness);
	divide_triangle(v[0], v[2], v[3], smoothness);
}