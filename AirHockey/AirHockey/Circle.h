#ifndef CIRCLE_H
#define CIRCLE_H

#include "Vec2.h"

namespace MANA3DGames
{
	class Circle
	{
	public:
		Circle();
		Circle(float radius, const Vec2& center);

		bool hits(Circle& another, Vec2& normal);

		float radius; // radius
		Vec2 center; // center point
	};
}
#endif // CIRCLE_H