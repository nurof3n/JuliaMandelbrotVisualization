#include "Game.h"
#include <algorithm>
#include <string>
#include <conio.h>
#include <cmath>
#include <random>

using namespace std::chrono;

Graphics& Game::gfx = Graphics::getInstance();

Game& Game::getInstance() noexcept {
	static Game _instance;
	return _instance;
}
// setup graphics and load things
void Game::setup() {
	// load shader
	if (!sf::Shader::isAvailable)
		throw EXCEPT("Shaders are not available on this system");
	if (!fractalShader.loadFromFile("Shaders/vertex.vert", "Shaders/fragment.frag"))
		throw EXCEPT("Cannot load shaders");
	// set resolution uniform
	fractalShader.setUniform("Resolution", sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
	// load text font
	if (!textFont.loadFromFile("Content/cour.ttf"))
		throw EXCEPT("Cannot load file: Content/cour.ttf");
	textFPS.setFont(textFont);
	textTAB.setFont(textFont);
	textFPS.setFillColor({ 222, 168, 47 });
	textTAB.setFillColor({ 222, 168, 47 });
	textTAB.setPosition({ 0.0f, 24.0f });
	textFPS.setCharacterSize(20);
	textTAB.setCharacterSize(20);
	textTAB.setString("Press F to toggle FPS\nPress TAB to cycle through color schemes\nPress 0-9 number keys to go through example sets\nPress U to toggle UBER mode\nPress ESCAPE to exit\nPress H to hide controls");
	// setup window
	canvas.createCanvas();
	gfx.setup();
	// set random red dot position inside the window
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> randHeight(0, gfx.getWindow().getSize().y - 1);
	std::uniform_int_distribution<std::mt19937::result_type> randWidth(0, gfx.getWindow().getSize().x - 1);
	sf::Vector2f randomPoint = sf::Vector2f(randWidth(rng), randHeight(rng));
	fractalShader.setUniform("RedDotPos", randomPoint);
	fractalShader.setUniform("IsExample", false);
	fractalShader.setUniform("ColorScheme", 0);
	// initialize member variables
	hasFocus = true;
	showFPS = false;
	showControls = true;
	uberMode = false;
	colorScheme = 0;	// default color scheme
	FPS = 0;
}
// updates game logic
void Game::updateModel() {
	static auto& window = gfx.getWindow();
	// event queue
	sf::Event event;
	while (window.pollEvent(event))
		switch (event.type) {
		case sf::Event::Closed:
			window.close();
			return;
		case sf::Event::GainedFocus:
			hasFocus = true;
			break;
		case sf::Event::LostFocus:
			hasFocus = false;
			break;
		case sf::Event::KeyPressed:
			// exit
			if (hasFocus && event.key.code == sf::Keyboard::Escape) {
				window.close();
				return;
			}
			// toggle FPS
			else
				if (event.key.code == sf::Keyboard::F)
					showFPS = showFPS ? false : true;
			// switch color scheme
				else
					if (event.key.code == sf::Keyboard::Tab) {
						colorScheme = (colorScheme + 1) % 4;
						fractalShader.setUniform("ColorScheme", colorScheme);
					}
			// toggle controls
					else
						if (event.key.code == sf::Keyboard::H)
							showControls = showControls ? false : true;
			// change example
						else
							if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
								fractalShader.setUniform("RedDotPos", cPoints[event.key.code - sf::Keyboard::Num0]);
								fractalShader.setUniform("IsExample", true);
							}
			// toggle UBER
							else
								if (event.key.code == sf::Keyboard::U) {
									uberMode = uberMode ? false : true;
									fractalShader.setUniform("UBER", uberMode);
								}
			break;
		}
	// step out if out of focus
	if (!hasFocus)
		return;

	// set the uniforms: update mouse position only if LMB is pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && gfx.isInWindow(sf::Vector2f(sf::Mouse::getPosition(gfx.getWindow())))) {
		fractalShader.setUniform("IsLMBPressed", true);
		fractalShader.setUniform("RedDotPos", sf::Vector2f(sf::Mouse::getPosition(gfx.getWindow())));
		fractalShader.setUniform("IsExample", false);
	}
	else
		fractalShader.setUniform("IsLMBPressed", false);
}
// draws the objects on the screen
void Game::composeFrame() {
	// draw the canvas and apply the shader
	gfx.draw(canvas.getSprite(), &fractalShader);
	// show FPS
	if (showFPS)
		gfx.draw(textFPS);
	// show controls
	if (showControls)
		gfx.draw(textTAB);
}
// main game loop
void Game::go() {
	if (hasFocus)
		gfx.beginFrame();

	// go through this even if out of focus, to handle the event queue
	updateModel();

	if (hasFocus) {
		composeFrame();
		gfx.endFrame();
	}

	// show FPS
	long long frameTime = frameTimer.mark();
	updateTimens += frameTime / 1000000.0f;
	++FPS;
	if (updateTimens >= 1000.0f) {
		updateTimens -= 1000.0f;
		if (showFPS)
			textFPS.setString("FPS: " + std::to_string(FPS));
		FPS = 0;
	}
}