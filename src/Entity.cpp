#include "Include/Bindu.h"

namespace BINDU
{
	Entity::Entity()
	{
		
		this->id = -1;
		this->name = "";
		this->visible = true;
		this->alive = true;
		this->objectType = 0;
		this->lifeTimeLength = 0;
		this->lifeTimeTimer.resetStopwatch();	
	}

	
};// namespace