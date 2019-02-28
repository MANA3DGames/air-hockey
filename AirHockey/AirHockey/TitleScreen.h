#ifndef TITLE_SCREEN
#define TITLE_SCREEN

#include "Sprite.h"

namespace MANA3DGames
{
	class TitleScreen
	{
	public:
		TitleScreen(HINSTANCE a_hAppInst, HWND a_hMainWnd, Vec2 a_wndCenterPt);
		~TitleScreen();

		void update(float dt);
		void draw(HDC hBackBufferDC, HDC hSpriteDC);

	private:
		Sprite *titleImage;

		HFONT titleFont;

		Vec2 wndCenterPt;				// Specifies the center point of the window’s client area.

		bool displayClickTxt;
		float clickTxtTimer;
	};
}
#endif