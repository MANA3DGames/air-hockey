#include "Rect.h"
using namespace MANA3DGames;

Rect::Rect()
{
}

Rect::Rect( const Vec2& a_minPt, const Vec2& a_maxPt )
: minPt( a_minPt ), maxPt( a_maxPt )
{
}

Rect::Rect( float x0, float y0, float x1, float y1 )
: minPt( x0, y0 ), maxPt( x1, y1 )
{
}


void Rect::forceInside( Circle& innerCircle )
{
	Vec2 center = innerCircle.center;
	float radius = innerCircle.radius;

	// Modify coordinates to force inside.
	if ( center.x - radius < minPt.x )
		center.x = minPt.x + radius;
	if ( center.x + radius > maxPt.x )
		center.x = maxPt.x - radius;
	if ( center.y - radius < minPt.y )
		center.y = minPt.y + radius;
	if ( center.y + radius > maxPt.y )
		center.y = maxPt.y - radius;

	// Save forced position.
	innerCircle.center = center;
}

bool Rect::isPtInside( const Vec2& pt )
{
	// Return true if the point (pt) is inside of this Rect.
	return pt.x >= minPt.x && pt.y >= minPt.y && 
		   pt.x <= maxPt.x && pt.y <= maxPt.y;
}