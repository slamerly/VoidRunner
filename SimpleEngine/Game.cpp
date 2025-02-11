#include "Game.h"
#include "Actor.h"
#include "Timer.h"
#include "Assets.h"
#include "MeshComponent.h"
#include "CubeActor.h"
#include "SphereActor.h"
#include "PlaneActor.h"
#include <algorithm>
#include "Enemy.h"
#include "Font.h"
#include "PauseMenu.h"

bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.initialize(window);
	bool isInputInit = inputSystem.initialize();
	bool isFontInit = Font::initialize();
	return isWindowInit && isRendererInit && isInputInit && isFontInit; // Return bool && bool && bool ...to detect error
}

void Game::load()
{
	inputSystem.setMouseRelativeMode(true);

	// ====================================================
	//					Load Ressources
	// ====================================================

	// Shaders
	Assets::loadShader("Res\\Shaders\\Sprite.vert", "Res\\Shaders\\Sprite.frag", "", "", "", "Sprite");
	Assets::loadShader("Res\\Shaders\\BasicMesh.vert", "Res\\Shaders\\BasicMesh.frag", "", "", "", "BasicMesh");
	Assets::loadShader("Res\\Shaders\\Phong.vert", "Res\\Shaders\\Phong.frag", "", "", "", "Phong");

	// Textures
	Assets::loadTexture(renderer, "Res\\Textures\\Default.png", "Default");
	Assets::loadTexture(renderer, "Res\\Textures\\Cube.png", "Cube");
	Assets::loadTexture(renderer, "Res\\Textures\\Plane.png", "Plane");
	Assets::loadTexture(renderer, "Res\\Textures\\Crosshair.png", "Crosshair");
	Assets::loadTexture(renderer, "Res\\Textures\\Sphere.png", "Sphere");
	Assets::loadTexture(renderer, "Res\\Textures\\Col.png", "Col");
	Assets::loadTexture(renderer, "Res\\Textures\\NoCol.png", "NoCol");
	Assets::loadTexture(renderer, "Res\\Textures\\PlayerBall.png", "PlayerBall");
	Assets::loadTexture(renderer, "Res\\Textures\\EnemyBall.png", "EnemyBall");
	Assets::loadTexture(renderer, "Res\\Textures\\Destroyer_01.png", "Destroyer_01");
	Assets::loadTexture(renderer, "Res\\Textures\\Rock.png", "Rock");
	Assets::loadTexture(renderer, "Res\\Textures\\station01_diffuse.png", "station01");
	Assets::loadTexture(renderer, "Res\\Textures\\station03_base_diffuse.png", "station03_base");
	Assets::loadTexture(renderer, "Res\\Textures\\station03_dock_diffuse.png", "station03_dock");
	Assets::loadTexture(renderer, "Res\\Textures\\station03_ring_diffuse.png", "station03_ring");
	Assets::loadTexture(renderer, "Res\\Textures\\Speed.png", "Speed");
	Assets::loadTexture(renderer, "Res\\Textures\\SpeedRight.png", "SpeedRight");
	Assets::loadTexture(renderer, "Res\\Textures\\SpeedUp.png", "SpeedUp");
	Assets::loadTexture(renderer, "Res\\Textures\\Spread.png", "Spread");

	Assets::loadTexture(renderer, "Res\\Textures\\Skybox\\_Skybox.png", "SpaceSkybox");

	// Textures UI
	Assets::loadTexture(renderer, "Res\\Textures\\ButtonYellow.png", "ButtonYellow");
	Assets::loadTexture(renderer, "Res\\Textures\\ButtonBlue.png", "ButtonBlue");
	Assets::loadTexture(renderer, "Res\\Textures\\DialogBG.png", "DialogBG");

	// Meshes
	Assets::loadMesh("Res\\Meshes\\Cube.gpmesh", "Mesh_Cube");
	Assets::loadMesh("Res\\Meshes\\Plane.gpmesh", "Mesh_Plane");
	Assets::loadMesh("Res\\Meshes\\Sphere.gpmesh", "Mesh_Sphere");
	Assets::loadMesh("Res\\Meshes\\skyv2.gpmesh", "Mesh_SpaceSkybox");
	Assets::loadMesh("Res\\Meshes\\Destroyer_01.gpmesh", "Destroyer_01");
	Assets::loadMesh("Res\\Meshes\\Rock_Medium01.gpmesh", "Rock_Medium01");
	Assets::loadMesh("Res\\Meshes\\Rock_Medium02.gpmesh", "Rock_Medium02");
	Assets::loadMesh("Res\\Meshes\\Rock_Small01.gpmesh", "Rock_Small01");
	Assets::loadMesh("Res\\Meshes\\Rock_Small02.gpmesh", "Rock_Small02");
	Assets::loadMesh("Res\\Meshes\\station01.gpmesh", "Station01");
	Assets::loadMesh("Res\\Meshes\\station03_base.gpmesh", "Station03_Base");
	Assets::loadMesh("Res\\Meshes\\station03_dock.gpmesh", "Station03_Dock");
	Assets::loadMesh("Res\\Meshes\\station03_ring.gpmesh", "Station03_Ring");

	// Font
	Assets::loadFont("Res\\Fonts\\SPACE.ttf", "Space");

	std::cout << "\n" << std::endl;
	std::cout << "=========================================\n	IN GAME RETURNS\n=========================================" << std::endl;

	// ====================================================
	//					End Load Ressources
	// ====================================================

	srand(time(nullptr));
	

	Quaternion q(Vector3::unitY, -Maths::piOver2);
	q = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::pi + Maths::pi / 4.0f));


	// ==============================
	//		Setup lights
	// ==============================
	//renderer.setAmbientLight(Vector3(0.25f, 0.25f, 0.25f));
	renderer.setAmbientLight(Vector3(0.5f, 0.5f, 0.5f));
	DirectionalLight& dir = renderer.getDirectionalLight();
	dir.direction = Vector3(0.0f, -0.707f, -0.707f);
	//dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.diffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	//dir.specColor = Vector3(0.8f, 0.8f, 0.8f);
	dir.specColor = Vector3(1.f, 1.0f, 1.0f);


	// ==============================
	//		UI elements
	// ============================== 
	/*Actor* ui = new Actor();
	ui->setPosition(Vector3(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(ui, Assets::getTexture("HealthBar"));

	ui = new Actor();
	ui->setPosition(Vector3(375.0f, -275.0f, 0.0f));
	ui->setScale(0.75f);
	sc = new SpriteComponent(ui, Assets::getTexture("Radar"));*/
	Actor* crosshairActor = new Actor();
	crosshairActor->setScale(2.0f);
	SpriteComponent* scCrosshair = new SpriteComponent(crosshairActor, Assets::getTexture("Crosshair"));

	// ===== Skybox =====
	skybox = new Actor();
	MeshComponent* mcsky = new MeshComponent(skybox);
	mcsky->setMesh(Assets::getMesh("Mesh_SpaceSkybox"));
	mcsky->setLighted(false);
	skybox->setScale(2000);

	// ==============================
	//			Actors
	// ============================== 

	// ===== Player =====
	chara = new Character();

	// ===== Enemies =====
	/*
	Enemy* t = new Enemy();
	const float start = -2500.0f;
	t->setPosition(Vector3(10000, 0, 0.0f));
	*/

	// ===== Asteroids =====
	// DEBUG FIELD
	/*
	SphereActor* beginArea = new SphereActor();
	SphereActor* endArea = new SphereActor();

	beginArea->getMeshComponent()->setTextureIndex(4);
	beginArea->setPosition(Vector3(5000, -50000, -50000));
	beginArea->setScale(25.0f);

	endArea->getMeshComponent()->setTextureIndex(4);
	endArea->setPosition(Vector3(100000, 50000, 50000));
	endArea->setScale(100.0f);
	*/
	beginAsteroidField = Vector3(5000, -50000, -50000);
	endAsteroidField = Vector3(100000, 50000, 50000);

	generateAsteroidField(9, 81, 721);
	
	/*
	Asteroid* ast;
	ast = new Asteroid(AsteroidSize::SMALL);
	ast->setPosition(Vector3(10000, 0, 0));
	ast = new Asteroid(AsteroidSize::SMALL);
	ast->setPosition(Vector3(10000, 1625, 0));
	*/

	// ===================================================================================
	//									Stations
	// ===================================================================================
	/*
	Station* station = new Station(1);
	station->setPosition(Vector3(beginAsteroidField.x - 10000, 0, 0));
	stations.push_back(station);
	*/

	Station* station = new Station(2);
	station->setPosition(Vector3(endAsteroidField.x + 10000, 0, 0));
	stations.push_back(station);
}

