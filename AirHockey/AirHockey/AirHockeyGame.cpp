#pragma comment(lib, "winmm.lib")

#define _CRT_SECURE_NO_WARNINGS

#include "AirHockeyGame.h"
#include <cstdio>
#include "resource.h"

using namespace MANA3DGames;

#define PADDLE_PUCK_COLLISION_SFX "Data/Sounds/paddlePuck_SFX.wav"
#define PUCK_WALL_COLLISION_SFX "Data/Sounds/puckWall_SFX.wav"
#define PLAYER_SCORE_SFX "Data/Sounds/start_SFX.wav"
#define OPPONENT_SCORE_SFX "Data/Sounds/error_SFX.wav"



AirHockeyGame::AirHockeyGame( HINSTANCE a_hAppInst, HWND a_hMainWnd, Vec2 a_wndCenterPt )
: MAX_PUCK_SPEED(7.0f), opponentSpeed(3.0f)
{
	// Save input parameters.
	hAppInst = a_hAppInst;
	hMainWnd = a_hMainWnd;
	wndCenterPt = a_wndCenterPt;

	// Players start game with score of zero.
	playerScore = 0;
	opponentScore = 0;

	// The game is initially paused.
	isPaused = true;

	// No recovery time for oppoenet to start.
	opponentRecoverTime = 0.0f;

	// Create the sprites:
	Circle boundingCircle;
	Vec2 initialPos = wndCenterPt;
	Vec2 initialVelocity( 0.0f, 0.0f );
	//gameBoardSprite = new Sprite( hAppInst, IDB_GAMEBOARD, IDB_GAMEBOARD_MASK, boundingCircle, initialPos, initialVelocity );
	gameBoardSprite = new Sprite(hAppInst, IDB_GAMEBOARD, boundingCircle, initialPos, initialVelocity);

	boundingCircle.center = initialPos;
	boundingCircle.radius = 18.0f; // Puck radius = 18
	puckSprite = new Sprite( hAppInst, IDB_PUCK, IDB_PUCK_MASK, boundingCircle, initialPos, initialVelocity );

	initialPos.x = 700;
	initialPos.y = 200;
	boundingCircle.center = initialPos;
	boundingCircle.radius = 25.0f; // Paddle radius = 25
	opponentPaddleSprite = new Sprite( hAppInst, IDB_OPPONENT_PADDLE, IDB_PADDLE_MASK, boundingCircle, initialPos, initialVelocity );

	initialPos.x = 100;
	initialPos.y = 100;
	boundingCircle.center = initialPos;
	boundingCircle.radius = 25.0f; // Paddle radius = 25
	playerPaddleSprite = new Sprite( hAppInst, IDB_PLAYER_PADDLE, IDB_PADDLE_MASK, boundingCircle, initialPos, initialVelocity );

	// Initialize the rectangles.
	playerBounds = Rect( 7, 40, 432, 463 );
	opponentBounds = Rect( 432, 40, 854, 463 );
	boardBounds = Rect( 7, 40, 854, 463 );
	playerGoal = Rect( 0, 146, 25, 354 );
	opponenetGoal = Rect( 838, 146, 863, 354 );

	// Create a new font for the Score.
	scoreFornt = CreateFont( 25, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "SYSTEM_FIXED_FONT" );

	/*char txt[64];
	sprintf(txt, " %f ", puckSprite->boundingCircle.center.y);
	MessageBox(0, txt, "Error", MB_OK);*/
}

AirHockeyGame::~AirHockeyGame()
{
	DeleteObject(scoreFornt);
	delete gameBoardSprite;
	delete playerPaddleSprite;
	delete opponentPaddleSprite;
	delete puckSprite;
}


void AirHockeyGame::pause()
{
	// Set isPaused to true.
	isPaused = true;

	// Currently, the game is unpaused--release capture on mouse.
	ReleaseCapture();

	// Show the mouse cursor when paused.
	ShowCursor( true );
}

void AirHockeyGame::unpause()
{
	// --- Fix cursor to paddle position ---
	// 1. Get the current pos of the player paddle.
	POINT pos = playerPaddleSprite->position;
	// 2. Convert the client-area coordinates of the pos point to screen coordinates.
	ClientToScreen( hMainWnd, &pos );
	// 3. Set the Cursor to pos.
	SetCursorPos( pos.x, pos.y );
	// 4. Save the current Cursor pos in the lastMousePos.
	GetCursorPos( &lastMousePos );

	// Set isPaused to false.
	isPaused = false;

	// Capture the mouse when not paused.
	SetCapture( hMainWnd );

	// Hide the mouse cursor when not paused.
	ShowCursor( false );

	// Play unpause SFX.
	PlaySound( "Data/Sounds/click_SFX.wav", NULL, SND_FILENAME | SND_ASYNC );
}


