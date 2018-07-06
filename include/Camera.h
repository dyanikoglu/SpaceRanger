#pragma once
#include "Angel/Angel.h"
#include "SpaceShip.h"

class Camera {
public:
	enum VIEW_MODE {
		FP = 0, // First Person
		TP = 1, // Third Person
		TOP = 2 // Top View
	};
	Camera();
	void attach(SpaceShip* player);
	void update();
	mat4 view;
	VIEW_MODE currMode; // Current camera mode
	VIEW_MODE prevMode; // Previously used camera mode
private:
	vec4 position;
	vec4 focal; // at
	vec4 up;
	SpaceShip* actor; // Attached actor
};