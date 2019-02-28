#include "Circle.h"
using namespace MANA3DGames;

Circle::Circle()
: radius( 0.0f ), center( 0.0f, 0.0f )
{
}

Circle::Circle( float radius, const Vec2& center )
: radius( radius ), center( center )
{
}

bool Circle::hits( Circle& another, Vec2& normal )
{
	Vec2 u = another.center - center;

	if ( u.length() <= radius + another.radius )
	{
		normal = u.normalize();

		// Make sure circles never overlap--at most  they can be tangent.
		another.center = center + ( normal * ( radius + another.radius ) );

		return true;
	}

	return false;
}