void AirHockeyGame::update( float dt )
{
	// Only update the game if the game is not paused.
	if ( !isPaused )
	{
		// Call all update functions.
		updatePlayerPaddle( dt );
		updateOpponentPaddle( dt );
		updatePuck( dt );

		// Decrease recovery time as time passes.
		if ( opponentRecoverTime > 0.0f )
			opponentRecoverTime -= dt;
	}
}

void AirHockeyGame::draw( HDC hBackBufferDC, HDC hSpriteDC )
{
	// Draw the sprites.
	gameBoardSprite->draw( hBackBufferDC, hSpriteDC );
	playerPaddleSprite->draw( hBackBufferDC, hSpriteDC );
	opponentPaddleSprite->draw( hBackBufferDC, hSpriteDC );
	puckSprite->draw( hBackBufferDC, hSpriteDC );

	HFONT hTmp = (HFONT)SelectObject( hBackBufferDC, scoreFornt );
	//SetBkMode(hBackBufferDC, TRANSPARENT);

	// Draw the player scores.
	char score[32];
	SetTextColor(hBackBufferDC, RGB(255, 255, 255));
	sprintf( score, "Player1's score : %d", playerScore );
	SetBkMode( hBackBufferDC, TRANSPARENT );
	TextOut( hBackBufferDC, 15, 45, score, (int)strlen( score ) );
	sprintf( score, "Player2's score : %d", opponentScore );
	TextOut( hBackBufferDC, 600, 45, score, (int)strlen( score ) );

	// If the game is paused -> display help text.
	if ( isPaused )
	{
		SetTextColor( hBackBufferDC, RGB( 0, 255, 0 ) );
		TextOut( hBackBufferDC, wndCenterPt.x - 320, 420, "Left click to Resume Game", (int)strlen( "Left click to resume game" ) );
		TextOut(hBackBufferDC, wndCenterPt.x + 50, 420, "Right click to Pause Game", (int)strlen("Right click to pause game"));
	}

	SelectObject( hBackBufferDC, hTmp );
}


void AirHockeyGame::updatePlayerPaddle( float dt )
{
	// Get current cursor position.
	GetCursorPos( &currMousePos );

	// Change in mouse position.
	int deltaX = currMousePos.x - lastMousePos.x;
	int deltaY = currMousePos.y - lastMousePos.y;
	Vec2 deltaPos( (float)deltaX, (float)deltaY );

	// Velocity is change in position with respect to time.
	playerPaddleSprite->velocity = deltaPos / dt;

	// Update the player paddle's position.
	playerPaddleSprite->update( dt );

	// Make sure the player paddle stays inbounds.
	playerBounds.forceInside( playerPaddleSprite->boundingCircle );
	playerPaddleSprite->position = playerPaddleSprite->boundingCircle.center;

	// The current position is now the last mouse position.
	lastMousePos = playerPaddleSprite->position;

	// Keep mouse cursor fixed to paddle.
	ClientToScreen( hMainWnd, &lastMousePos );
	SetCursorPos( lastMousePos.x, lastMousePos.y );
}

void AirHockeyGame::updateOpponentPaddle( float dt )
{
	// Simple AI
	// When the puck moves into opponent's boundary, the opponent paddle simply moves directly towards the puck to hit it. 
	// When the puck leaves opponent's boundaries, the opponent paddle returns to the center of its boundary.
	if ( opponentRecoverTime <= 0.0f )
	{
		// Is the puck in opponent's boundary? If yes, then move the opponent paddle directly toward the puck.
		if ( opponentBounds.isPtInside( puckSprite->position ) )
		{
			// Vector directed from paddle to puck.
			Vec2 paddleVel = puckSprite->position - opponentPaddleSprite->position;
			paddleVel.normalize();
			paddleVel *= opponentSpeed;
			opponentPaddleSprite->velocity = paddleVel;
		}
		// If no, then move the red paddle to the center of its boundary --> point (700, 200).
		else
		{
			// Vector directed from paddle to center of the boundary.
			Vec2 paddleVel = Vec2(700, 200) - opponentPaddleSprite->position;

			if ( paddleVel.length() > 5.0f )
			{
				paddleVel.normalize();
				paddleVel *= opponentSpeed;
				opponentPaddleSprite->velocity = paddleVel;
			}
			// Within 5 units--close enough.
			else
				opponentPaddleSprite->velocity = Vec2( 0.0f, 0.0f );
		}

		// Update the opponent paddle's position.
		opponentPaddleSprite->update( dt );

		// Make sure the opponent paddle stays inbounds.
		opponentBounds.forceInside( opponentPaddleSprite->boundingCircle );
		opponentPaddleSprite->position = opponentPaddleSprite->boundingCircle.center;
	}
}

