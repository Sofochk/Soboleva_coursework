#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::MouseRect;

SDL_Rect Game::camera = { 0,0,1200,1100 };

bool inventory[5] = { 1, 0, 0, 0, 1 };
int items_use[5] = { 0, 0, 0, 0, 0 };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;
bool Game::GameMode = false;
bool Game::Menu = true;
bool Game::Battle = false;
bool Game::StartButton = false;
bool Game::ExitButton = false;
bool Game::Attack = false;

int mult = 1;

auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& menu(manager.addEntity());
auto& startbutton(manager.addEntity());
auto& exitbutton(manager.addEntity());
auto& item(manager.addEntity());
auto& item1(manager.addEntity());
auto& item2(manager.addEntity());
auto& item3(manager.addEntity());
auto& item4(manager.addEntity());
auto& Item(manager.addEntity());
auto& Item1(manager.addEntity());
auto& Item2(manager.addEntity());
auto& Item3(manager.addEntity());
auto& Item4(manager.addEntity());
auto& ghoul(manager.addEntity());
auto& battle(manager.addEntity());
auto& hp(manager.addEntity());
auto& ghoulhp(manager.addEntity());
auto& back(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}

	assets->AddTexture("terrain", "assets/terrain_ss.png");
	assets->AddTexture("player", "assets/player_anims.png");
	assets->AddTexture("ghoul", "assets/ghoul.png");
	assets->AddTexture("menu", "assets/Menu.png");
	assets->AddTexture("startbutton", "assets/StartButton.png");
	assets->AddTexture("startbuttonpressed", "assets/StartButtonPressed.png");
	assets->AddTexture("exitbutton", "assets/ExitButton.png");
	assets->AddTexture("exitbuttonpressed", "assets/ExitButtonPressed.png");
	assets->AddTexture("item", "assets/item.png");
	assets->AddTexture("item1", "assets/item1.png");
	assets->AddTexture("item2", "assets/item2.png");
	assets->AddTexture("item3", "assets/item3.png");
	assets->AddTexture("item4", "assets/item4.png");
	assets->AddTexture("win", "assets/win.png");
	assets->AddTexture("defeat", "assets/defeat.png");

	assets->AddFont("arial", "assets/arial.ttf", 16);
	
	map = new Map("terrain", 3, 32);


	map->LoadMap("assets/map.map", 25, 20);

	player.addComponent<TransformComponent>(800.0f, 640.0f, 48, 32, 4);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player", 900.0f, 600.0f, 80, 80);
	player.addGroup(groupPlayers);

	ghoul.addComponent<TransformComponent>(1800.0f, 1500.0f, 64, 48, 4);
	ghoul.addComponent<SpriteComponent>("ghoul", true);
	ghoul.addComponent<ColliderComponent>("ghoul", 1800.0f, 1500.0f, 200, 300);
	ghoul.addGroup(groupPlayers);

	item.addComponent<TransformComponent>(220.0f, 110.0f, 4, 4, 10);
	item.addComponent<SpriteComponent>("item");
	item.addComponent<ColliderComponent>("item");
	item.addGroup(groupItems);
	item.addGroup(groupUI);

	item1.addComponent<TransformComponent>(890.0f, 1500.0f, 4, 4, 10);
	item1.addComponent<SpriteComponent>("item1");
	item1.addComponent<ColliderComponent>("item1");
	item1.addGroup(groupItems);

	item2.addComponent<TransformComponent>(350.0f, 1500.0f, 4, 4, 10);
	item2.addComponent<SpriteComponent>("item2");
	item2.addComponent<ColliderComponent>("item2");
	item2.addGroup(groupItems);

	item3.addComponent<TransformComponent>(1800.0f, 200.0f, 4, 4, 10);
	item3.addComponent<SpriteComponent>("item3");
	item3.addComponent<ColliderComponent>("item3");
	item3.addGroup(groupItems);

	item4.addComponent<TransformComponent>(2100.0f, 1000.0f, 4, 4, 10);
	item4.addComponent<SpriteComponent>("item4");
	item4.addComponent<ColliderComponent>("item4");
	item4.addGroup(groupItems);


	if (Menu) {

		menu.addComponent<TransformComponent>(0.0f, 0.0f, 800, 1200, 1);
		menu.addComponent<SpriteComponent>("menu");
		menu.addGroup(groupUI);
		startbutton.addComponent<TransformComponent>(100.0f, 200.0f, 64, 256, 1);
		startbutton.addComponent<SpriteComponent>("startbutton");
		startbutton.addGroup(groupUI);
		exitbutton.addComponent<TransformComponent>(100.0f, 320.0f, 64, 256, 1);
		exitbutton.addComponent<SpriteComponent>("exitbutton");
		exitbutton.addGroup(groupUI);
	}
	

	//ecs implementation

	



	SDL_Color white = { 255, 255, 255, 255 };

	label.addComponent<UILabel>(10, 10, "", "arial", white);

	/*assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");*/

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& items(manager.getGroup(Game::groupItems));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& ui(manager.getGroup(Game::groupUI));