void Game::addPlane(PlaneActor* plane)
{
	planes.emplace_back(plane);
}

void Game::removePlane(PlaneActor* plane)
{
	auto iter = std::find(begin(planes), end(planes), plane);
	planes.erase(iter);
}

void Game::addMovableActor(Actor* actor)
{
	movableActors.emplace_back(actor);
}

void Game::removeMovableActor(Actor* actor)
{
	auto iter = std::find(begin(movableActors), end(movableActors), actor);
	movableActors.erase(iter);
}

void Game::generateAsteroidField(int numLarge, int numMedium, int numSmall)
{
	if ((beginAsteroidField.equal(Vector3::zero) && endAsteroidField.equal(Vector3::zero)) ||
		beginAsteroidField.equal(endAsteroidField))
	{
		// Error
		std::cout << "ERROR: begin and/or end Asteroid Field are/is not initialized or are equal." << std::endl;
		return;
	}

	int rangeX = 0;
	int rangeY = 0;
	int rangeZ = 0;

	if ((beginAsteroidField.x >= 0 && endAsteroidField.x >= 0) ||
		(beginAsteroidField.x <= 0 && endAsteroidField.x <= 0))
	{
		rangeX = abs(beginAsteroidField.x - endAsteroidField.x);
	}
	else
	{
		rangeX = abs(beginAsteroidField.x) + abs(endAsteroidField.x);
	}
	if ((beginAsteroidField.y >= 0 && endAsteroidField.y >= 0) ||
		(beginAsteroidField.y <= 0 && endAsteroidField.y <= 0))
	{
		rangeY = abs(beginAsteroidField.y - endAsteroidField.y);
	}
	else
	{
		rangeY = abs(beginAsteroidField.y) + abs(endAsteroidField.y);
	}
	if ((beginAsteroidField.z >= 0 && endAsteroidField.z >= 0) ||
		(beginAsteroidField.z <= 0 && endAsteroidField.z <= 0))
	{
		rangeZ = abs(beginAsteroidField.z - endAsteroidField.z);
	}
	else
	{
		rangeZ = abs(beginAsteroidField.z) + abs(endAsteroidField.z);
	}

	//std::cout << "Range:\nx:" << rangeX << "\ny:" << rangeY << "\nz:" << rangeZ << "\n" << std::endl;

	if (numLarge > 0)
	{
		for (int i = 0; i < numLarge; i++)
		{
			Asteroid* ast = new Asteroid(LARGE);
			placedAsteroid(ast, rangeX, rangeY, rangeZ);
		}
	}

	if (numMedium > 0)
	{
		for (int i = 0; i < numMedium; i++)
		{
			Asteroid* ast = new Asteroid(MEDIUM);
			placedAsteroid(ast, rangeX, rangeY, rangeZ);
		}
	}

	if (numSmall > 0)
	{
		for (int i = 0; i < numSmall; i++)
		{
			Asteroid* ast = new Asteroid(SMALL);
			placedAsteroid(ast, rangeX, rangeY, rangeZ);
		}
	}
}

