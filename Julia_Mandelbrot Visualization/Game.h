#pragma once

#include "FrameTimer.h"
#include "Graphics.h"
#include "SpriteObj.h"

/**
 * @brief Defines the game state. Uses Singleton Pattern to ensure only one instance of the game is
 * run
 *
 */
class Game {
private:
    // constructor is private to prevent instantiation from outside
    Game() = default;

    /**
     * @brief Updates the game state with user input
     */
    void updateModel();

    /**
     * @brief Draws the objects on the canvas
     */
    void composeFrame();

public:
    /**
     * @brief Gets the instance of this class
     */
    static Game& getInstance()
    {
        static Game _instance;
        return _instance;
    }

    /*delete unwanted constructors and assignment operators*/
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&)                 = delete;
    Game& operator=(Game&&) = delete;

    /**
     * @brief Setup the graphics object and load resources. This should be called before starting
     * the game
     */
    void setup();

    /**
     * @brief The game loop. This should be called inside the main loop
     */
    void go();

    /*getters and setters*/
    void            setFrametimeToggle(bool toggle);
    void            toggleFrametime();
    void            setControlsToggle(bool toggle);
    void            toggleControls();
    void            setUberModeToggle(bool toggle);
    void            toggleUberMode();
    bool            isFocused() const;
    void            setFocused(bool focus);
    const Graphics& getGraphics() const;
    Graphics&       getGraphics();


    /**
     * @brief Sets the color scheme according to a given index

     * @param index
     */
    void setColorScheme(int index);

    /**
     * @brief Cycles the color schemes
     */
    void setNextColorScheme();

    /**
     * @brief Sets the example image according to a given index (it changes the Julia parameter
     * position - red dot - to a preset value)
     *
     * @param index
     */
    void setExampleImage(int index);

    /**
     * @brief Sets the Julia parameter position - red dot - according to given coordinates
     *
     * @param pos new coordinates
     */
    void setRedDot(sf::Vector2f pos);

    /**
     * @brief Measure the time passed from the completion of the previous frame. Should be
     * called at the end of the frame computation
     */
    void measureFrametime();

private:
    /*constants*/
    static constexpr int COLOR_SCHEME_COUNT   = 4;
    static constexpr int IMAGE_EXAMPLES_COUNT = 10;

    Graphics& gfx = Graphics::getInstance();  // reference to the instance of the graphics object

    /*game options toggles*/
    bool showFrametimeToggle;
    bool showControlsToggle;
    bool uberModeToggle;

    SpriteObj  shaderCanvas;   // the canvas object to shade
    sf::Shader fractalShader;  // shader to apply on the canvas

    FrameTimer frameTimer;  // frametimer used for measurements
    float      frametime;   // game frametime measure in milliseconds

    /*text objects*/
    sf::Font font;
    sf::Text textFrametime;  // shows the game frametime in milliseconds
    sf::Text textControls;   // shows the controls

    bool         focused;      // focus of the window
    int          colorScheme;  // defines the color scheme used (0-3)
    sf::Vector2f juliaParameters[IMAGE_EXAMPLES_COUNT] = {
        {-0.1f, 0.651f}, {-1.0f, 0.0f}, {0.687f, 0.312f}, {0.295f, 0.55f},
        {-0.4f, 0.6f},   {0.25f, 0.0f}, {0.28f, 0.008f},  {-0.12f, -0.77f},
        {-0.79f, 0.15f}, {0.0f, -0.8f}};  // Julia set parameter points for example images
};
