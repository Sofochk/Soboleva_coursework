#pragma once

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;

public:

	int animIndex = 0;
	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;

		/*Animation idle = Animation(0, 3, 100);
		Animation walk = Animation(1, 8, 100);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);*/
		
		if (id == "player") {

			Animation idle_front = Animation(0, 4, 100);
			Animation idle_back = Animation(1, 4, 100);
			Animation idle_right = Animation(2, 4, 100);
			Animation walk_front = Animation(3, 8, 100);
			Animation walk_back = Animation(4, 8, 100);
			Animation walk_right = Animation(5, 12, 100);

			animations.emplace("Idle_f", idle_front);
			animations.emplace("Idle_b", idle_back);
			animations.emplace("Idle_r", idle_right);
			animations.emplace("Walk_f", walk_front);
			animations.emplace("Walk_b", walk_back);
			animations.emplace("Walk_r", walk_right);

			Play("Idle_f");

		}

		if (id == "ghoul") {

			Animation idle_ghoul = Animation(0, 3, 100);
			Animation walk_ghoul = Animation(1, 8, 100);
			Animation attack_ghoul = Animation(2, 14, 100);

			animations.emplace("Idle_ghoul", idle_ghoul);
			animations.emplace("Walk_ghoul", walk_ghoul);
			animations.emplace("Attack_ghoul", attack_ghoul);

			Play("Idle_ghoul");

		}


		setTex(id);
	}

	~SpriteComponent()
	{
	}

	void setTex(std::string id)
	{
		texture = Game::assets->GetTexture(id);
	}

	void init() override
	{

		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{

		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x - Game::camera.x);
		destRect.y = static_cast<int>(transform->position.y - Game::camera.y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}

};