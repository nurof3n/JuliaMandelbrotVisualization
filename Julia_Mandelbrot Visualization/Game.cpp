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
			case sf::Event::KeyPressed:
				// exit
				if( event.key.code == sf::Keyboard::Escape ) {
					window.close();
					return;
				}
		}

	fractalShader.setUniform( "Resolution", sf::Vector2f( sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height ) );
	fractalShader.setUniform( "MousePos", sf::Vector2f( sf::Mouse::getPosition( window ) ) );
	fractalShader.setUniform( "IsLMBPressed", sf::Mouse::isButtonPressed( sf::Mouse::Left ) );
}
// draws the objects on the screen
void Game::ComposeFrame() {
	gfx.Draw( canvas.GetSprite(), &fractalShader );
}
// main game loop
void Game::Go() {
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}