void AirHockeyGame::updatePuck( float dt )
{
	// Check for goal!
	if (opponenetGoal.isPtInside(puckSprite->position))
		increaseScore(true);
	if (playerGoal.isPtInside(puckSprite->position))
		increaseScore(false);

	// Check for player's Paddle and puck collision.
	paddlePuckCollision( playerPaddleSprite );

	// If opponent paddle hits the puck then make a small 1/10th of a second delay 
	// before the opponent paddle can move away as sort of a "recovery period" after the hit. 
	// This is to model the fact that when a human player hits something, it takes a short period of time to recover from the collision.
	if ( paddlePuckCollision( opponentPaddleSprite ) )
		opponentRecoverTime = 0.1f;

	// Clamp puck speed to some maximum velocity; this provides good stability.
	if ( puckSprite->velocity.length() >= MAX_PUCK_SPEED )
		puckSprite->velocity.normalize() *= MAX_PUCK_SPEED;

	// Did the puck hit a wall? If so, reflect about edge.
	Circle puckCircle = puckSprite->boundingCircle;
	if (puckCircle.center.x - puckCircle.radius < boardBounds.minPt.x)
	{
		// Play paddlePuckCollision SFX.
		PlaySound(PUCK_WALL_COLLISION_SFX, NULL, SND_FILENAME | SND_ASYNC);
		puckSprite->velocity.x *= -1.0f;
	}
	if (puckCircle.center.x + puckCircle.radius > boardBounds.maxPt.x)
	{
		// Play paddlePuckCollision SFX.
		PlaySound(PUCK_WALL_COLLISION_SFX, NULL, SND_FILENAME | SND_ASYNC);
		puckSprite->velocity.x *= -1.0f;
	}	
	if (puckCircle.center.y - puckCircle.radius < boardBounds.minPt.y)
	{
		// Play paddlePuckCollision SFX.
		PlaySound(PUCK_WALL_COLLISION_SFX, NULL, SND_FILENAME | SND_ASYNC);
		puckSprite->velocity.y *= -1.0f;
	}
	if (puckCircle.center.y + puckCircle.radius > boardBounds.maxPt.y)
	{
		// Play paddlePuckCollision SFX.
		PlaySound(PUCK_WALL_COLLISION_SFX, NULL, SND_FILENAME | SND_ASYNC);
		puckSprite->velocity.y *= -1.0f;
	}

	// Make sure puck stays inbounds of the gameboard.
	boardBounds.forceInside( puckSprite->boundingCircle );

	puckSprite->position = puckSprite->boundingCircle.center;
	puckSprite->update( dt );
}


bool AirHockeyGame::paddlePuckCollision( Sprite* paddle )
{
	Vec2 normal;

	// Check if there is a collision between the paddle and the puck.
	if ( paddle->boundingCircle.hits( puckSprite->boundingCircle, normal ) )
	{
		// Play paddlePuckCollision SFX.
		PlaySound( PADDLE_PUCK_COLLISION_SFX, NULL, SND_FILENAME | SND_ASYNC );

		// Note: hits() updates circle's position. So update pucks position as well since the two correspond.
		puckSprite->position = puckSprite->boundingCircle.center;


		// Compute the paddle's velocity relative to the puck's velocity.
		Vec2 relVel = paddle->velocity - puckSprite->velocity;

		// Get the component of the relative velocity along the normal.
		float impulseMag = relVel.dot( normal );

		// Are the objects getting closer together?
		if ( impulseMag >= 0.0f )
		{
			// Project the relative velocity onto the normal.
			Vec2 impulse = normal * impulseMag;

			// Add the impulse to the puck.
			puckSprite->velocity += impulse * 2.0f;

			return true;
		}
	}
	return false;
}


void AirHockeyGame::increaseScore( bool pScore )
{
	// Is it a score for the player?
	if ( pScore )
	{
		++playerScore;
		PlaySound( PLAYER_SCORE_SFX, NULL, SND_FILENAME | SND_SYNC );
		Sleep(500);
	}
	else
	{
		++opponentScore;
		PlaySound(OPPONENT_SCORE_SFX, NULL, SND_FILENAME | SND_SYNC);
		Sleep(500);

		// Increase opponenet's speed.
		opponentSpeed += 0.1f;
	}

	// A point was just scored, so reset puck to center and pause game.
	puckSprite->position = Vec2( wndCenterPt.x, wndCenterPt.y );
	puckSprite->velocity = Vec2( 0.0f, 0.0f );
	puckSprite->boundingCircle.center = Vec2( wndCenterPt.x, wndCenterPt.y );

	// After score, pause the game so player can prepare for next round.
	pause();
}