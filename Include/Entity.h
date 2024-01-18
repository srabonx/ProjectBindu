#include "Bindu.h"
#pragma once

namespace BINDU
{
	class Entity
	{
	protected:
		
		int id;
		const char* name;
		bool visible;
		bool alive;
		int objectType;
		int lifeTimeLength;
		Timer lifeTimeTimer;
		
	public:

		Entity();
		virtual ~Entity() {}

		virtual void Update(float dt) = 0;
		virtual void Animate() = 0;
		virtual void Draw(Graphics* graphics) = 0;

		inline void setID(int value) { id = value; }
		inline int getId() const { return id; }
		inline const char* getName() { return name; }
		inline void setName(const char* value) { name = value; }
		inline bool getVisible() const { return visible; }
		inline void setVisible(bool value) { visible = value; }
		inline bool getAlive() const { return alive; }
		inline void setAlive(bool value) { alive = value; }
		inline int getLifeTime() const { return lifeTimeLength; }
		inline void setLifeTime(int millisecond) {
			lifeTimeLength = millisecond;
			lifeTimeTimer.resetStopwatch();
		}
		inline bool lifeTimeExpired() {
			return lifeTimeTimer.stopwatch(lifeTimeLength);
		}
		inline int getObjectType() const { return objectType; }
		inline void setObjectType(int value) { objectType = value; }

	};
};// Namespace
