#include "World.h"

World::World(GLuint program) {
	program_ptr = program;
	GLuint offset = 0;

	frameCounter = 0;

	worldState = State::RUN;

	playerShips.emplace_back(new SpaceShip(vec4(-1500, 0, 0, 1), 0, 0, 0, -1));
	playerShips.emplace_back(new SpaceShip(vec4(-2000, 0, 0, 1), 0, 0, 3, -1));
	playerIndex = 0;
	player = playerShips[playerIndex];

	camera = new Camera();
	camera->attach(player);
	
	PlanetBuilder* planetBuilder = new PlanetBuilder(5, 0);
	PlanetBuilder* starBuilder = new PlanetBuilder(5, 1); // Flip normals for stars

	buildGalaxy(player->points.size(), planetBuilder->points.size(), 0, player->points.size(), player->points.size() + planetBuilder->points.size(), starBuilder->points.size());
	
	surface = new Surface(player->points.size() + planetBuilder->points.size() + starBuilder->points.size(), player, 30, 30, 300);

	beamBufOffset = player->points.size() + planetBuilder->points.size() + starBuilder->points.size() + surface->points.size();
	dummyBeam = new Beam(beamBufOffset, vec4(-1500,0,0,1), 0, -1, player);

	offset += sizeof(vec4) * player->points.size() + player->normals.size() * sizeof(vec3);
	offset += planetBuilder->points.size() * sizeof(vec4) + starBuilder->points.size() * sizeof(vec4);
	offset += planetBuilder->normals.size() * sizeof(vec3) + starBuilder->normals.size() * sizeof(vec3);
	offset += surface->points.size() * sizeof(vec4) + surface->normals.size() * sizeof(vec3);
	offset += dummyBeam->points.size() * sizeof(vec4) + dummyBeam->normals.size() * sizeof(vec3);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, offset, NULL, GL_STATIC_DRAW);

	GLuint tempOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, tempOffset, sizeof(vec4) * player->points.size(), player->points.front());
	tempOffset += sizeof(vec4) * player->points.size();

	glBufferSubData(GL_ARRAY_BUFFER, tempOffset, sizeof(vec4) * planetBuilder->points.size(), planetBuilder->points.front());
	tempOffset += sizeof(vec4) * planetBuilder->points.size();

	glBufferSubData(GL_ARRAY_BUFFER, tempOffset, sizeof(vec4) * starBuilder->points.size(), starBuilder->points.front());
	tempOffset += sizeof(vec4) * starBuilder->points.size();

	glBufferSubData(GL_ARRAY_BUFFER, tempOffset, sizeof(vec4) * surface->points.size(), surface->points.front());
	tempOffset += sizeof(vec4) * surface->points.size();

	glBufferSubData(GL_ARRAY_BUFFER, tempOffset, sizeof(vec4) * dummyBeam->points.size(), dummyBeam->points.front());
	tempOffset += sizeof(vec4) * dummyBeam->points.size();

	GLuint normalOffset = tempOffset;

	glBufferSubData(GL_ARRAY_BUFFER, tempOffset, sizeof(vec3) * player->normals.size(), player->normals.front());
	tempOffset += sizeof(vec3) * player->normals.size();

	glBufferSubData(GL_ARRAY_BUFFER, tempOffset, sizeof(vec3) * planetBuilder->normals.size(), planetBuilder->normals.front());
	tempOffset += sizeof(vec3) * planetBuilder->normals.size();

	glBufferSubData(GL_ARRAY_BUFFER, tempOffset, sizeof(vec3) * starBuilder->normals.size(), starBuilder->normals.front());
	tempOffset += sizeof(vec3) * starBuilder->normals.size();

	glBufferSubData(GL_ARRAY_BUFFER, tempOffset, sizeof(vec3) * surface->normals.size(), surface->normals.front());
	tempOffset += sizeof(vec3) * surface->normals.size();

	glBufferSubData(GL_ARRAY_BUFFER, tempOffset, sizeof(vec3) * dummyBeam->normals.size(), dummyBeam->normals.front());
	tempOffset += sizeof(vec3) * surface->normals.size();

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program_ptr, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program_ptr, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(normalOffset));

	vec4 light_position(0.0, 0.0, 0.0, 1.0);

	glUniform4fv(glGetUniformLocation(program_ptr, "LightPosition"), 1, light_position);

	ModelView_ptr = glGetUniformLocation(program_ptr, "ModelView");
	Projection_ptr = glGetUniformLocation(program_ptr, "Projection");

	projection = Perspective(60, 1, 0.1, 10000);
	glUniformMatrix4fv(Projection_ptr, 1, GL_TRUE, projection);

	initSounds();

	FMODSystem->playSound(ambience, 0, false, &ambienceChannel);
	ambienceChannel->setVolume(0.25f);

	FMODSystem->playSound(engine_loop, 0, false, &engineChannel);
	engineChannel->setVolume(0.f);
}



