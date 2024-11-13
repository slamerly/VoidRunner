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
#include "Asteroid.h"

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

	// Textures UI
	Assets::loadTexture(renderer, "Res\\Textures\\ButtonYellow.png", "ButtonYellow");
	Assets::loadTexture(renderer, "Res\\Textures\\ButtonBlue.png", "ButtonBlue");
	Assets::loadTexture(renderer, "Res\\Textures\\DialogBG.png", "DialogBG");

	// Meshes
	Assets::loadMesh("Res\\Meshes\\Cube.gpmesh", "Mesh_Cube");
	Assets::loadMesh("Res\\Meshes\\Plane.gpmesh", "Mesh_Plane");
	Assets::loadMesh("Res\\Meshes\\Sphere.gpmesh", "Mesh_Sphere");
	Assets::loadMesh("Res\\Meshes\\Destroyer_01.gpmesh", "Destroyer_01");
	Assets::loadMesh("Res\\Meshes\\Rock_Medium01.gpmesh", "Rock_Medium01");
	Assets::loadMesh("Res\\Meshes\\Rock_Medium02.gpmesh", "Rock_Medium02");
	Assets::loadMesh("Res\\Meshes\\Rock_Small01.gpmesh", "Rock_Small01");
	Assets::loadMesh("Res\\Meshes\\Rock_Small02.gpmesh", "Rock_Small02");

	// Font
	Assets::loadFont("Res\\Fonts\\SPACE.ttf", "Space");

	srand(time(nullptr));

	chara = new Character();
	//chara->setPosition(Vector3(200.0f, 0.0f, 0.0f));
	//chara->setScale(25.0f);

	achieve = new Achievements();

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

	// Enemies
	/*
	Enemy* t = new Enemy();
	const float start = -2500.0f;
	t->setPosition(Vector3(10000, 0, 0.0f));
	*/

	// Asteroids

	Asteroid* ast = new Asteroid(AsteroidSize::LARGE);
	ast->setPosition(Vector3(5500, 0, 0));
	ast->sphere->setPosition(Vector3(5500, 0, 0));
	/*
	for (int i = 0; i < 4; i++)
	{
		Asteroid* ast;
		/*
		if(i == 0 || i == 2)
			ast = new Asteroid(AsteroidSize::SMALL);
		else
			ast = new Asteroid(AsteroidSize::MEDIUM);
		
		ast = new Asteroid(AsteroidSize::LARGE);

		ast->setPosition(Vector3(7500, -6000 + (i * 3000), 0));
		/*
		int selectScale = 1;
		selectScale = rand() % 1 + 10;

		ast->setScale(selectScale);
		std::cout << selectScale << std::endl;
		
	}
	*/
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

void Game::generateAsteroidField(int numLarge, int numMedium, int numSmall, float minDisLarge, float minDisMedium, float minDisSmall)
{
	
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
