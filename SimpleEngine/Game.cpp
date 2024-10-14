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

	Assets::loadShader("Res\\Shaders\\Sprite.vert", "Res\\Shaders\\Sprite.frag", "", "", "", "Sprite");
	Assets::loadShader("Res\\Shaders\\BasicMesh.vert", "Res\\Shaders\\BasicMesh.frag", "", "", "", "BasicMesh");
	Assets::loadShader("Res\\Shaders\\Phong.vert", "Res\\Shaders\\Phong.frag", "", "", "", "Phong");

	Assets::loadTexture(renderer, "Res\\Textures\\Default.png", "Default");
	Assets::loadTexture(renderer, "Res\\Textures\\Cube.png", "Cube");
	Assets::loadTexture(renderer, "Res\\Textures\\Plane.png", "Plane");
	Assets::loadTexture(renderer, "Res\\Textures\\Crosshair.png", "Crosshair");
	Assets::loadTexture(renderer, "Res\\Textures\\Sphere.png", "Sphere");
	Assets::loadTexture(renderer, "Res\\Textures\\ak74.png", "ak74");
	Assets::loadTexture(renderer, "Res\\Textures\\lm.png", "lm");
	Assets::loadTexture(renderer, "Res\\Textures\\Wood.png", "Wood");
	Assets::loadTexture(renderer, "Res\\Textures\\PlayerBall.png", "PlayerBall");
	Assets::loadTexture(renderer, "Res\\Textures\\EnemyBall.png", "EnemyBall");
	Assets::loadTexture(renderer, "Res\\Textures\\lmDmg.png", "lmDmg");
	Assets::loadTexture(renderer, "Res\\Textures\\lmTrig.png", "lmTrig");
	Assets::loadTexture(renderer, "Res\\Textures\\Col.png", "Col");
	Assets::loadTexture(renderer, "Res\\Textures\\NoCol.png", "NoCol");
	Assets::loadTexture(renderer, "Res\\Textures\\Destroyer_01.png", "Destroyer_01");

	Assets::loadTexture(renderer, "Res\\Textures\\ButtonYellow.png", "ButtonYellow");
	Assets::loadTexture(renderer, "Res\\Textures\\ButtonBlue.png", "ButtonBlue");
	Assets::loadTexture(renderer, "Res\\Textures\\DialogBG.png", "DialogBG");

	Assets::loadMesh("Res\\Meshes\\Cube.gpmesh", "Mesh_Cube");
	Assets::loadMesh("Res\\Meshes\\Plane.gpmesh", "Mesh_Plane");
	Assets::loadMesh("Res\\Meshes\\Wood.gpmesh", "Mesh_Wood");
	Assets::loadMesh("Res\\Meshes\\Sphere.gpmesh", "Mesh_Sphere");
	Assets::loadMesh("Res\\Meshes\\ak74.gpmesh", "Mesh_Ak74");
	Assets::loadMesh("Res\\Meshes\\lemon.gpmesh", "Mesh_Lemon");
	Assets::loadMesh("Res\\Meshes\\Destroyer_01.gpmesh", "Destroyer_01");

	Assets::loadFont("Res\\Fonts\\SPACE.ttf", "Space");

	chara = new Character();
	//chara->setPosition(Vector3(200.0f, 0.0f, 0.0f));
	//chara->setScale(25.0f);

	achieve = new Achievements();

	/*camera = new Camera();
	camera->setCharacter(chara);
	camera->addObserver(achieve);*/

	Quaternion q(Vector3::unitY, -Maths::piOver2);
	q = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::pi + Maths::pi / 4.0f));

	/*BulletHole* bh = new BulletHole();
	bh->setPosition(Vector3(1.0f, 1.0f, 10.0f));*/


	// ==============================
	//		Floor and walls
	// ==============================

	// Setup floor
	const float start = -2500.0f;
	const float startL = -4500.0f;
	const float size = 500.0f;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			PlaneActor* p = new PlaneActor();
			p->setPosition(Vector3(start + i * size, startL + j * size, -100.0f));

			p = new PlaneActor();
			p->setPosition(Vector3(start + i * size, startL + j * size, 500.0f));
		}
	}

	// Left/right walls
	q = Quaternion(Vector3::unitX, Maths::piOver2);
	for (int i = 0; i < 10; i++)
	{
		PlaneActor* p = new PlaneActor();
		p->changeMesh(&Assets::getMesh("Mesh_Wood"));
		p->setPosition(Vector3(start + i * size, startL - size, 0.0f));
		p->setRotation(q);

		p = new PlaneActor();
		p->changeMesh(&Assets::getMesh("Mesh_Wood"));
		p->setPosition(Vector3(start + i * size, -startL, 0.0f));
		p->setRotation(q);

		if (i >= 2 && i < 8)
		{
			p = new PlaneActor();
			p->changeMesh(&Assets::getMesh("Mesh_Wood"));
			p->setPosition(Vector3(start + i * size, startL + size , 0.0f));
			p->setRotation(q);

			p = new PlaneActor();
			p->changeMesh(&Assets::getMesh("Mesh_Wood"));
			p->setPosition(Vector3(start + i * size, startL + size * 5, 0.0f));
			p->setRotation(q);

			p = new PlaneActor();
			p->changeMesh(&Assets::getMesh("Mesh_Wood"));
			p->setPosition(Vector3(start + i * size, -startL - size * 2, 0.0f));
			p->setRotation(q);

			p = new PlaneActor();
			p->changeMesh(&Assets::getMesh("Mesh_Wood"));
			p->setPosition(Vector3(start + i * size, -startL - size * 6, 0.0f));
			p->setRotation(q);
		}
		if (i < 2 || i >= 8)
		{
			p = new PlaneActor();
			p->changeMesh(&Assets::getMesh("Mesh_Wood"));
			p->setPosition(Vector3(start + i * size, startL + size * 7, 0.0f));
			p->setRotation(q);

			p = new PlaneActor();
			p->changeMesh(&Assets::getMesh("Mesh_Wood"));
			p->setPosition(Vector3(start + i * size, -startL - size * 8, 0.0f));
			p->setRotation(q);
		}
	}

	// Forward/back walls
	/*
	q = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::piOver2));
	for (int i = 0; i < 18; i++)
	{
		PlaneActor* p = new PlaneActor();
		p->changeMesh(&Assets::getMesh("Mesh_Wood"));
		p->setPosition(Vector3(start - size, startL + i * size, 0.0f));
		p->setRotation(q);

		p = new PlaneActor();
		p->changeMesh(&Assets::getMesh("Mesh_Wood"));
		p->setPosition(Vector3(-start, startL + i * size, 0.0f));
		p->setRotation(q);

		if ((i > 1 && i < 5) || (i > 12 && i < 16))
		{
			p = new PlaneActor();
			p->changeMesh(&Assets::getMesh("Mesh_Wood"));
			p->setPosition(Vector3(start + size, startL + i * size, 0.0f));
			p->setRotation(q);

			p = new PlaneActor();
			p->changeMesh(&Assets::getMesh("Mesh_Wood"));
			p->setPosition(Vector3(-start - size * 2, startL + i * size, 0.0f));
			p->setRotation(q);
		}
		if (i > 7 && i < 10)
		{
			p = new PlaneActor();
			p->changeMesh(&Assets::getMesh("Mesh_Wood"));
			p->setPosition(Vector3(start + size * 2, startL + i * size, 0.0f));
			p->setRotation(q);

			p = new PlaneActor();
			p->changeMesh(&Assets::getMesh("Mesh_Wood"));
			p->setPosition(Vector3(-start - size * 3, startL + i * size, 0.0f));
			p->setRotation(q);
		}
	}
	*/
	// ==============================
	//		Setup lights
	// ==============================
	renderer.setAmbientLight(Vector3(0.25f, 0.25f, 0.25f));
	DirectionalLight& dir = renderer.getDirectionalLight();
	dir.direction = Vector3(0.0f, -0.707f, -0.707f);
	dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	//dir.diffuseColor = Vector3(0.f, 0.f, 0.f);
	//dir.specColor = Vector3(0.8f, 0.8f, 0.8f);
	dir.specColor = Vector3(0.1f, 0.1f, 0.1f);


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
	Enemy* t = new Enemy();
	t->setPosition(Vector3(start + 10000, 0, 0.0f));
	//t->setRotation(Quaternion(Vector3::unitZ, -Maths::piOver2));
	/*
	Enemy* t = new Enemy();
	t->setPosition(Vector3(-start - size, -startL - size, 0.0f));
	//t->setRotation(Quaternion(Vector3::unitZ, -Maths::piOver2));
	t = new Enemy();
	t->setPosition(Vector3(-start - size, startL, 0.0f));
	t->setRotation(Quaternion(Vector3::unitZ, Maths::piOver2));
	t = new Enemy();
	t->setPosition(Vector3(start, -startL - size, 0.0f));
	t->setRotation(Quaternion(Vector3::unitZ, -Maths::piOver2));
	t = new Enemy();
	t->setPosition(Vector3(start, startL, 0.0f));
	t->setRotation(Quaternion(Vector3::unitZ, Maths::twoPi));

	t = new Enemy();
	t->setPosition(Vector3(-start - size, -startL - size * 7, 0.0f));
	t->setRotation(Quaternion(Vector3::unitZ, Maths::piOver2));
	t = new Enemy();
	t->setPosition(Vector3(start, -startL - size * 7, 0.0f));
	t->setRotation(Quaternion(Vector3::unitZ, Maths::twoPi));
	t = new Enemy();
	t->setPosition(Vector3(-start - size, startL + size * 6, 0.0f));
	t = new Enemy();
	t->setPosition(Vector3(start, startL + size * 6, 0.0f));
	t->setRotation(Quaternion(Vector3::unitZ, -Maths::piOver2));

	
	//Enemy* t = new Enemy();
	t->setPosition(Vector3(0.0f, 1200.0f, 0.0f));
	t->setRotation(Quaternion(Vector3::unitZ, -Maths::piOver2));
	t = new Enemy();
	t->setPosition(Vector3(0.0f, -1200.0f, 0.0f));
	t->setRotation(Quaternion(Vector3::unitZ, Maths::piOver2));
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
