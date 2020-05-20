#pragma once
#include <SFML/Graphics.hpp>
#include "Graphics.h"

// class that encapsulates sf::Texture and sf::Sprite for ease of use
class SpriteObj {
public:
	SpriteObj() = default;
	SpriteObj( const SpriteObj& ) = default;
	SpriteObj( const std::string& filename, const sf::Vector2f& pos = sf::Vector2f( 0.0f, 0.0f ), const sf::Vector2f& scale = sf::Vector2f( 1.0f, 1.0f ) );
	SpriteObj& operator=( const SpriteObj& other );
	void Draw( Graphics& gfx );
	void CreateTexture( const sf::Vector2i& size );
	void CreateCanvas();
	sf::Vector2f GetPos() const noexcept;
	sf::Texture GetTexture() const noexcept;
	void SetTexture( const sf::Texture& texture );
	sf::Sprite GetSprite() const noexcept;
	void MoveTo( const sf::Vector2f& pos ) noexcept;
protected:
	sf::Vector2f _pos;
	sf::Texture _texture;
	sf::Sprite _sprite;
};

