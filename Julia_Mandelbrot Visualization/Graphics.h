#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "CustomExcept.h"

// encapsulates a sf::RenderWindow object
class Graphics {
private:
	// constructor is private to prevent instantiation from outside
	Graphics() = default;
public:
	// singleton
	static Graphics& getInstance() noexcept;
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	Graphics( Graphics&& ) = delete;
	Graphics& operator=( Graphics&& ) = delete;

	sf::RenderWindow& getWindow() noexcept;
	void setup();
	void beginFrame();
	void endFrame();
	void draw( const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default );
	bool isInWindow( const sf::Vector2f& pos ) const;
private:
	sf::RenderWindow window;
};

