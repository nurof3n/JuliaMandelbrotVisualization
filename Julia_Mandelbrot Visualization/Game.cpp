#include "Game.h"
#include <algorithm>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cmath>

Graphics& Game::gfx = Graphics::GetInstance();

// singleton
Game& Game::GetInstance() noexcept {
	static Game _instance;
	return _instance;
}
// intro in console
void Game::Setup() {
	ShowWindow( GetConsoleWindow(), SW_HIDE );
	if( !fractalShader.loadFromFile( "Shaders/fractal.frag", sf::Shader::Fragment ) )
		throw EXCEPT( "Cannot load file: Shaders/fractal.frag" );
	canvas.CreateCanvas();
	gfx.Setup();
	hasFocus = true;
}
// updates game logic
void Game::UpdateModel() {
	static auto& window = gfx.GetWindow();

	sf::Event event;
	while( window.pollEvent( event ) )
		switch( event.type ) {
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
				if( hasFocus && event.key.code == sf::Keyboard::Escape ) {
					window.close();
					return;
				}
		}

	if( !hasFocus )
		return;
}
// draws the objects on the screen
void Game::ComposeFrame() {
	fractalShader.setUniform( "Resolution", sf::Vector2f( sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height ) );
	if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
		fractalShader.setUniform( "MousePos", sf::Vector2f( sf::Mouse::getPosition( gfx.GetWindow() ) ) );
	fractalShader.setUniform( "IsLMBPressed", sf::Mouse::isButtonPressed( sf::Mouse::Left ) );
	gfx.Draw( canvas.GetSprite(), &fractalShader );
}
// main game loop
void Game::Go() {
	if( hasFocus )
		gfx.BeginFrame();

	UpdateModel();

	if( hasFocus ) {
		ComposeFrame();
		gfx.EndFrame();
	}
}