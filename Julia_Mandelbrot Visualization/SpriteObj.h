#pragma once

#include "Graphics.h"

/**
 * @brief Defines a 2D textured sprite object. It is necessary to hold a separate object for the
 * texture because the sprite member does not hold a copy of the texture that it gets set
 */
class SpriteObj {
public:
    SpriteObj() = default;
    SpriteObj(const std::string& filename, const sf::Vector2f& pos = sf::Vector2f(0.0f, 0.0f),
              const sf::Vector2f& scale = sf::Vector2f(1.0f, 1.0f));

    /**
     * @brief Copies all member variables from other into this object
     *
     * @param other object
     * @return reference to this object
     */
    SpriteObj& operator=(const SpriteObj& other);

    /**
     * @brief Draws the sprite to the graphics object

     * @param gfx object to draw to
     */
    void draw(Graphics& gfx) const;

    /*getters and setters*/
    sf::Vector2f getPos() const noexcept;
    sf::Texture  getTexture() const noexcept;
    void         setTexture(const sf::Texture& texture);
    sf::Sprite   getSprite() const noexcept;

    /**
     * @brief Creates a blank texture of given size
     *
     * @param size
     */
    void createTexture(const sf::Vector2i& size);

    /**
     * @brief Creates a blank texture that spans the entire window
     *
     */
    void createCanvas();

    /**
     * @brief Moves the sprite to specified position
     *
     * @param pos
     */
    void moveTo(const sf::Vector2f& pos);

protected:
    sf::Vector2f pos;      // position of the sprite
    sf::Texture  texture;  // sprite texture
    sf::Sprite   sprite;   // sprite object (should hold a reference to the texture object)
};
