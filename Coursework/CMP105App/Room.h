#pragma once

#include "Framework/GameObject.h"
#include "Framework/Input.h"
#include "Player.h"
#include "Door.h"
#include <String>
#include "tiles_Map.h"

class Room : public GameObject {
	private:
		
		// Determines area where player can move
		sf::RectangleShape playerArea;

		std::vector<tiles_Map*> tileMaps;

		// Textures
		sf::Texture backgroundTexture;
		sf::Texture roomTexture;

		// Vector variable containing all current door objects
		// Can create and destroy door objects with few issues
		std::vector<Door> allDoors;

	public:
		// Constructor
		Room(sf::Vector2f areaSize);

		// FUNCTIONS

		// Get traversable area
		sf::RectangleShape getArea();

		// Draws all objects in room
		void draw(sf::RenderWindow* w, sf::View v);

		// update() - see room.cpp for info
		void update(Player *player, sf::View v, float dt);

		// Get input for doors
		void doorInputs(Input* input, int* currentRoom, Player* player);

		// addDoor() - see room.cpp for info
		void addDoor(sf::Vector2f position, sf::Vector2f moveCoords, float rotation, int moveTo);

		// loadTexture() - see room.cpp for info
		void loadTexture(sf::Texture texture);

		// addTileMap() - see room.cpp for info
		void addTileMap(int id, const std::string& map);
		void addTileMap(int id, const std::string& map, const std::string& collision);
};