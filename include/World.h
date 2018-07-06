#pragma once
#include "SpaceShip.h"
#include "Camera.h"
#include "Angel/Angel.h"
#include "Planet.h"
#include "Beam.h"
#include "PlanetBuilder.h"
#include "Surface.h"
#include "FMOD/fmod.hpp"

class World {
public:
	enum State {
		RUN = 0, // Running...
		PAUSE = 1, // Paused...
		ONE_FRAME = 2 // Advance by one frame...
	};

	State worldState; // State of world

	void draw();
	void updateWorld();
	void playerShoot();

	Camera* camera; // Pointer to camera
	SpaceShip* player; // Pointer to current selected player ship
	Surface* surface; // Surface object
	GLint playerIndex; // Current player index in playerShips vector
	std::vector<SpaceShip*> playerShips; // Stores 2 ships of player
	std::vector<Planet*> planets; // All planets in world
	std::vector<SpaceShip*> NPCShips; // All NPC ships in world
	std::vector<Beam*> beams; // All beams in world
	World(GLuint program); // Constructor
	GLuint frameCounter;

private:
	void buildGalaxy(GLuint planetBufOffset, GLuint planetBufSize, GLuint ringBufOffset, GLuint ringBufSize, GLuint sunBufOffset, GLuint sunBufSize);
	void engineLoudness();
	void initSounds();
	void spawnNPCShip();
	void checkNPCShips();
	void checkCollision();
	void NPCShoot();
	void checkBeams();

	// Sound effect pointers
	FMOD::System *FMODSystem;
	FMOD::Channel *ambienceChannel = 0;
	FMOD::Channel *engineChannel = 0;
	FMOD::Channel *effectChannel = 0;
	FMOD::Channel *blasterChannel = 0;
	FMOD::Channel *explosionChannel = 0;
	FMOD::Sound *ambience;
	FMOD::Sound *engine_loop;
	FMOD::Sound *engine_startup;
	FMOD::Sound *blaster;
	FMOD::Sound *explosion;

	GLuint ModelView_ptr; // Pointer for modelview uniform
	GLuint Projection_ptr; // Pointer for projection uniform
	mat4 view; // General view matrix
	mat4 projection; // General projection matrix
	GLuint program_ptr; // Pointer for program object
	GLuint beamBufOffset; // Buffer offset for beams
	Beam *dummyBeam; // Dummy beam object for getting points into array buffer object
};