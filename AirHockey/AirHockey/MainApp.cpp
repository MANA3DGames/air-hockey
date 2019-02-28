#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include "resource.h"
#include "AirHockeyGame.h"
#include "TitleScreen.h"
#include "BackBuffer.h"
using namespace MANA3DGames;
using namespace std;

#define START_NEW_GAME_SFX "Data/Sounds/start_SFX.wav"



//==================================================================================================================
// MainApp: Global variables
//==================================================================================================================
HWND ghMainWnd = 0;
HINSTANCE ghAppInst = 0;
HMENU ghMainMenu = 0;
HDC ghSpriteDC = 0;
BackBuffer* gBackBuffer = 0;
AirHockeyGame* gAirHockey = 0;
TitleScreen* gTitleScreen = 0;
string gWndCaption = "Air Hockey Game";

// Client dimensions exactly equal dimensions of background bitmap.
const int gClientWidth = 864;
const int gClientHeight = 504;

// Center point of client rectangle.
const POINT gClientCenter =
{
	gClientWidth / 2,
	gClientHeight / 2
};

// Pad window dimensions so that there is room for window borders, caption bar, and menu.
const int gWindowWidth = gClientWidth + 6;
const int gWindowHeight = gClientHeight + 52;


//==================================================================================================================
// MainApp: Enums
//==================================================================================================================
enum GameState
{
	GAME_STATE_TITLE, 
	GAME_STATE_GAME
};

// Current game state.
int gGameState = GAME_STATE_TITLE;




//==================================================================================================================
// MainApp: Function Prototypes
//==================================================================================================================
bool InitMainWindow();
int Run();
void DrawFramesPerSecond( float deltaTime );
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK AboutBoxProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
void StartNewGame();


//==================================================================================================================
// MainApp: Function Implementations
//==================================================================================================================
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		// Create application resources.
	case WM_CREATE:
		// Create the hockey game.
		gAirHockey = new AirHockeyGame( ghAppInst, hWnd, gClientCenter );
		// Create the Title Screen
		gTitleScreen = new TitleScreen( ghAppInst, hWnd, gClientCenter );
		// Create system memory DCs
		ghSpriteDC = CreateCompatibleDC(0);
		// Create the backbuffer.
		gBackBuffer = new BackBuffer( hWnd, gClientWidth, gClientHeight );
		return 0;
	case WM_COMMAND:
		switch ( LOWORD( wParam ) )
		{
			// Destroy the window when the user selects the 'exit' menu item.
		case ID_FILE_EXIT:
			DestroyWindow( ghMainWnd );
			break;
			// Display the about dialog box when the user selects the 'about' menu item.
		case ID_HELP_ABOUT:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_ABOUTBOX),ghMainWnd, AboutBoxProc);
			break;
		}
		return 0;
		// Left mouse button to unpause the game.
	case WM_LBUTTONDOWN:
		if (gGameState == GAME_STATE_TITLE)
			StartNewGame();
		else if (gGameState == GAME_STATE_GAME)
			gAirHockey->unpause();
		return 0;
		// Right mouse button to pause the game.
	case WM_RBUTTONDOWN:
		if (gGameState == GAME_STATE_TITLE)
			StartNewGame();
		else if (gGameState == GAME_STATE_GAME)
			gAirHockey->pause();
		return 0;
		// Destroy application resources.
	case WM_DESTROY:
		delete gAirHockey;
		delete gTitleScreen;
		delete gBackBuffer;
		DeleteDC( ghSpriteDC );
		PostQuitMessage(0);
		return 0;
	}

	// Forward any other messages we didn't handle to the default window procedure.
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

BOOL CALLBACK AboutBoxProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch ( LOWORD( wParam ) )
		{
		// Terminate the dialog when the user presses the OK button.
		case IDOK:
			EndDialog( hDlg, 0 );
			break;
		}
		return true;
	}
	return false;
}


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR cmdLine, int showCmd )
{
	ghAppInst = hInstance;

	// Create the main window.
	if ( !InitMainWindow() )
	{
		MessageBox( 0, "Window Creation Failed.", "Error", MB_OK );
		return 0;
	}

	// Enter the message loop.
	return Run();
}

