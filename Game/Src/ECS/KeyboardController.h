#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:

	bool moveright = true;
	bool movevert = false;
	bool movehor = false;

	bool canmove = true;

	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (canmove) {
			if (not moveright) {
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			}

			else {
				sprite->spriteFlip = SDL_FLIP_NONE;
			}

			if (Game::event.type == SDL_KEYDOWN)
			{
				switch (Game::event.key.keysym.sym)
				{
				case SDLK_w:
					movevert = true;
					transform->velocity.y = -1;
					sprite->Play("Walk_b");
					break;
				case SDLK_a:
					movehor = true;
					moveright = false;
					transform->velocity.x = -1;
					sprite->Play("Walk_r");
					//sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					break;
				case SDLK_d:
					movehor = true;
					moveright = true;
					transform->velocity.x = 1;
					sprite->Play("Walk_r");
					break;
				case SDLK_s:
					movevert = true;
					transform->velocity.y = 1;
					sprite->Play("Walk_f");
					break;
				default:
					break;
				}
			}

			if (Game::event.type == SDL_KEYUP)
			{
				switch (Game::event.key.keysym.sym)
				{
				case SDLK_w:
					movevert = false;
					transform->velocity.y = 0;
					if (not movehor) {
						sprite->Play("Idle_b");
					}
					break;
				case SDLK_a:
					movehor = false;
					transform->velocity.x = 0;
					if (not movevert) {
						sprite->Play("Idle_r");
					}
					//sprite->spriteFlip = SDL_FLIP_NONE;
					break;
				case SDLK_d:
					movehor = false;
					transform->velocity.x = 0;
					if (not movevert) {
						sprite->Play("Idle_r");
					}
					break;
				case SDLK_s:
					movevert = false;
					transform->velocity.y = 0;
					if (not movehor) {
						sprite->Play("Idle_f");
					}
					break;
				case SDLK_ESCAPE:
					Game::isRunning = false;
				default:
					break;
				}
			}
		}
		else {
			transform->velocity.x = 0;
			transform->velocity.y = 0;
			sprite->Play("Idle_f");
			
		}
	}
};