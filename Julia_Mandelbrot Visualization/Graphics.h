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
	static Graphics& GetInstance() noexcept;
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	Graphics( Graphics&& ) = delete;
	Graphics& operator=( Graphics&& ) = delete;

	sf::RenderWindow& GetWindow() noexcept;
	void Setup();
	void BeginFrame();
	void EndFrame();
	void Draw( const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default );
	bool IsInWindow( const sf::Vector2f& pos ) const;
private:
	sf::RenderWindow window;
};

