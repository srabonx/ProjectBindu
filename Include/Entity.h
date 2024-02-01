#ifndef ENTITY_H
#define ENTITY_H

#include "Bindu.h"


namespace BINDU
{
	class Entity
	{
	protected:

		std::string m_guid{};

		bool		m_isActive{true};
		bool		m_isAlive{ true };

		int			m_lifeTimeLength{};
		Timer		m_lifeTimeTimer;
		
	public:

		Entity();
		virtual ~Entity() = default;

		virtual void Update(float dt) = 0;

		inline void  setGuid(const char* guid) { m_guid = guid; }
		inline std::string getGuid() const { return m_guid; }

		inline void  setAlive(const bool value) { m_isAlive = value; }
		inline bool	 isAlive() const { return m_isAlive; }

		inline void		setActive(const bool value) { m_isActive = value; }

		inline bool		isActive() const { return m_isActive; }

		inline void		setLifetime(const int value) { m_lifeTimeLength = value; }

		inline int		getLifetime() const { return m_lifeTimeLength; }

	};
};// Namespace
#endif // ENTITY_H
