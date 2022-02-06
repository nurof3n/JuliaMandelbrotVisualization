#include "Graphics.h"

#include "CustomExcept.h"

const sf::RenderWindow& Graphics::getWindow() const { return window; }

sf::RenderWindow& Graphics::getWindow() { return window; }

void Graphics::setup()
{
    window.create(sf::VideoMode::getDesktopMode(), "Julia Explorer", sf::Style::None);
    sf::Image icon;
    if (!icon.loadFromFile("Content/Icon.png"))
        throw EXCEPT("Cannot load file: Content/Icon.png");

    window.setIcon(16, 16, icon.getPixelsPtr());
}

void Graphics::beginFrame() { window.clear(); }

void Graphics::endFrame() { window.display(); }

void Graphics::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
    window.draw(drawable, states);
}

bool Graphics::isInWindow(const sf::Vector2f& pos) const
{
    return (pos.x >= 0 && pos.x < window.getSize().x && pos.y >= 0 && pos.y < window.getSize().y);
}
