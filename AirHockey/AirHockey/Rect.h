#ifndef RECT_H
#define RECT_H

#include "Circle.h"

namespace MANA3DGames
{
	class Rect
	{
	public:
		// Data members.
		Vec2 minPt;
		Vec2 maxPt;

	public:
		// Constructors
		Rect();
		Rect(const Vec2& a, const Vec2& b);
		Rect(float x0, float y0, float x1, float y1);

		// This method forces the Circle circle to be inside the Rect object that invokes this method. 
		// For keeping the paddle inside its boundary rectangle.
		void forceInside(Circle& innerCircle);

		// This method returns true if the point (pt) is inside the Rect object that invokes this method, otherwise the method returns false.
		// Which useful for detecting when the puck’s center point is inside the goal box rectangle.
		bool isPtInside(const Vec2& pt);

	};
}
#endif // RECT_H