//************************************
// Method:    engineLoudness
// FullName:  World::engineLoudness
// Access:    private 
// Returns:   void
// Sets loudness of engine with amount of linear velocity
//************************************
void World::engineLoudness() {
	GLfloat loudness = abs(this->player->linearVelocity) / 50.f;
	if (loudness > 1.f) {
		loudness = 1.f;
	}
	engineChannel->setVolume(loudness);
}


//************************************
// Method:    initSounds
// FullName:  World::initSounds
// Access:    private 
// Returns:   void
// Initializes sound pointers with FMOD
//************************************
void World::initSounds() {
	FMOD::System_Create(&FMODSystem);
	FMODSystem->init(32, FMOD_INIT_NORMAL, 0);
	FMODSystem->createStream("media/ambience.wav", FMOD_DEFAULT, 0, &ambience);
	ambience->setMode(FMOD_LOOP_NORMAL);
	FMODSystem->createSound("media/engine_loop.wav", FMOD_DEFAULT, 0, &engine_loop);
	engine_loop->setMode(FMOD_LOOP_NORMAL);
	FMODSystem->createSound("media/engine_startup.wav", FMOD_DEFAULT, 0, &engine_startup);
	FMODSystem->createSound("media/blaster.wav", FMOD_DEFAULT, 0, &blaster);
	FMODSystem->createSound("media/explosion.wav", FMOD_DEFAULT, 0, &explosion);
}


//************************************
// Method:    buildGalaxy
// FullName:  World::buildGalaxy
// Access:    private 
// Returns:   void
// Parameter: GLuint planetBufOffset
// Parameter: GLuint planetBufSize
// Parameter: GLuint sunBufOffset
// Parameter: GLuint sunBufSize
// Builds a small galaxy with spiral distribution, places a big star on center, and additionally puts small stars at distant places to center.
//************************************
void World::buildGalaxy(GLuint planetBufOffset, GLuint planetBufSize, GLuint ringBufOffset, GLuint ringBufSize, GLuint sunBufOffset, GLuint sunBufSize) {
	// Create a sun in center of spiral(center of galaxy)
	planets.emplace_back(new Planet(sunBufOffset, sunBufSize, ringBufOffset, ringBufSize, vec4(0, 0, 0, 1), Planet::STAR));

	// Distribute planets on a spiral, add some displacement to their coordinates
	GLfloat x, y;
	GLfloat a = 0.8, b = 0.2;
	GLfloat theta = 0;
	for (int i = 1600; i < 3000; i=i+25) {
		GLint displacement = (rand() % 800) - 400;
		theta = 0.025*i;
		Planet *p = new Planet(planetBufOffset, planetBufSize, ringBufOffset, ringBufSize, vec4(a*pow(2.718281, (double)b*theta)*cos(theta), a*pow(2.718281, (double)b*theta)*sin(theta), displacement, 1), Planet::PLANET);
		planets.push_back(p);
		Planet *p2 = new Planet(planetBufOffset, planetBufSize, ringBufOffset, ringBufSize, vec4((a-0.4)*pow(2.718281, (double)b*theta)*cos(theta), (a-0.4)*pow(2.718281, (double)b*theta)*sin(theta), displacement, 1), Planet::PLANET);
		planets.push_back(p2);
	}

	// Place starts to distant places
	PlanetBuilder* pb = new PlanetBuilder(3, 1);
	for (int i = 0; i < pb->points.size(); i+=5) {
		planets.push_back(new Planet(sunBufOffset, sunBufSize, ringBufOffset, ringBufSize, Scale(7500) * pb->points[i], Planet::FAR_STAR));
	}
}

// Spawn random ships in random places in world
void World::spawnNPCShip() {
	NPCShips.emplace_back(new SpaceShip(vec4((rand() % 2500) - 1250, (rand() % 2500) - 1250, 0, 1), rand() % 360, 2, (rand() % 10) / 10.f, frameCounter + 1000));
}

// Check lifespans of ships in world, destroy which passes it's lifespan
void World::checkNPCShips() {
	for (int i = 0; i < NPCShips.size(); i++) {
		if (NPCShips[i]->lifespan < frameCounter) {
			delete NPCShips[i];
			NPCShips.erase(NPCShips.begin() + i);
		}
	}
}

// Check lifespans of beams in world, destroy which passes it's lifespan
void World::checkBeams() {
	for (int i = 0; i < beams.size(); i++) {
		if (beams[i]->lifespan < frameCounter) {
			delete beams[i];
			beams.erase(beams.begin() + i);
		}
	}
}

