#include "SpriteObj.h"

SpriteObj::SpriteObj(const std::string& filename, const sf::Vector2f& pos, const sf::Vector2f& scale) {
	_pos = pos;
	if (!_texture.loadFromFile(filename))
		throw EXCEPT("Cannot load file: " + filename);
	_sprite.setTexture(_texture);
	_sprite.setScale(scale);
	_sprite.move(_pos);
}

SpriteObj& SpriteObj::operator=(const SpriteObj& other) {
	if (this != &other) {
		_pos = other._pos;
		_texture = other._texture;
		_sprite = other._sprite;
		_sprite.setTexture(_texture);
	}
	return *this;
}
// draw sprite to gfx window
void SpriteObj::draw(Graphics& gfx) const {
	gfx.draw(getSprite());
}
// creates a blank texture of dimension size
void SpriteObj::createTexture(const sf::Vector2i& size) {
	_texture.create(size.x, size.y);
	// update the sprite texture
	_sprite.setTexture(_texture);
}
// creates a texture that spans the whole window
void SpriteObj::createCanvas() {
	_texture.create(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	// update the sprite texture
	_sprite.setTexture(_texture);
}

sf::Vector2f SpriteObj::getPos() const noexcept {
	return _pos;
}

sf::Texture SpriteObj::getTexture() const noexcept {
	return _texture;
}

void SpriteObj::setTexture(const sf::Texture& texture) {
	_texture = texture;
	// update the sprite texture
	_sprite.setTexture(texture);
}

sf::Sprite SpriteObj::getSprite() const noexcept {
	return _sprite;
}

void SpriteObj::moveTo(const sf::Vector2f& pos) {
	_sprite.move(pos - _pos);
	// set the position too
	_pos = pos;
}