void Game::placedAsteroid(Asteroid* rock, int rangeX, int rangeY, int rangeZ)
{
	float x, y, z;
	bool placed = false;
	bool rightDist = false;
	int cpt = 0;
	
	while (!placed && cpt < 100)
	{
		std::mt19937 gen(rd());  // to seed mersenne twister.
		std::uniform_int_distribution<> distributX(0, rangeX);
		x = distributX(gen);

		std::uniform_int_distribution<> distributY(0, rangeY);
		y = distributY(gen);

		std::uniform_int_distribution<> distributZ(0, rangeZ);
		z = distributZ(gen);

		if (beginAsteroidField.x > endAsteroidField.x)
		{
			x += endAsteroidField.x;
		}
		else
		{
			x += beginAsteroidField.x;
		}

		if (beginAsteroidField.y > endAsteroidField.y)
		{
			y += endAsteroidField.y;
		}
		else
		{
			y += beginAsteroidField.y;
		}

		if (beginAsteroidField.z > endAsteroidField.z)
		{
			z += endAsteroidField.z;
		}
		else
		{
			z += beginAsteroidField.z;
		}

		rock->setPosition(Vector3(x, y, z));

		if (!asteroids.empty())
		{
			int cptTrue = 0;
			for (const auto& other : asteroids)
			{
				if (rightDistance(*rock, *other))
				{
					cptTrue++;
				}
			}

			if (cptTrue == asteroids.size())
			{
				//std::cout << "Rock: " << rock->getPosition().x << ", " << rock->getPosition().y << ", " << rock->getPosition().z << std::endl;
				asteroids.push_back(rock);
				placed = true;
			}
		}
		else
		{
			//std::cout << "Rock: " << rock->getPosition().x << ", " << rock->getPosition().y << ", " << rock->getPosition().z << std::endl;
			asteroids.push_back(rock);
			placed = true;
		}
		cpt++;
	}

	if (cpt >= 100)
	{
		delete rock;
		std::cout << "Asteroid not placed!" << std::endl;
	}
}

