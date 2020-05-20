#include "Game.h"
#include <algorithm>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cmath>
#include <random>

Graphics& Game::gfx = Graphics::GetInstance();

// singleton
Game& Game::GetInstance() noexcept {
	static Game _instance;
	return _instance;
}
// intro in console
void Game::Setup() {
	// load shader
	if( !fractalShader.loadFromFile( "Shaders/fractal.frag", sf::Shader::Fragment ) )
		throw EXCEPT( "Cannot load file: Shaders/fractal.frag" );
	// set resolution uniform
	fractalShader.setUniform( "Resolution", sf::Vector2f( sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height ) );
	// load text font
	if( !textFont.loadFromFile( "Content/cour.ttf" ) )
		throw EXCEPT( "Cannot load file: Content/cour.ttf" );
	text.setFont( textFont );
	text.setFillColor( { 222, 168, 47 } );
	// setup window
	canvas.CreateCanvas();
	gfx.Setup();
	// set random red dot position inside the window
	std::random_device dev;
	std::mt19937 rng( dev() );
	std::uniform_int_distribution<std::mt19937::result_type> randHeight( 0, gfx.GetWindow().getSize().y - 1 );
	std::uniform_int_distribution<std::mt19937::result_type> randWidth( 0, gfx.GetWindow().getSize().x - 1 );
	fractalShader.setUniform( "MousePos", sf::Vector2f( randWidth( rng ), randHeight( rng ) ) );
	// initialize member variables
	hasFocus = true;
	showFPS = false;
	colorScheme = 0;	// default color scheme
}
// updates game logic
void Game::UpdateModel() {
	static auto& window = gfx.GetWindow();
	// event queue
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
				// switch color scheme
					else
						if( event.key.code == sf::Keyboard::Tab ) {
							colorScheme = (colorScheme + 1) % 4;
						}
				break;
		}
	// step out if out of focus
	if( !hasFocus )
		return;
}
// draws the objects on the screen
void Game::ComposeFrame() {
	// set the uniforms: update mouse position only if LMB is pressed
	if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) && gfx.IsInWindow( sf::Vector2f( sf::Mouse::getPosition( gfx.GetWindow() ) ) ) )
		fractalShader.setUniform( "MousePos", sf::Vector2f( sf::Mouse::getPosition( gfx.GetWindow() ) ) );
	fractalShader.setUniform( "IsLMBPressed", sf::Mouse::isButtonPressed( sf::Mouse::Left ) );
	fractalShader.setUniform( "colorScheme", colorScheme );
	// draw the canvas and apply the shader
	gfx.Draw( canvas.GetSprite(), &fractalShader );
	// show FPS
	if( showFPS )
		gfx.Draw( text );
}
// main game loop
void Game::Go() {
	if( hasFocus )
		gfx.BeginFrame();

	// go through this even if out of focus, to handle the event queue
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