#include "TitleScreen.h"
#include "resource.h"
using namespace MANA3DGames;

TitleScreen::TitleScreen(HINSTANCE a_hAppInst, HWND a_hMainWnd, Vec2 a_wndCenterPt)
{
	wndCenterPt = a_wndCenterPt;

	Circle boundingCircle;
	Vec2 initialPos = wndCenterPt;
	Vec2 initialVelocity(0.0f, 0.0f);

	titleImage = new Sprite(a_hAppInst, IDB_TITLE, boundingCircle, initialPos, initialVelocity);

	titleFont = CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "SYSTEM_FIXED_FONT");

	clickTxtTimer = 0;
	displayClickTxt = false;
}

TitleScreen::~TitleScreen()
{
	DeleteObject(titleFont);
	delete titleImage;
}


void TitleScreen::update(float dt)
{
	if ( clickTxtTimer > 0.0f)
		clickTxtTimer -= dt;
	else
	{
		displayClickTxt = !displayClickTxt;
		clickTxtTimer = 0.5f;
	}
}

void TitleScreen::draw(HDC hBackBufferDC, HDC hSpriteDC)
{
	// Draw the sprites.
	titleImage->draw(hBackBufferDC, hSpriteDC);


	if (displayClickTxt)
	{
		HFONT hTmp = (HFONT)SelectObject(hBackBufferDC, titleFont);
		//SetBkMode(hBackBufferDC, TRANSPARENT);

		SetTextColor(hBackBufferDC, RGB(255, 255, 255));
		SetBkMode(hBackBufferDC, TRANSPARENT);
		TextOut(hBackBufferDC, wndCenterPt.x - 80, wndCenterPt.y + 150, "Click to Play", (int)strlen("Click to Play"));

		SelectObject(hBackBufferDC, hTmp);
	}
	
}
