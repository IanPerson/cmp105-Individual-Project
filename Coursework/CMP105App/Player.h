#pragma once 
#include "Framework/GameObject.h"
#include "Framework/Input.h"
#include "Framework/Animation.h"

class Player : public GameObject, public Input {
	private:
		// Variables

		// Movement vectors
		sf::Vector2f direction;
		sf::Vector2f speed;

		// Animations
		Animation idle;
		Animation walk;
		Animation* currentAnimation;

		// Display for collision box - testing only
		sf::RectangleShape hitbox;

		// Textures for animations
		sf::Texture playerIdle;
		sf::Texture playerWalk;

		// Tracks current active collisions
		int currentCollisions;

	public:
		// Constructor and deconstructor
		Player();
		~Player();
		
		// handleInput function
		// 
		void handleInput(float dt);
		void update(float dt);

		// Stop function
		// Sets direction vectors to 0 and resets to idle animation
		void stop();

		// setDirection function
		// Set direction vector according to x & y parameters
		void setDirection(float x, float y);

		// getDirection function
		// Returns direction vector
		sf::Vector2f getDirection();

		// setCurrentCollision function
		// Set currentCollision variable according to parameter
		void setCurrentCollisions(int amount);

		// getCurrentCollision function
		// Return currentCollision variable
		int getCurrentCollisions();

		// collisionResponse function
		// Updates position of player based on GameObject parameter
		void collisionResponse(GameObject* o);

		// Draw function
		// Draws the player (and hitbox if enabled) to the given window
		void draw(sf::RenderWindow* w);
};