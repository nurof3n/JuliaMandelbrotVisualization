#include "Game.h"
#include "CustomExcept.h"


/**
 * @brief This is the main function, where the program loop happens
 *
*/
int main() {
	try {
		Game& game = Game::getInstance();
		game.setup();
		sf::RenderWindow& window = game.gfx.getWindow();

		while (window.isOpen()) {
			game.go();
		}
	}
	catch (const CustomExcept& e) {
		MessageBoxA(nullptr, e.what(), "Exception thrown", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Exception thrown", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBoxA(nullptr, "Unknown exception.", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return 0;
}