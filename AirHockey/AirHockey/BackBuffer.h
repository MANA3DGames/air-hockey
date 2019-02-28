#ifndef BACKBUFFER_H
#define BACKBUFFER_H

#include <windows.h>

namespace MANA3DGames
{
	class BackBuffer
	{
	public:
		BackBuffer(HWND hWnd, int width, int height);
		~BackBuffer();

		HDC getDC();

		int width();
		int height();

		void present();

	private:
		// Make copy constructor and assignment operator private so client cannot copy BackBuffers. We do this because
		// this class is not designed to be copied because it is not efficient--copying bitmaps is slow (lots of memory).
		// In addition, most applications will probably only need one BackBuffer anyway.
		BackBuffer(const BackBuffer& rhs);

		BackBuffer& operator=(const BackBuffer& rhs);

	private:
		HWND mhWnd;
		HDC mhDC;

		HBITMAP mhSurface;
		HBITMAP mhOldObject;

		int mWidth;
		int mHeight;
	};
}
#endif // BACKBUFFER_H