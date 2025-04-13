#pragma once
#include <vector>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Window.h"
#include "Vector2.h"
#include "RendererOGL.h"
#include "Camera.h"
#include "Character.h"
#include "InputSystem.h"
#include "Achievements.h"
#include "PhysicsSystem.h"
#include "PlaneActor.h"
#include "Asteroid.h"
#include "Station.h"
#include <random>
#include "TargetPoint.h"

using std::vector;

enum class GameState
{
	Gameplay, Pause, Quit
};

class Game
{
public:
	static Game& instance()
	{
		static Game inst;
		return inst;
	}

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

private:
	/*
	Game() : isRunning(true), 
		isUpdatingActors(false), 
		//camera(nullptr), 
		chara(nullptr) {}
	*/
	Game() : state(GameState::Gameplay), isUpdatingActors(false), chara(nullptr) {}


public:
	bool initialize();
	void load();
	void loop();
	void unload();
	void close();

	void addActor(Actor* actor);
	void removeActor(Actor* actor);
	RendererOGL& getRenderer() { return renderer; }
	PhysicsSystem& getPhysicsSystem() { return physicsSystem; }
	InputSystem& getInputSystem() { return inputSystem; }

	const vector<class UIScreen*>& getUIStack() { return UIStack; }
	void pushUI(class UIScreen* screen);
	GameState getState() const { return state; }
	void setState(GameState stateP);

	// ==================================================================
	//						Game Specific
	// ==================================================================
	Character* getCharacter() { return chara; }
	//Camera* getCamera() { return camera; }
	void addPlane(class PlaneActor* plane);
	void removePlane(class PlaneActor* plane);
	vector<PlaneActor*>& getPlanes() { return planes; }
	void addMovableActor(class Actor* actor);
	void removeMovableActor(class Actor* actor);
	vector<Actor*>& getMovableActors() { return movableActors; }

	void generateAsteroidField(int numLarge, int numMedium, int numSmall);
	void placedAsteroid(Asteroid* rock, int rangeX, int rangeY, int rangeZ);

	bool rightDistance(Asteroid& origin, Asteroid& target);

	vector<Actor*> getEnemies() { return enemies; }
	Actor* getPatrolLeader() { return patrolLeader; }
	vector<Actor*> getTargetPoints() { return targetPoints; }

private:
	void processInput();
	void update(float dt);
	void render();

	bool isRunning;
	Window window;
	RendererOGL renderer;
	InputSystem inputSystem;
	PhysicsSystem physicsSystem;

	bool isUpdatingActors;
	vector<Actor*> actors;
	vector<Actor*> pendingActors;

	
	vector<class UIScreen*> UIStack;
	GameState state;

	// ==================================================================
	//						Game Specific
	// ==================================================================
	//Camera* camera;

	std::random_device rd; // non-deterministic generator

	Character* chara;
	Actor* skybox;
	Achievements* achieve;
	vector<PlaneActor*> planes;
	vector<Actor*> movableActors;

	vector<Asteroid*> asteroids;
	Vector3 beginAsteroidField, endAsteroidField;

	vector<Station*> stations;

	vector<Actor*> enemies;
	Actor* patrolLeader;
	vector<Actor*> targetPoints;
};

