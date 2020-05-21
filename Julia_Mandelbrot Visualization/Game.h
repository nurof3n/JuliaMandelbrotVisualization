#pragma once
#include "Button.h"
#include "Graphics.h"
#include "FrameTimer.h"

class Game {
private:
	// constructor is private to prevent instantiation from outside
	Game() = default;
public:
	// singleton
	static Game& GetInstance() noexcept;
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	Game( Game&& ) = delete;
	Game& operator=( Game&& ) = delete;

	void Setup();
	void ComposeFrame();
	void UpdateModel();
	void Go();
public:
	static Graphics& gfx;
private:
	SpriteObj canvas;
	sf::Shader fractalShader;
	bool hasFocus;		// focus of the window
	bool showFPS;
	bool showControls;
	bool uberMode;		// 4 times more iterations for more depth
	FrameTimer frameTimer;
	float updateTime;	// update rate for fps
	sf::Font textFont;
	sf::Text textFPS, textTAB;
	int colorScheme;	// varies from 0 to 3
	sf::Vector2f cPoints[10] = { {-0.1f, 0.651f}, {-1.0f, 0.0f}, {0.687f, 0.312f}, {0.295f, 0.55f}, {-0.4f, 0.6f}, {0.25f, 0.0f}, {0.28f, 0.008f}, {-0.12f,-0.77f}, {-0.79f,0.15f}, {0.0f, -0.8f} };	// examples of julia sets
};

