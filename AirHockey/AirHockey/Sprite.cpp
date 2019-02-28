#include "Sprite.h"
#include <cassert>
using namespace MANA3DGames;

Sprite::Sprite( HINSTANCE a_hAppInst, int imageID, const Circle& a_boundingCircle, const Vec2& a_initialPosition, const Vec2& a_initialVelocity )
{
	// Initialization.
	hAppInst = a_hAppInst;
	boundingCircle = a_boundingCircle;
	position = a_initialPosition;
	velocity = a_initialVelocity;

	// Load the bitmap resources.
	hImage = LoadBitmap(hAppInst, MAKEINTRESOURCE(imageID));

	// Get the BITMAP structure for the bitmap.
	GetObject(hImage, sizeof(BITMAP), &imageBM);
}

Sprite::Sprite( HINSTANCE a_hAppInst, int imageID, int maskID, const Circle& a_boundingCircle, const Vec2& a_initialPosition, const Vec2& a_initialVelocity )
{
	// Initialization.
	hAppInst = a_hAppInst;
	boundingCircle = a_boundingCircle;
	position = a_initialPosition;
	velocity = a_initialVelocity;

	// Load the bitmap resources.
	hImage = LoadBitmap( hAppInst, MAKEINTRESOURCE( imageID ) );
	hMask = LoadBitmap( hAppInst, MAKEINTRESOURCE( maskID ) );

	// Get the BITMAP structure for each of the bitmaps.
	GetObject( hImage, sizeof(BITMAP), &imageBM );
	GetObject( hMask, sizeof(BITMAP), &maskBM );

	// Verify that image and Mask should be the same dimensions.
	assert( imageBM.bmWidth == maskBM.bmWidth );
	assert( imageBM.bmHeight == maskBM.bmHeight );
}

Sprite::~Sprite()
{
	// Free the resources we created in the constructor.
	DeleteObject( hImage );
	DeleteObject( hMask );
}


int Sprite::getWidth()
{
	// Returns the width, in pixels, of the sprite.
	return imageBM.bmWidth;
}

int Sprite::getHeight()
{
	// Returns the height, in pixels, of the sprite.
	return imageBM.bmHeight;
}


void Sprite::update( float dt )
{
	// Update the sprites position.
	position += velocity * dt;

	// Update bounding circle, too. That is, the bounding circle moves with the sprite.
	boundingCircle.center = position;
}

void Sprite::draw( HDC hBackBufferDC, HDC hSpriteDC )
{
	// The position BitBlt wants is not the sprite's center position; rather, it wants the upper-left position, so compute that.
	int width = getWidth();
	int height = getHeight();

	// Upper-left corner.
	int x = (int)position.x - ( width / 2 );
	int y = (int)position.y - ( height / 2 );


	// Note: For this masking technique to work, it is assumed the backbuffer bitmap has been cleared to some non-zero value. 

	// Check for a mask bitmap.
	if ( hMask != NULL )
	{
		// Select the mask bitmap.
		HGDIOBJ oldObj = SelectObject( hSpriteDC, hMask );

		// Draw the mask to the backbuffer with SRCAND. This only draws the BLACK pixels in the mask to the backbuffer,
		// thereby marking the pixels we want to draw the sprite image onto.
		BitBlt(hBackBufferDC, x, y, width, height, hSpriteDC, 0, 0, SRCAND);

		// Now select the image bitmap. (we don't need the mask bitmap anymore).
		SelectObject(hSpriteDC, hImage);

		// Draw the image to the backbuffer with SRCPAINT. This will only draw the image onto the pixels that where previously marked black by the mask.
		BitBlt(hBackBufferDC, x, y, width, height, hSpriteDC, 0, 0, SRCPAINT);

		// Restore the original bitmap object.
		SelectObject(hSpriteDC, oldObj);
	}
	// There is no mask bitmap.
	else
	{
		// Select the image bitmap.
		HGDIOBJ oldObj = SelectObject( hSpriteDC, hImage );

		// Draw the image to the backbuffer with SRCPAINT. This will only draw the image onto the pixels that where previously marked black by the mask.
		BitBlt( hBackBufferDC, x, y, width, height, hSpriteDC, 0, 0, SRCCOPY );

		// Restore the original bitmap object.
		SelectObject( hSpriteDC, oldObj );
	}
	
}
