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
	if( !fractalShader.loadFromFile( "Shaders/fractal.frag", sf::Shader::Fragment ) )
		throw EXCEPT( "Cannot load file: Shaders/fractal.frag" );
	fractalShader.setUniform( "Resolution", sf::Vector2f( sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height ) );
	if( !textFont.loadFromFile( "Content/cour.ttf" ) )
		throw EXCEPT( "Cannot load file: Content/cour.ttf" );
	text.setFont( textFont );
	canvas.CreateCanvas();
	gfx.Setup();
	hasFocus = true;
	showFPS = false;
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
				// toggle FPS
				else
					if( event.key.code == sf::Keyboard::F ) {
						showFPS = showFPS ? false : true;
						updateTime = 0.25; // for resetting fps
					}
						
				break;
		}

	if( !hasFocus )
		return;
}
// draws the objects on the screen
void Game::ComposeFrame() {
	if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) && gfx.IsInWindow( sf::Vector2f( sf::Mouse::getPosition( gfx.GetWindow() ) ) ) )
		fractalShader.setUniform( "MousePos", sf::Vector2f( sf::Mouse::getPosition( gfx.GetWindow() ) ) );
	fractalShader.setUniform( "IsLMBPressed", sf::Mouse::isButtonPressed( sf::Mouse::Left ) );
	gfx.Draw( canvas.GetSprite(), &fractalShader );
	if( showFPS )
		gfx.Draw( text );
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

	// show FPS
	auto frameTime = frameTimer.Mark();
	if( showFPS ) {
		updateTime += frameTime;
		if( updateTime > 0.25 ) {
			updateTime -= 0.25;
			text.setString( "FPS: " + std::to_string( int( 1.0 / frameTime ) ) );
		}
	} 
}