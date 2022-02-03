#include "SpriteObj.h"

SpriteObj::SpriteObj(const std::string& filename, const sf::Vector2f& pos,
                     const sf::Vector2f& scale)
{
    this->pos = pos;
    if (!texture.loadFromFile(filename))
        throw EXCEPT("Cannot load file: " + filename);
    sprite.setTexture(texture);
    sprite.setScale(scale);
    sprite.move(pos);
}

SpriteObj& SpriteObj::operator=(const SpriteObj& other)
{
    if (this != &other) {
        pos     = other.pos;
        texture = other.texture;
        sprite  = other.sprite;
        sprite.setTexture(texture);
    }

    return *this;
}

void SpriteObj::draw(Graphics& gfx) const { gfx.draw(getSprite()); }

void SpriteObj::createTexture(const sf::Vector2i& size)
{
    texture.create(size.x, size.y);
    sprite.setTexture(texture);
}

void SpriteObj::createCanvas()
{
    texture.create(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
    sprite.setTexture(texture);
}

sf::Vector2f SpriteObj::getPos() const noexcept { return pos; }

sf::Texture SpriteObj::getTexture() const noexcept { return texture; }

void SpriteObj::setTexture(const sf::Texture& texture)
{
    this->texture = texture;
    sprite.setTexture(texture);
}

sf::Sprite SpriteObj::getSprite() const noexcept { return sprite; }

void SpriteObj::moveTo(const sf::Vector2f& pos)
{
    sprite.move(pos - this->pos);
    this->pos = pos;
}
