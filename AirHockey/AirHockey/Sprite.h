#ifndef SPRITE_H
#define SPRITE_H

#include <windows.h>
#include "Circle.h"
#include "Vec2.h"

namespace MANA3DGames
{
	class Sprite
	{
	public:
		Circle boundingCircle;	// Area of the sprite.
		Vec2 position;			// The center position of the sprite rectangle.
		Vec2 velocity;			// The direction and speed the sprite is moving in.

	public:
		Sprite(HINSTANCE hAppInst, int imageID, const Circle& boundingCircle, const Vec2& initialPosition, const Vec2& initialVelocity);
		Sprite(HINSTANCE hAppInst, int imageID, int maskID, const Circle& boundingCircle, const Vec2& initialPosition, const Vec2& initialVelocity);
		~Sprite();

		int getWidth();
		int getHeight();

		void update(float dt);
		void draw(HDC hBackBufferDC, HDC hSpriteDC);


	private:
		// Make copy constructor and assignment operator private so client cannot copy Sprites. We do this because
		// this class is not designed to be copied because it is not efficient--copying bitmaps is slow (lots of memory).
		Sprite(const Sprite& rhs);
		Sprite& operator=(const Sprite& rhs);

	protected:
		HINSTANCE hAppInst;	// A handle to the application instance.
		HBITMAP hImage;		// A handle to the sprite image bitmap.
		HBITMAP hMask;		// A handle to the sprite mask bitmap.
		BITMAP imageBM;		// A structure containing the sprite image bitmap info.
		BITMAP maskBM;		// A structure containing the sprite mask bitmap info.

	};
}
#endif // SPRITE_H