void Game::handleEvents()
{

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		SDL_GetMouseState(&MouseRect.x, &MouseRect.y);
		//std::cout << MouseRect.x << " " << MouseRect.y << std::endl;
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (StartButton) {
			Menu = false;
			GameMode = true;
			StartButton = false;
			menu.destroy();
			startbutton.destroy();
			exitbutton.destroy();
		}
		if (ExitButton) {
			isRunning = false;
		}
		for (int i = 0; i < 5; i++) {
			if (items_use[i] == 3) {
				items_use[i] = 4;
				switch (i) {
				case 0:
					mult *= 4;
					Item.destroy();
					break;
				case 1:
					mult *= 3;
					Item1.destroy();
					break;
				case 2:
					mult *= 4;
					Item2.destroy();
					break;
				case 3:
					mult *= 2;
					Item3.destroy();
					break;
				case 4:
					mult *= 2;
					Item4.destroy();
					break;
				default:
					break;
				}
			}
		}
		break;
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		Attack = true;
		break;
	case SDL_KEYUP:
		//Attack = false;
		break;
	default:
		break;
	}
}



void Game::update()
{

	if (not Menu && not Battle) {

		SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
		Vector2D playerPos = player.getComponent<TransformComponent>().position;

		/*std::stringstream ss;
		ss << "Player position: " << playerPos;
		label.getComponent<UILabel>().SetLabelText(ss.str(), "arial");*/

		for (auto& c : colliders)
		{
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(cCol, playerCol))
			{
				//std::cout << "Hit" << std::endl;
				player.getComponent<TransformComponent>().velocity.x *= -1;
				player.getComponent<TransformComponent>().velocity.y *= -1;
				player.getComponent<TransformComponent>().position = playerPos;
			}
		}


		for (auto& p : items)
		{
			if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
			{
				//std::cout << "Get item!" << std::endl;
				if (p->getComponent<ColliderComponent>().tag == "item") {
					items_use[0] = 1;
				}
				else {
					items_use[(p->getComponent<ColliderComponent>().tag[4] - '0')] = 1;
				}

				p->destroy();

			}
		}

		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, ghoul.getComponent<ColliderComponent>().collider))
		{
			Game::Battle = true;
		}

		camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - 550);
		camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - 320);

		if (camera.x < 0)
			camera.x = 0;
		if (camera.y < 0)
			camera.y = 0;
		if (camera.x > camera.w)
			camera.x = camera.w;
		if (camera.y > camera.h)
			camera.y = camera.h;
	}

	else if (Menu) {

		if (MouseRect.x >= 100 && MouseRect.x <= 356) {
			if (MouseRect.y >= 200 && MouseRect.y <= 264) {
				startbutton.getComponent<SpriteComponent>().setTex("startbuttonpressed");
				StartButton = true;
			}
			else if (MouseRect.y >= 320 && MouseRect.y <= 384) {
				exitbutton.getComponent<SpriteComponent>().setTex("exitbuttonpressed");
				ExitButton = true;
			}
			else if (StartButton) {
				startbutton.getComponent<SpriteComponent>().setTex("startbutton");
				StartButton = false;
			}
			else if (ExitButton) {
				exitbutton.getComponent<SpriteComponent>().setTex("exitbutton");
				ExitButton = false;
			}
		}
		else {
			if (StartButton) {
				startbutton.getComponent<SpriteComponent>().setTex("startbutton");
				StartButton = false;
			}
			if (ExitButton) {
				exitbutton.getComponent<SpriteComponent>().setTex("exitbutton");
				ExitButton = false;
			}
		}
	}

	else if (Battle) {


		//std::cout << MouseRect.x << " " << MouseRect.y << std::endl;

		if (GameMode) {

			player.getComponent<KeyboardController>().canmove = false;

			player.getComponent<TransformComponent>().position.x = 1400;
			player.getComponent<TransformComponent>().position.y = 1400;
			ghoul.getComponent<TransformComponent>().position.x = 2000;
			ghoul.getComponent<TransformComponent>().position.y = 1300;

			hp.addComponent<TransformComponent>(player.getComponent<TransformComponent>().position.x, player.getComponent<TransformComponent>().position.y, 4, 27, 5);
			hp.addComponent<SpriteComponent>("item1");
			hp.addGroup(groupUI);

			ghoulhp.addComponent<TransformComponent>(ghoul.getComponent<TransformComponent>().position.x, ghoul.getComponent<TransformComponent>().position.y, 4, 40, 5);
			ghoulhp.addComponent<SpriteComponent>("item2");
			ghoulhp.addGroup(groupUI);

			if (items_use[0] == 1) {
				items_use[0] = 2;
				if (!Item.hasComponent<TransformComponent>()) {
					Item.addComponent<TransformComponent>(1400.0f, 1300.0f, 4, 4, 10);
					Item.addComponent<SpriteComponent>("item");
					Item.addComponent<ColliderComponent>("item");
					Item.addGroup(groupUI);
				}

			}
			if (items_use[1] == 1) {
				items_use[1] = 2;
				if (!Item1.hasComponent<TransformComponent>()) {
					Item1.addComponent<TransformComponent>(1460.0f, 1300.0f, 4, 4, 10);
					Item1.addComponent<SpriteComponent>("item1");
					Item1.addComponent<ColliderComponent>("item1");
					Item1.addGroup(groupUI);
				}
			}
			if (items_use[2] == 1) {
				items_use[2] = 2;
				if (!Item2.hasComponent<TransformComponent>()) {
					Item2.addComponent<TransformComponent>(1520.0f, 1300.0f, 4, 4, 10);
					Item2.addComponent<SpriteComponent>("item2");
					Item2.addComponent<ColliderComponent>("item2");
					Item2.addGroup(groupUI);
				}
			}
			if (items_use[3] == 1) {
				items_use[3] = 2;
				if (!Item3.hasComponent<TransformComponent>()) {
					Item3.addComponent<TransformComponent>(1580.0f, 1300.0f, 4, 4, 10);
					Item3.addComponent<SpriteComponent>("item3");
					Item3.addComponent<ColliderComponent>("item3");
					Item3.addGroup(groupUI);
				}
			}
			if (items_use[4] == 1) {
				items_use[4] = 2;
				if (!Item4.hasComponent<TransformComponent>()) {
					Item4.addComponent<TransformComponent>(1640.0f, 1300.0f, 4, 4, 10);
					Item4.addComponent<SpriteComponent>("item4");
					Item4.addComponent<ColliderComponent>("item4");
					Item4.addGroup(groupUI);
				}
			}

			camera.x = 1200;
			camera.y = 1100;

			GameMode = false;

		}

		if (MouseRect.y >= 200 && MouseRect.y <= 240) {
			if (MouseRect.x >= 200 && MouseRect.x <= 240) {
				for (int i = 0; i < 5; i++) {
					if (items_use[i] == 3) {
						items_use[i] = 2;
					}
				}
				if (items_use[0] == 2) {
					items_use[0] = 3;
				}
			}
			if (MouseRect.x >= 260 && MouseRect.x <= 300) {
				for (int i = 0; i < 5; i++) {
					if (items_use[i] == 3) {
						items_use[i] = 2;
					}
				}
				if (items_use[1] == 2) {
					items_use[1] = 3;
				}
			}
			if (MouseRect.x >= 320 && MouseRect.x <= 360) {
				for (int i = 0; i < 5; i++) {
					if (items_use[i] == 3) {
						items_use[i] = 2;
					}
				}
				if (items_use[2] == 2) {
					items_use[2] = 3;
				}
			}
			if (MouseRect.x >= 380 && MouseRect.x <= 420) {
				for (int i = 0; i < 5; i++) {
					if (items_use[i] == 3) {
						items_use[i] = 2;
					}
				}
				if (items_use[3] == 2) {
					items_use[3] = 3;
				}
			}
			if (MouseRect.x >= 440 && MouseRect.x <= 480) {
				for (int i = 0; i < 5; i++) {
					if (items_use[i] == 3) {
						items_use[i] = 2;
					}
				}
				if (items_use[4] == 2) {
					items_use[4] = 3;
				}
			}
		}

		

		if (Attack) {

			Attack = false;
			hp.getComponent<TransformComponent>().width -= std::rand() % 3;
			ghoulhp.getComponent<TransformComponent>().width -= mult * (std::rand() % 3);

			if (mult != 1) {
				mult = 1;
			}

			if (hp.getComponent<TransformComponent>().width <= 0) {

				back.addComponent<TransformComponent>(player.getComponent<TransformComponent>().position.x - 200, player.getComponent<TransformComponent>().position.y - 300, 800, 1200, 1);
				back.addComponent<SpriteComponent>("defeat");
				back.addGroup(groupUI);
				hp.destroy();
				ghoulhp.destroy();
				player.destroy();
				ghoul.destroy();
			}
			if (ghoulhp.getComponent<TransformComponent>().width <= 0) {

				back.addComponent<TransformComponent>(player.getComponent<TransformComponent>().position.x - 200, player.getComponent<TransformComponent>().position.y - 300, 800, 1200, 1);
				back.addComponent<SpriteComponent>("win");
				back.addGroup(groupUI);
				hp.destroy();
				ghoulhp.destroy();
				player.destroy();
				ghoul.destroy();
			}

			SDL_Delay(100);

		}

	}

	manager.refresh();
	manager.update();

}

void Game::render()
{
	SDL_RenderClear(renderer);

	if (not Battle) {

		for (auto& t : tiles)
		{
			t->draw();
		}

		for (auto& c : colliders)
		{
			c->draw();
		}

		for (auto& p : items)
		{
			p->draw();
		}


	}

	for (auto& p : ui)
	{
		p->draw();
	}
	
	if (not Menu) {

		for (auto& p : players)
		{
			p->draw();
		}
	}
	

	label.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}