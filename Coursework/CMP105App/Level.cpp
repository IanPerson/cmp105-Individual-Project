#include "Level.h"
#include <windows.h>
using namespace std;

Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) {
	srand(time(0));
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;

	// initialise game objects

	// Set size & position of window according to size of computer screen
	window->setSize(sf::Vector2u(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));

	// Limit framerate to 60
	window->setFramerateLimit(60);

	// Disable repeating keys
	window->setKeyRepeatEnabled(false);

	// Disable mouse visibility
	window->setMouseCursorVisible(false);

	// Default states
	isPaused = true;
	currentlyTrading = false;

	// Default position in menu & room vectors
	currentMenu = 0;
	currentRoom = 0;

	// Load font
	font.loadFromFile("font/arial.ttf");

	// Load frame rate text properties
	frameRate.setFont(font);
	frameRate.setFillColor(sf::Color::Black);
	frameRate.setCharacterSize(50);

	// Initialize inventory
	inventory.setWindow(window);
	inventory.setInput(input);
	inventory.initialize();

	// Set initial player position
	player.setPosition({ 2000,2000 });

	// Create animals and push into vector
	for (int i = 0; i < 6; i++) {
		Animal a;
		animals.push_back(a);
	}

	// Add seeds to inventory
	for (int i = 0; i < 2; i++) {
		inventory.addToInventory(i, 5);
	}

	// Door to inside
	farm->addDoor({1150, 1340}, {750, 950}, 0, 1);

	// Door to outside
	home->addDoor({700, 1000}, {1125, 1350}, 0, 0);

	// Set outside tile maps
	farm->addTileMap(3, "Maps/Map1_Path.csv");
	farm->addTileMap(0, "Maps/Map1_Grass.csv");
	farm->addTileMap(1, "Maps/Map1_Hills.csv", "Maps/Map1_Collision.csv");

	// Set inside tile maps
	home->addTileMap(0, "Maps/Map2_Grass.csv", "Maps/Map2_Collision.csv");
	home->addTileMap(5, "Maps/Map2_House.csv");

	// Add objects to rooms & fields vector
	rooms.push_back(farm);
	rooms.push_back(home);

	// Push all fields to vector
	fields.push_back(field1);
	fields.push_back(field2);
	fields.push_back(field3);
	fields.push_back(field4);
	fields.push_back(field5);

	// Load & set texture for house
	houseTexture.loadFromFile("gfx/House.png");
	house.setTexture(&houseTexture);

	// Set camera properties
	camera = window->getView();
	camera.zoom(0.8);

	// Set ui properties
	ui = window->getView();
	ui.zoom(1);
	ui.setSize(sf::Vector2f(window->getSize()));
	ui.setCenter({ window->getSize().x / 2.f, window->getSize().y / 2.f });
	
	// Set inputs
	mouse.setInput(input);
	player.setInput(input);
	family.setInput(input);

	// Set shopkeeper input and window
	shopkeeper.setInput(input);
	shopkeeper.setWindow(window);

	// Create menus and push to vector
	for (int i = 0; i < 8; i++) {
		menus.push_back(new Menu(i));
		menus[i]->setInput(input);
	}

	// Create two save objects
	saves.push_back(new Save);
	saves.push_back(new Save);

	// Add save to both save objects
	saves[0]->addSave(*this);
	saves[1]->addSave(*this);
}

Level::~Level() {

}

