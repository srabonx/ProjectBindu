
#define NOMINMAX

#include "Include/CollisionHandler.h"
#include <algorithm>


#include "Include/initConsole.h"

namespace BINDU
{
	bool CollisionHandler::PointVsRect(const Vec2f& point, const Bnd_Rect_F& rect)
	{
		return (point.x >= rect.x && point.y >= rect.y && point.x < rect.x + rect.w && point.y < rect.y + rect.h);
	}

	bool CollisionHandler::RectVsRect(const Bnd_Rect_F& a, const Bnd_Rect_F& b)
	{
		return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
	}

	bool CollisionHandler::RayVsRect(const Vec2f& rayOrigin, const Vec2f& rayDir, const Bnd_Rect_F& target, Vec2f& contactPoint, Vec2f& contactNormal, float& tHitNear)
	{

		Vec2f tNear = (Vec2f(target.x, target.y) - rayOrigin) / rayDir;
		Vec2f tFar = ((Vec2f(target.x, target.y) + Vec2f(target.w, target.h)) - rayOrigin) / rayDir;

		if (std::isnan(tFar.y) || std::isnan(tFar.x)) return false;
		if (std::isnan(tNear.y) || std::isnan(tNear.x)) return false;

		if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
		if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

		if (tNear.x > tFar.y || tNear.y > tFar.x) return false;


     	tHitNear = std::max(tNear.x, tNear.y);
				
		float tHitFar = std::min(tFar.x, tFar.y);

		if (tHitFar < 0) return false;

		if (tHitNear > 1) return false;

		contactPoint = rayOrigin + (rayDir * tHitNear) ;

		if (tNear.x > tNear.y)
			if (rayDir.x < 0)
				contactNormal = { 1,0 };
			else
				contactNormal = { -1,0 };
		else if (tNear.x < tNear.y)
			if (rayDir.y < 0)
				contactNormal = { 0,1 };
			else
				contactNormal = { 0,-1 };

		return true;

	}

	bool CollisionHandler::DynamicRectVsRect(const Bnd_Rect_F& in, const Vec2f& inVel, const Bnd_Rect_F& target, Vec2f& contactPoint,
		Vec2f& contactNormal, float& contactTime,float dt)
	{
		if (inVel.x == 0.f && inVel.y == 0.f)
			return false;

		Bnd_Rect_F expandedRect;
		expandedRect.x = target.x - in.w / 2.f;
		expandedRect.y = target.y - in.h / 2.f;
		expandedRect.w = target.w + in.w;
		expandedRect.h = target.h + in.h;

		Vec2f rayStart = Vec2f(in.x, in.y) + Vec2f(in.w, in.h) / 2.f;

		if(RayVsRect(rayStart,inVel*dt,expandedRect,contactPoint,contactNormal,contactTime))
		{
			if (contactTime < 1.0f)
				return true;
		}

		return false;

	}

