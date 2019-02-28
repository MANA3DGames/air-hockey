#include "Vec2.h"
using namespace MANA3DGames;

// ------ Constructors ------

Vec2::Vec2() : x(0.0f), y(0.0f)
{
}

Vec2::Vec2( float coords[2] ) : x(coords[0]), y(coords[1])
{
}

Vec2::Vec2( const POINT& p ) : x(p.x), y(p.y)
{
}

Vec2::Vec2( float v1, float v2 ) : x(v1), y(v2)
{
}

Vec2::Vec2( const Vec2& rhs ) 
{ 
	*this = rhs; 
}


// ------ Destructor ------

Vec2::~Vec2()
{
}



// ------ Operators ------

Vec2& Vec2::operator=(const Vec2& rhs)
{
	if (this == &rhs)
		return *this;

	x = rhs.x;
	y = rhs.y;

	return *this;
}

Vec2 Vec2::operator+( const Vec2& rhs ) const
{
	Vec2 sum;

	sum.x = x + rhs.x;
	sum.y = y + rhs.y;

	return sum;
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	Vec2 result;

	result.x = x - rhs.x;
	result.y = y - rhs.y;

	return result;
}

Vec2 Vec2::operator-()
{
	Vec2 result;
	x = -x;
	y = -y;

	return result;
}

void Vec2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
}

void Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator/=(const Vec2& rhs)
{
	// Assumes rhs != 0
	x /= rhs.x;
	y /= rhs.y;
}


// ------ Vector2 operations ------

Vec2::operator POINT()
{
	POINT p = { (int)x, (int)y };
	return p;
}

float Vec2::length()
{
	return sqrtf( x * x + y * y );
}

//Note: Don't return a ref.
Vec2 Vec2::normalize()
{
	float magnitude = length();
	Vec2 result( x / magnitude, y / magnitude );
	return result;
}

float Vec2::dot(const Vec2& rhs)
{
	return x * rhs.x + y * rhs.y;
}

Vec2& Vec2::rotate(float t)
{
	float newX;
	newX = x * cosf(t) - y * sinf(t);
	y = y * cosf(t) + x * sinf(t);

	this->x = newX;
	return *this;
}




Vec2 MANA3DGames::operator*( const Vec2& rhs, float s )
{
	Vec2 result;

	result.x = rhs.x * s;
	result.y = rhs.y * s;

	return result;
}

Vec2 MANA3DGames::operator/( const Vec2& rhs, float s )
{
	Vec2 result;

	result.x = rhs.x / s;
	result.y = rhs.y / s;

	return result;
}

