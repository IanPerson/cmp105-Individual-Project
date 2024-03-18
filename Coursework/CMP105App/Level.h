#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "Vector"
#include "Room.h"
#include "Button.h"
#include "Field.h"
#include "Diagonal.h"
#include "Vendor.h"
#include "Animal.h"
#include "Family.h"
#include "Save.h"
#include "Menu.h"

class Level : BaseLevel {
public:
	Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Level();

	void handleInput(float dt) override;
	void update(float dt);
	void render();

private:
	// Default functions for rendering to the screen.
	

	// Default variables for level class.

	// Vectors for objects
	std::vector<Save*> saves;
	std::vector<Room*> rooms;
	std::vector<Field> fields;
	std::vector<Menu*> menus;
	std::vector<Animal> animals;

	// Current indexes for rooms and menu vectors, determines what is drawn and updated
	int currentRoom;
	int currentMenu;

	// States for pausing, trading, and using the inventory
	bool isPaused;
	bool currentlyTrading;
	bool inInventory;

	// Create controllable objects
	Player player;
	Cursor mouse;

	// Create inventory
	Inventory inventory{ 5, 5 };

	// Collision object to use as house
	Wall house{ {1000,1000}, {0,0}, {400,400} };
	sf::Texture houseTexture;

	// Create family
	Family family{ 500,500 };

	// Create vendor
	Vendor shopkeeper{2};

	// Create rooms
	Room* farm = new Room{ {4000, 4000} };
	Room* home = new Room{ {1500, 1000} };

	// Create fields
	Field field1{ 600, 2200, 4, 7 };
	Field field2{ 1600, 2200, 4, 7 };
	Field field3{ 2600, 2200, 4, 7 };
	Field field4{ 1100, 2800, 4, 7 };
	Field field5{ 2100, 2800, 4, 7 };

	// Text for frame rate - testing only
	sf::Text frameRate;
	sf::Font font;

	// Moving view - for player & room
	sf::View camera;

	// Static view - for UI
	sf::View ui;
};