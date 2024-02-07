#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H
#include "BND_Helper.h"
#include "Vector.h"

namespace BINDU
{
	class CollisionHandler
	{
	private:


	public:
		CollisionHandler() = default;
		~CollisionHandler() = default;

		static bool PointVsRect(const Vec2f& point, const Bnd_Rect_F& rect);

		static bool RectVsRect(const Bnd_Rect_F& a, const Bnd_Rect_F& b);

		static bool RayVsRect(const Vec2f& rayOrigin, const Vec2f& rayDir, const Bnd_Rect_F& target, Vec2f& contactPoint, Vec2f& contactNormal, float& tHitNear);

		static bool DynamicRectVsRect(const Bnd_Rect_F& in,const Vec2f& inVel, const Bnd_Rect_F& target, Vec2f& contactPoint, Vec2f& contactNormal, float& contactTime,float dt);

		static float SweptAABB(const Bnd_Rect_F& in, const Vec2f& inVel, const Bnd_Rect_F& target, Vec2f& normal);	// TODO: Doesn't work properly ! need fixing

		static bool CircleVsRect(const Bnd_Circle& circle, const Vec2f& circleVelocity, const Bnd_Rect_F& target, const float dt, Vec2f& collisionNormal , float& overlap);

	};
};

#endif // COLLISIONHANDLER_H