bool InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ghAppInst;
	wc.hIcon = ::LoadIcon( 0, IDI_APPLICATION );
	wc.hCursor = ::LoadCursor( 0, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)::GetStockObject( NULL_BRUSH );
	wc.lpszMenuName = 0;
	wc.lpszClassName = "MyWndClassName";

	RegisterClass( &wc );

	// WS_OVERLAPPED | WS_SYSMENU: Window cannot be resized and does not have a min/max button.
	ghMainMenu = LoadMenu( ghAppInst, MAKEINTRESOURCE( IDR_MENU ) );
	ghMainWnd = ::CreateWindow( "MyWndClassName", gWndCaption.c_str(), 
								WS_OVERLAPPED | WS_SYSMENU, 
								200, 200, gWindowWidth, gWindowHeight, 
								0, ghMainMenu, ghAppInst, 0 );
	
	if ( ghMainWnd == 0 )
	{
		::MessageBox( 0, "CreateWindow - Failed", 0, 0 );
		return 0;
	}

	ShowWindow( ghMainWnd, SW_NORMAL );
	UpdateWindow( ghMainWnd );

	return true;
}

int Run()
{
	MSG msg;
	ZeroMemory( &msg, sizeof( MSG ) );

	// Get the current time.
	float lastTime = (float)timeGetTime();
	float timeElapsed = 0.0f;

	while ( msg.message != WM_QUIT )
	{
		// IF there is a Windows message then process it.
		if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		// ELSE, do game stuff.
		else
		{
			// Get the time now.
			float currTime = (float)timeGetTime();

			// Compute the differences in time from the last time we checked. Since the last time we checked was the previous loop iteration, this difference
			// gives us the time between loop iterations... or, I.e., the time between frames.
			float deltaTime = max( ( currTime - lastTime ) * 0.001f, 0.0f );
			
			timeElapsed += deltaTime;

			// Only update once every 1/100 seconds.
			if ( timeElapsed >= 0.01 )
			{
				if (gGameState == GAME_STATE_TITLE)
				{
					// Update title screen.
					gTitleScreen->update((float)timeElapsed);
					// Draw Title screen.
					gTitleScreen->draw( gBackBuffer->getDC(), ghSpriteDC );
				}
				else if (gGameState == GAME_STATE_GAME)
				{
					// Update the game and draw everything.
					gAirHockey->update((float)timeElapsed);
					// Draw every frame.
					gAirHockey->draw(gBackBuffer->getDC(), ghSpriteDC);
				}

				// Draw frame rate.
				DrawFramesPerSecond( timeElapsed );

				// Now present the backbuffer contents to the main window client area.
				gBackBuffer->present();

				// Zero out timeElapsed.
				timeElapsed = 0.0;
			}

			// We are at the end of the loop iteration, so prepare for the next loop iteration by making the "current time" the "last time."
			lastTime = currTime;
		}
	}

	// Return exit code back to operating system.
	return (int)msg.wParam;
}

void DrawFramesPerSecond( float deltaTime )
{
	// Make static so the variables persist even after the function returns.
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	static char buffer[256];

	// Function called implies a new frame, so increment the frame count.
	++frameCnt;

	// Also increment how much time has passed since the last frame.
	timeElapsed += deltaTime;

	// Has one second passed?
	if (timeElapsed >= 1.0f)
	{
		// Yes, so compute the frames per second.
		// FPS = frameCnt / timeElapsed, but since we compute only when timeElapsed = 1.0, 
		// we can reduce to:
		// FPS = frameCnt / 1.0 = frameCnt.
		sprintf( buffer, "--Frames Per Second = %d", frameCnt );

		// Add the frames per second string to the main window caption--that is, we'll display the frames per second in the window's caption bar.
		string newCaption = gWndCaption + buffer;

		// Now set the new caption to the main window.
		SetWindowText(ghMainWnd, newCaption.c_str());

		// Reset the counters to prepare for the next time we compute the frames per second.
		frameCnt = 0;
		timeElapsed = 0.0f;
	}
}

void StartNewGame()
{
	// Play start game. [Sync mode]
	PlaySound( START_NEW_GAME_SFX, NULL, SND_FILENAME | SND_SYNC );
	Sleep(1000);
	// Set the current state to Gameplay.
	gGameState = GAME_STATE_GAME;
}
