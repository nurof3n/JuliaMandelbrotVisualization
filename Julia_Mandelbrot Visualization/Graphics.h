#pragma once

#include <SFML/Graphics.hpp>

#include "CustomExcept.h"

#include <Windows.h>

/**
 * @brief  Encapsulates a render window and defines drawing operations on it. This class uses
 * Singleton Pattern because we only need one render window
 */
class Graphics {
private:
    // constructor is private to prevent instantiation from outside
    Graphics() = default;

public:
    /**
     * @brief Gets the instance of this class
     */
    static Graphics& getInstance()
    {
        static Graphics _instance;
        return _instance;
    }

    /*delete unwanted constructors and assignment operators*/
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    Graphics(Graphics&&)                 = delete;
    Graphics& operator=(Graphics&&) = delete;

    /*getters and setters*/
    const sf::RenderWindow& getWindow() const;
    sf::RenderWindow&       getWindow();

    /**
     * @brief Initializes the window for display (title, icon etc.)
     */
    void setup();

    /**
     * @brief Clears the window's render buffer
     */
    void beginFrame();

    /**
     * @brief Displays the window's render buffer
     */
    void endFrame();

    /**
     * @brief Draws the given object to the window's render buffer, applying the render states
     *
     * @param drawable object to draw
     * @param states render states to apply
     */
    void draw(const sf::Drawable&     drawable,
              const sf::RenderStates& states = sf::RenderStates::Default);

    /**
     * @brief Checks if the given coordinates are inside the window's render canvas
     *
     * @param pos coordinates
     * @return true if inside, false otherwise
     */
    bool isInWindow(const sf::Vector2f& pos) const;

private:
    sf::RenderWindow window;  // window to draw on
};