	float CollisionHandler::SweptAABB(const Bnd_Rect_F& in,const Vec2f& inVel, const Bnd_Rect_F& target, Vec2f& normal)
	{
		Vec2f invEntry;
		Vec2f invExit;

		// find the distance between the objects on the near and far side for both axis.

		if(inVel.x > 0.0f)						// moving right.
		{
			invEntry.x = target.x - (in.x + in.w);					// near side on positive X axis.
			invExit.x = (target.x + target.w) - in.x;				// far side on positive X axis.
		}
		else									// moving left.
		{
			invEntry.x = (target.x + target.w) - in.x;				// near side on negative X axis.
			invExit.x = target.x - (in.x + in.w);					// far side on negative X axis.
		}

		if(inVel.y > 0.0f)						// moving down
		{
			invEntry.y = target.y - (in.y + in.h);					// near side on the positive Y axis.
			invExit.y = (target.y + target.h) - in.y;				// far side on the positive Y axis.
		}
		else									// moving up
		{	
			invEntry.y = (target.y + target.h) - in.y;				// near side on the negative Y axis.
			invExit.y = target.y - (in.y + in.h);					// far side on the negative Y axis.
		}

		Vec2f timeEntry;
		Vec2f timeExit;

		if(inVel.x == 0.0f)						// to prevent divide by zero.
		{
			timeEntry.x = -std::numeric_limits<float>::infinity();
			timeExit.x = std::numeric_limits<float>::infinity();
		}
		else
		{
			timeEntry.x = invEntry.x / inVel.x;						// time of entry on X axis.
			timeExit.x = invExit.x / inVel.x;						// time of exit on X axis.
		}

		if(inVel.y == 0.0f)						// to prevent divide by zero.
		{
			timeEntry.y = -std::numeric_limits<float>::infinity();
			timeExit.y = std::numeric_limits<float>::infinity();
		}
		else
		{
			timeEntry.y = invEntry.y / inVel.y;						// time of entry on Y axis.
			timeExit.y = invExit.y / inVel.y;						// time of exit on Y axis.
		}

		// find the earliest/latest time of collision.

		float nearTime = std::max(timeEntry.x, timeEntry.y);			// when the collision occured.
		float farTime = std::min(timeExit.x, timeExit.y);			// when the collision ended.

		// if there was no collision
		if(nearTime > farTime || timeEntry.x < 0.0f && timeEntry.y < 0.0f || timeEntry.x > 1.0f || timeEntry.y > 1.0f)
		{
			normal.x = 0.0f;
			normal.y = 0.0f;
			return 1.0f;
		}
		else				// if there was a collision.
		{
			if(timeEntry.x > timeEntry.y)
			{
				if(invEntry.x < 0.0f)
				{
					normal.x = 1.0f;
					normal.y = 0.0f;
				}
				else
				{
					normal.x = -1.0f;
					normal.y = 0.0f;
				}
			}
			else
			{
				if(invEntry.y < 0.0f)
				{
					normal.x = 0.0f;
					normal.y = 1.0f;
				}
				else
				{
					normal.x = 0.0f;
					normal.y = -1.0f;
				}
			}

			// return time of collision

			return nearTime;
		}

	}

	bool CollisionHandler::CircleVsRect(const Bnd_Circle& circle, const Vec2f& circleVelocity, const Bnd_Rect_F& target, const float dt, Vec2f& collisionNormal, float& overlap)
	{
		Vec2f nearestPoints;
		Vec2f potentialPosition = circle.position + circleVelocity * dt;


		nearestPoints.x = std::max(target.x , std::min(potentialPosition.x, (target.x + target.w) + 1));
		nearestPoints.y = std::max(target.y , std::min(potentialPosition.y, (target.y + target.h) + 1));

		Vec2f rayToNearest = nearestPoints - potentialPosition;

		float l_overlap = circle.radius - rayToNearest.Length();

		if (std::isnan(l_overlap))
			l_overlap = 0;

		collisionNormal = rayToNearest.Normal();

		if(std::isnan(collisionNormal.x) || std::isnan(collisionNormal.y))
		{
			collisionNormal = { 0.0f,0.0f };

			if (potentialPosition.x < 0.0f)
				collisionNormal.x = -1.0f;
			if (potentialPosition.x > 0.0f)
				collisionNormal.x = 1.0f;
			if (potentialPosition.y < 0.0f)
				collisionNormal.y = -1.0f;
			if (potentialPosition.y > 0.0f)
				collisionNormal.y = 1.0f;
		}

	/*	if (collisionNormal.x < 0)
			collisionNormal.x = -1;
		if (collisionNormal.x > 0)
			collisionNormal.x = 1;
		if (collisionNormal.y < 0)
			collisionNormal.y = -1;
		if (collisionNormal.y > 0)
			collisionNormal.y = 1;*/

		overlap = l_overlap;

		if(l_overlap > 0)
		{
			return true;
		}
		return false;
	}
};
