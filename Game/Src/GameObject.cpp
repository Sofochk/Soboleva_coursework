#include "GameObject.h"
#include "TextureManager.h"


GameObject::GameObject(const char* texturesheet, int x, int y) {
	objTexture = TextureManager::LoadTexture(texturesheet);
	xpos = x;
	ypos = y;
}

void GameObject::Update() {

	xpos++;
	ypos++;

	scrRect.h = 64;
	scrRect.w = 48;
	scrRect.x = 0;
	scrRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = scrRect.w * 2;
	destRect.h = scrRect.h * 2;
}

void GameObject::Render() {
	SDL_RenderCopy(Game::renderer, objTexture, &scrRect, &destRect);
}