// handle user input
void Level::handleInput(float dt) {

	// If escape is pressed, pause game
	if (input->isPressed(sf::Keyboard::Escape) && !isPaused) {
		isPaused = true;
		currentMenu = 1;
	}
	// If escape is pressed while paused and the player is currently in the pause menu, unpause game
	else if (input->isPressed(sf::Keyboard::Escape) && isPaused && currentMenu == 1) {
		isPaused = false;
	}

	// If i is pressed, enter/exit inventory
	if (input->isPressed(sf::Keyboard::I) && !inInventory) {
		inInventory = true;
	}
	else if (input->isPressed(sf::Keyboard::I) && inInventory) {
		inInventory = false;
	}

	// If the game isn't paused, call input functions
	if (!isPaused && !currentlyTrading) {
		player.handleInput(dt);
		rooms[currentRoom]->doorInputs(input, &currentRoom, &player);
		// Call field and animal input functions if outside
		if (currentRoom == 0) {
			for (int i = 0; i < fields.size(); i++) {
				fields[i].handleInput(player, input, camera, &inventory);
			}
			for (int i = 0; i < animals.size(); i++) {
				animals[i].handleInput(input, &player, &inventory);
			}
		}
	}
	
	// If paused, call input function for current menu
	// Otherwise, call shopkeeper's input function if outside
	if (isPaused) {
		menus[currentMenu]->handleInput(&isPaused, &currentMenu, saves[0], saves[1], this);
	} else if (currentRoom == 0) {
		shopkeeper.handleInput(&inventory);
	}
	
	// If not trading, call input function for inventory
	if (!currentlyTrading) {
		inventory.handleInput(inInventory);
	}
}

// Update game objects
void Level::update(float dt) {
	// Update trade state based on shopkeeper's state
	if (shopkeeper.tradeState()) {
		currentlyTrading = true;
	} else {
		currentlyTrading = false;
	}

	// Update inventory
	inventory.update(inInventory, &currentMenu, &isPaused);

	// Update cursor and buttons
	mouse.update(*input);

	// Update size and center of ui according to the size of the window
	ui.setCenter({ window->getSize().x / 2.f, window->getSize().y / 2.f });
	ui.setSize(sf::Vector2f(window->getSize()));

	// Update current room
	rooms[currentRoom]->update(&player, camera, dt);

	// If the game isn't paused, call update functions for appropriate objects
	if (!isPaused) {

		// Stop player while trading, update otherwise
		if (shopkeeper.tradeState()) {
			player.stop();
		} else {
			player.update(dt);
		}
		
		// Update fields
		for (int i = 0; i < fields.size(); i++) {
			fields[i].update(player, camera, dt);
		}

		// Update animals
		for (int i = 0; i < animals.size(); i++) {
			animals[i].update(player, dt);
		}

		// If outside, update outside objects
		if (currentRoom == 0) {
			// Update shopkeeper & check collision
			shopkeeper.update(player);
			shopkeeper.collisionCheck(&player, camera, dt);

			// Update house
			house.update(&player, camera, dt);
		}

		// Update family
		family.update(player, dt, &inventory);

		// Have camera follow player
		camera.setCenter(player.getPosition().x + player.getSize().x / 2, player.getPosition().y + player.getSize().y / 2);

	} else {
		// Update current menu
		menus[currentMenu]->update(mouse);
	}
}

// Render level
void Level::render() {
	beginDraw();

	// Draw environment
	rooms[currentRoom]->draw(window, camera);

	// If outside, draw outside entities
	if (currentRoom == 0) {
		// Draw house
		window->draw(house);

		// Draw fields
		for (int i = 0; i < fields.size(); i++) {
			fields[i].drawCrops(window->getView(), window);
		}
		
		// Draw animals
		for (auto& animal : animals) {
			window->draw(animal);
		}

	// Otherwise draw inside entities
	} else {
		window->draw(family);
		window->draw(family.getText());
	}
	
	// Draw player & shopkeeper in a different order depending on player's y position
	if (player.getPosition().y < shopkeeper.getPosition().y + shopkeeper.getSize().y/3) {
		window->draw(player);
		window->draw(shopkeeper);
	} else {
		window->draw(shopkeeper);
		window->draw(player);
	}	

	// Set view to ui
	window->setView(ui);

	// Draw vendor stock if trading
	if (shopkeeper.tradeState() && !isPaused) {
		shopkeeper.drawStock(window);

	// Otherwise, draw inventory
	} else {
		if (inInventory) {
			window->draw(inventory);
			inventory.drawInv();
		}
		inventory.drawHotbar();
	}

	// Draw menus if paused
	if (isPaused) {
		menus[currentMenu]->draw(window);
	}

	// Draw cursor
	window->draw(mouse);

	//window->draw(frameRate);

	// Reset view to camera
	window->setView(camera);

	endDraw();
}