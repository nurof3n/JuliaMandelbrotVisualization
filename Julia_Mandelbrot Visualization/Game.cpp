#include "Game.h"

#include <conio.h>

#include <algorithm>
#include <cmath>
#include <random>
#include <string>

using namespace std::chrono;

void Game::setup()
{
    // load shader
    if (!sf::Shader::isAvailable())
        throw EXCEPT("Shaders are not available on this system");

    if (!fractalShader.loadFromFile("Shaders/vertex.vert", "Shaders/fragment.frag"))
        throw EXCEPT("Cannot load shaders");

    // set resolution of the shader
    fractalShader.setUniform("Resolution",
                             sf::Vector2f((float) sf::VideoMode::getDesktopMode().width,
                                          (float) sf::VideoMode::getDesktopMode().height));

    // load text font
    if (!font.loadFromFile("Content/cour.ttf"))
        throw EXCEPT("Cannot load file: Content/cour.ttf");

    // initialize the text objects
    textFrametime.setFont(font);
    textControls.setFont(font);
    textFrametime.setFillColor({222, 168, 47});
    textControls.setFillColor({222, 168, 47});
    textControls.setPosition({0.0f, 24.0f});
    textFrametime.setCharacterSize(20);
    textControls.setCharacterSize(20);
    textControls.setString(
        "Press F to toggle frametime\nPress TAB to cycle through color schemes\nPress 0-9 number "
        "keys to go through example sets\nPress U to toggle UBER mode\nPress ESCAPE to "
        "exit\nPress H to hide controls");

    // setup the canvas and render window
    shaderCanvas.createCanvas();
    gfx.setup();

    // set Julia parameter's position inside the window, randomly
    std::random_device                                       dev;
    std::mt19937                                             rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randHeight(
        0, gfx.getWindow().getSize().y - 1);
    std::uniform_int_distribution<std::mt19937::result_type> randWidth(
        0, gfx.getWindow().getSize().x - 1);

    setRedDot(sf::Vector2f((float) randWidth(rng), (float) randHeight(rng)));

    // set the color scheme to default
    setColorScheme(0);

    // initialize member variables
    focused             = true;
    showFrametimeToggle = false;
    showControlsToggle  = true;
    uberModeToggle      = false;
    colorScheme         = 0;
    frametime           = 0.0f;
}

void Game::updateModel()
{
    static auto& window = gfx.getWindow();

    // Handle the event queue
    sf::Event event;
    while (window.pollEvent(event))
        switch (event.type) {
        case sf::Event::Closed: window.close(); return;
        case sf::Event::GainedFocus: setFocused(true); break;
        case sf::Event::LostFocus: setFocused(false); break;
        case sf::Event::KeyPressed:
            // exit
            if (isFocused() && event.key.code == sf::Keyboard::Escape) {
                window.close();
                return;
            }
            // toggle FPS
            else if (event.key.code == sf::Keyboard::F)
                toggleFrametime();
            // switch color scheme
            else if (event.key.code == sf::Keyboard::Tab) {
                setNextColorScheme();
            }
            // toggle controls
            else if (event.key.code == sf::Keyboard::H)
                toggleControls();
            // change example image
            else if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
                setExampleImage(event.key.code - sf::Keyboard::Num0);
            }
            // toggle UBER
            else if (event.key.code == sf::Keyboard::U) {
                toggleUberMode();
            }

            break;
        }

    // step out if out of focus
    if (!isFocused())
        return;

    // set the uniforms
    // update mouse position only if LMB is pressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
        && gfx.isInWindow(sf::Vector2f(sf::Mouse::getPosition(gfx.getWindow())))) {
        fractalShader.setUniform("IsLMBPressed", true);
        setRedDot(sf::Vector2f(sf::Mouse::getPosition(gfx.getWindow())));
    }
    else
        fractalShader.setUniform("IsLMBPressed", false);
}

void Game::composeFrame()
{
    // draw the canvas and apply the shader
    gfx.draw(shaderCanvas.getSprite(), &fractalShader);

    // show toggled elements
    if (showFrametimeToggle)
        gfx.draw(textFrametime);
    if (showControlsToggle)
        gfx.draw(textControls);
}

void Game::go()
{
    // render only if in focus
    if (isFocused())
        gfx.beginFrame();

    // update game state (even if out of focus, to handle the event queue)
    updateModel();

    // render only if in focus
    if (isFocused()) {
        composeFrame();
        gfx.endFrame();
    }

    // compute frametime
    measureFrametime();
    textFrametime.setString("Frametime: " + std::to_string(frametime) + " ms");
}

void Game::setFrametimeToggle(bool toggle) { showFrametimeToggle = toggle; }

void Game::toggleFrametime() { setFrametimeToggle(!showFrametimeToggle); }

void Game::setControlsToggle(bool toggle) { showControlsToggle = toggle; }

void Game::toggleControls() { setControlsToggle(!showControlsToggle); }

void Game::setUberModeToggle(bool toggle)
{
    uberModeToggle = toggle;
    fractalShader.setUniform("UBER", uberModeToggle);
}

void Game::toggleUberMode() { setUberModeToggle(!uberModeToggle); }

bool Game::isFocused() const { return focused; }

void Game::setFocused(bool focus) { focused = focus; }

const Graphics& Game::getGraphics() const { return gfx; }

Graphics& Game::getGraphics() { return gfx; }

void Game::setColorScheme(int index)
{
    if (index < 0 || index >= COLOR_SCHEME_COUNT)
        throw EXCEPT("Index out of bounds");

    colorScheme = index;
    fractalShader.setUniform("ColorScheme", colorScheme);
}

void Game::setNextColorScheme() { setColorScheme((colorScheme + 1) % COLOR_SCHEME_COUNT); }

void Game::setExampleImage(int index)
{
    if (index < 0 || index >= IMAGE_EXAMPLES_COUNT)
        throw EXCEPT("Index out of bounds");

    fractalShader.setUniform("RedDotPos", juliaParameters[index]);
    fractalShader.setUniform("IsExample", true);
}

void Game::setRedDot(sf::Vector2f pos)
{
    fractalShader.setUniform("RedDotPos", pos);
    fractalShader.setUniform("IsExample", false);
}

void Game::measureFrametime() { frametime = frameTimer.mark() / 1e6f; }
