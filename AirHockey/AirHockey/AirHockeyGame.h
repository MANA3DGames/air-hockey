#ifndef AIR_HOCKEY_GAME_H
#define AIR_HOCKEY_GAME_H

#include <windows.h>
#include "Sprite.h"
#include "Rect.h"
#include "Circle.h"

namespace MANA3DGames
{
	class AirHockeyGame
	{
	public:
		// Constructor & Destructor.
		AirHockeyGame(HINSTANCE hAppInst, HWND hMainWnd, Vec2 wndCenterPt);
		~AirHockeyGame();

		// Pause and unpause the game.
		void pause();
		void unpause();

		// This method updates all of the game objects; essentially it calls all of the private “helper” update methods.
		void update(float dt);

		// This method draws all the sprites to the backbuffer, and it also draws the current player and opponent scores to the backbuffer.
		void draw(HDC hBackBufferDC, HDC hSpriteDC);

	private:
		// This method updates the player paddle’s position after dt seconds have passed. 
		// It computes the current mouse velocity and updates the position accordingly.
		// In addition, it also ensures that the player paddle stays within its bounds which is inbounds(18.2.1.5).
		void updatePlayerPaddle(float dt);

		// This method updates the opponent paddle’s position after dt seconds have passed using the AI algorithm 
		// it also ensures that the opponent paddle stays within its bounds which is inbounds(18.2.1.5).
		void updateOpponentPaddle(float dt);

		// This method updates the puck’s position after dt seconds have passed. 
		// It computes the reflected velocity if the puck hits a wall(18.2.1.4).
		// It also detects paddle - puck collisions, and if there is a collision then it computes the new velocity of the puck after the collision(18.2.1.3).
		// In addition to making sure the puck stays within the game board boundaries, 
		// it also checks to see if the puck’s center made it into one of the goal boxes(18.2.1.7); i.e., did a player score a goal ?
		void updatePuck(float dt);

		// This method is called internally by updatePuck. This method actually performs the mathematical calculations of the paddle - puck collision(18.2.1.3).
		// Note that this function takes a pointer to the sprite representing the paddle as a parameter.
		// In this way, the function can be called for either the player or oppoenet paddle.
		bool paddlePuckCollision(Sprite* paddle);

		// This method simply increases the score of the player if playerScroe is true, otherwise it increases the score of the opponent.
		// Furthermore, after increasing the score this method resets the puck to the middle of the game board.
		// And will increase the difficulty of the game by increasing the speed of the opponent
		void increaseScore(bool playerScroe);

	private:
		HINSTANCE hAppInst;				// A handle to the application instance.
		HWND hMainWnd;					// A handle to the main window.
		Vec2 wndCenterPt;				// Specifies the center point of the window’s client area.

		int playerScore;				// Keeps track of how many points player has scored.
		int opponentScore;				// Keeps track of how many points AI player has scored.

		bool isPaused;					// Indicates if the game is currently paused.

		float opponentRecoverTime;		// Stores the remaining time before the AI player paddle can move again.
		const float MAX_PUCK_SPEED;		// Specifies the maximum speed the puck can move.
		//const float OPPONENT_SPEED;		// Specifies the speed at which the AI player moves the paddle.
		float opponentSpeed;			// Specifies the speed at which the AI player moves the paddle.

		Sprite *gameBoardSprite;		// The sprite that represents the game board graphic.
		Sprite *playerPaddleSprite;		// The sprite that represents the player paddle graphic.
		Sprite *opponentPaddleSprite;	// The sprite that represents the opponent paddle graphic.
		Sprite *puckSprite;				// The sprite that represents the puck graphic.

		POINT lastMousePos;				// Used to store the mouse cursor position from the previous frame.
		POINT currMousePos;				// Used to store the mouse cursor of the current frame.

		Rect playerBounds;				// A rectangle describing the blue side.
		Rect opponentBounds;			// A rectangle describing the red side.
		Rect boardBounds;				// A rectangle describing the entire game board.
		Rect playerGoal;				// A rectangle describing the player side goal box.
		Rect opponenetGoal;				// A rectangle describing the red side goal box.

		HFONT scoreFornt;				// will create a new font for score.
	};

}

#endif // AIR_HOCKEY_GAME_H