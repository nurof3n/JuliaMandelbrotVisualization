#include "Graphics.h"
#include "CustomExcept.h"

Graphics& Graphics::getInstance() noexcept {
	static Graphics _instance;
	return _instance;
}

sf::RenderWindow& Graphics::getWindow() noexcept {
	return window;
}
// creates window
void Graphics::setup() {
	window.create( sf::VideoMode::getDesktopMode(), "Julia Explorer", sf::Style::None );
	sf::Image icon;
	if( !icon.loadFromFile( "Content/Icon.png" ) )
		throw EXCEPT( "Cannot load file: Content/Icon.png" );
	window.setIcon( 16, 16, icon.getPixelsPtr() );
}
// stuff to do before drawing to the buffer
void Graphics::beginFrame() {
	window.clear();
}
// stuff to do after drawing to the buffer
void Graphics::endFrame() {
	window.display();
}
// function that takes an object that can be drawn and some state (e.g. shader) to apply, and draws it
void Graphics::draw( const sf::Drawable& drawable, const sf::RenderStates& states ) {
	window.draw( drawable, states );
}

bool Graphics::isInWindow( const sf::Vector2f& pos ) const {
	return (pos.x >= 0 && pos.x <= window.getSize().x - 1 && pos.y >= 0 && pos.y <= window.getSize().y - 1);
}
