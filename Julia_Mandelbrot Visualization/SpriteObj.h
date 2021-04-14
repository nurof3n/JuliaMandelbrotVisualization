#pragma once
#include <SFML/Graphics.hpp>
#include "Graphics.h"

// class that encapsulates sf::Texture and sf::Sprite for ease of use
class SpriteObj {
public:
	SpriteObj() = default;
	SpriteObj(const std::string& filename, const sf::Vector2f& pos = sf::Vector2f(0.0f, 0.0f), const sf::Vector2f& scale = sf::Vector2f(1.0f, 1.0f));
	SpriteObj& operator=(const SpriteObj& other);

	void draw(Graphics& gfx) const;
	void createTexture(const sf::Vector2i& size);
	void createCanvas();
	sf::Vector2f getPos() const noexcept;
	sf::Texture getTexture() const noexcept;
	void setTexture(const sf::Texture& texture);
	sf::Sprite getSprite() const noexcept;
	void moveTo(const sf::Vector2f& pos);
protected:
	sf::Vector2f _pos;
	sf::Texture _texture;
	sf::Sprite _sprite;
};

