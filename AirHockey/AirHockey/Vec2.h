#ifndef VEC2_H
#define VEC2_H

#include <Windows.h>
#include <cmath>

namespace MANA3DGames
{
	class Vec2
	{
		// Data members.
	public:
		float x;
		float y;

	public:
		// Constructors
		Vec2();
		Vec2(float coords[2]);
		Vec2(const POINT& p);
		Vec2(float v1, float v2);
		Vec2(const Vec2& rhs);

		~Vec2();

		Vec2& operator=(const Vec2& rhs);
		Vec2 operator+(const Vec2& rhs) const;
		Vec2 operator-(const Vec2& rhs) const;
		Vec2 operator-();
		void operator*=(float scalar);
		void operator+=(const Vec2& rhs);
		void operator-=(const Vec2& rhs);
		void operator/=(const Vec2& rhs);

		operator POINT();		// Convert to point.

		float length();			// Return length.

		Vec2 normalize();

		float dot(const Vec2& rhs);

		Vec2& rotate(float t);

	};

	Vec2 operator*(const Vec2& rhs, float s);
	Vec2 operator/(const Vec2& rhs, float s);
}

#endif