//************************************
// Method:    updateWorld
// FullName:  World::updateWorld
// Access:    public 
// Returns:   void
// Update components of world in each frame
//************************************
void World::updateWorld() {
	FMODSystem->update();

	frameCounter++;

	// Check lifespans
	checkNPCShips();
	checkBeams();

	// Spawn random ships in world
	if (frameCounter % 25 == 0) {
		spawnNPCShip();
	}

	// Shoot with some random ships
	if (frameCounter % 10 == 0 && NPCShips.size() > 1) {
		NPCShoot();
	}

	// Update engine volume
	engineLoudness();

	// Move objects in world
	for (auto & ship : playerShips) { ship->move(); }
	for (auto & ship : NPCShips) { ship->move(); }
	for (auto & beam : beams) { beam->move(); }

	// Refresh surface
	surface->checkBounds();

	// Update camera components
	camera->update();

	// Check collisions
	checkCollision();
}

void World::checkCollision() {
	for (auto & planet : planets) {
		if (!player->isCrashed && length(planet->center - player->position) < planet->radius + 2) {
			player->isCrashed = 1;
			FMODSystem->playSound(explosion, 0, false, &explosionChannel);
		}
	}

	for (auto & npc : NPCShips) {
		if (!npc->isCrashed) {
			// Check player - npc collision
			if (length(npc->position - player->position) < 4) {
				player->isCrashed = 1;
				npc->isCrashed = 1;
				FMODSystem->playSound(explosion, 0, false, &explosionChannel);
			}

			// Check npc - npc collision
			for (auto & npc2 : NPCShips) {
				if (npc == npc2) {
					continue;
				}
				if (!npc2->isCrashed && length(npc->position - npc2->position) < 4) {
					npc2->isCrashed = 1;
					npc->isCrashed = 1;
				}
			}

			// Check npc-planet collision
			for (auto & planet : planets) {
				if (!npc->isCrashed && length(planet->center - npc->position) < planet->radius + 2) {
					npc->isCrashed = 1;
				}
			}
		}
	}

	// Beam - Ship Collision
	for (int i = 0; i < beams.size(); i++) {
		// Beam - player collision
		for (auto & playerShip : playerShips) {
			if ((beams[i]->owner != player || playerShip != player) &&  length(beams[i]->position - playerShip->position) < 4) {
				delete beams[i];
				beams.erase(beams.begin() + i);
				playerShip->isCrashed = 1;
				playerShip->isHitByBeam = 1;
				FMODSystem->playSound(explosion, 0, false, &explosionChannel);
			}
		}
		
		for (auto & NPCShip : NPCShips) {
			// Beam - NPC Collision
			if (beams[i]->owner != NPCShip && length(beams[i]->position - NPCShip->position) < 4) {
				delete beams[i];
				beams.erase(beams.begin() + i);
				NPCShip->isCrashed = 1;
				NPCShip->isHitByBeam = 1;
				if (beams[i]->owner == player) {
					FMODSystem->playSound(explosion, 0, false, &explosionChannel);
				}
			}
		}
	}
}

// A random NPC Shoots a beam
void World::NPCShoot() {
	for (auto & NPCShip : NPCShips) {
		if (NPCShip->isCrashed == 0) {
			if (rand() % 2 == 0) {
				beams.emplace_back(new Beam(beamBufOffset, NPCShip->position, NPCShip->heading, frameCounter + 100, NPCShip));
			}
		}
	}
}

// Player shoots a beam
void World::playerShoot() {
	if (worldState == State::RUN) {
		if (player->isCrashed == 0 && player->isHitByBeam == 0) {
			FMODSystem->playSound(blaster, 0, false, &blasterChannel);
			blasterChannel->setVolume(0.03f);
			beams.emplace_back(new Beam(beamBufOffset, player->position, player->heading, frameCounter + 100, player));
		}
	}
}

void World::draw() {
	// Update light position with respect to camera
	glUniform4fv(glGetUniformLocation(program_ptr, "LightPosition"), 1, camera->view * vec4(0, 0, 0, 1));

	for (auto & ship : playerShips) { ship->draw(camera->view, program_ptr, ModelView_ptr);}
	for (auto & ship : NPCShips) { ship->draw(camera->view, program_ptr, ModelView_ptr); }

	for (auto & beam : beams) {
		beam->draw(camera->view, program_ptr, ModelView_ptr);
	}

	surface->draw(camera->view, program_ptr, ModelView_ptr);

	glEnable(GL_CULL_FACE); // Do not render inner side of planets, increases performance on higher planet counts
	for (auto & planet : planets) {
		planet->draw(camera->view, program_ptr, ModelView_ptr);
	}
	glDisable(GL_CULL_FACE);	
}