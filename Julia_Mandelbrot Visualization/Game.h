#pragma once
#include "Button.h"
#include "Graphics.h"

class Game {
private:
	Game() = default;
public:
	static Game& GetInstance() noexcept;
	Game( const Game& ) = delete;
	const Game& operator=( const Game& ) = delete;

	void Setup();
	void ComposeFrame();
	void UpdateModel();
	void Go();
public:
	static Graphics& gfx;
private:
	SpriteObj canvas;
	sf::Shader fractalShader;
};