bool Game::rightDistance(Asteroid& origin, Asteroid& target)
{
	float minDist = 0;
	/*
	if (target.getAsteroidSize() <= origin.getAsteroidSize())
	{
		minDist = origin.getAsteroidSize() * 0.5f + 1.5f * target.getAsteroidSize();
	}
	else
	{
		minDist = target.getAsteroidSize() * 0.5f + 1.5f * origin.getAsteroidSize();
	}
	*/

	minDist = origin.getAsteroidSize() * 0.5f + 0.5f * target.getAsteroidSize();

	float dist = Vector3::distance(origin.getPosition(), target.getPosition());

	//std::cout << "dist: " << dist << std::endl;

	if (dist < minDist)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

void Game::processInput()
{
	inputSystem.preUpdate();

	// SDL Event
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		/*
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		}
		*/
		isRunning = inputSystem.processEvent(event);
		if (!isRunning) 
			state = GameState::Quit;
	}

	inputSystem.update();
	const InputState& input = inputSystem.getInputState();

	if (state == GameState::Gameplay)
	{
		// Escape: pause
		if (input.keyboard.getKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::Released)
		{
			new PauseMenu();
			return;
		}

		// Regenerate asteroidsfield
		if (input.keyboard.getKeyState(SDL_SCANCODE_G) == ButtonState::Released)
		{
			std::cout << "\nRegenerate" << std::endl;
			
			for (auto ast : asteroids)
			{
				ast->setState(Actor::ActorState::Dead);
			}
			
			asteroids.clear();

			generateAsteroidField(9, 81, 721);
		}

		// Actor input
		isUpdatingActors = true;
		for (auto actor : actors)
		{
			actor->processInput(input);
		}
		isUpdatingActors = false;
	}
	else
	{
		if (!UIStack.empty())
		{
			UIStack.back()->processInput(input);
		}
	}
}

void Game::update(float dt)
{
	if (state == GameState::Gameplay)
	{
		// Update actors 
		isUpdatingActors = true;
		for (auto actor : actors)
		{
			actor->update(dt);
		}
		isUpdatingActors = false;

		// Put the skybox at the same place then the chara
		skybox->setPosition(chara->getPosition());

		// Move pending actors to actors
		for (auto pendingActor : pendingActors)
		{
			pendingActor->computeWorldTransform();
			actors.emplace_back(pendingActor);
		}
		pendingActors.clear();

		// Delete dead actors
		vector<Actor*> deadActors;
		for (auto actor : actors)
		{
			if (actor->getState() == Actor::ActorState::Dead)
			{
				deadActors.emplace_back(actor);
			}
		}
		for (auto deadActor : deadActors)
		{
			delete deadActor;
		}
	}

	// Update UI screens
	for (auto ui : UIStack)
	{
		if (ui->getState() == UIState::Active)
		{
			ui->update(dt);
		}
	}

	// Delete any UIScreens that are closed
	auto iter = UIStack.begin();
	while (iter != UIStack.end())
	{
		if ((*iter)->getState() == UIState::Closing)
		{
			inputSystem.setMouseRelativeMode(true);
			inputSystem.setMouseCursor(false);
			delete* iter;
			iter = UIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Game::render()
{
	renderer.beginDraw();
	renderer.draw();
	renderer.endDraw();
}

void Game::loop()
{
	Timer timer;
	float dt = 0;
	while (state != GameState::Quit)
	{
		float dt = timer.computeDeltaTime() / 1000.0f;
		processInput();
		update(dt);
		render();
		timer.delayTime();
	}
}

void Game::unload()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!actors.empty())
	{
		delete actors.back();
	}

	// Resources
	Assets::clear();
}

void Game::close()
{
	Font::close();
	inputSystem.close();
	renderer.close();
	window.close();
	SDL_Quit();
}

void Game::addActor(Actor* actor)
{
	if (isUpdatingActors)
	{
		pendingActors.emplace_back(actor);
	}
	else
	{
		actors.emplace_back(actor);
	}
}

void Game::removeActor(Actor* actor)
{
	// Erase actor from the two vectors
	auto iter = std::find(begin(pendingActors), end(pendingActors), actor);
	if (iter != end(pendingActors))
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, end(pendingActors) - 1);
		pendingActors.pop_back();
	}
	iter = std::find(begin(actors), end(actors), actor);
	if (iter != end(actors))
	{
		std::iter_swap(iter, end(actors) - 1);
		actors.pop_back();
	}
}

void Game::pushUI(UIScreen* screen)
{
	UIStack.emplace_back(screen);
}

void Game::setState(GameState stateP)
{
	state = stateP;
}
