#ifndef LAYER_H
#define LAYER_H

#include "Bindu.h"

namespace BINDU
{
	class Layer
	{
	private:

		bool	m_isActive{};

		Vec2f   m_parallaxFactor{ 1.f,1.f };

		std::string m_guid{};

		std::vector<std::unique_ptr<SceneObject>> m_objects;

	public:
		
		Layer() = default;
		~Layer() = default;

		void			Update(float dt) const;
		void			Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) const;
		void			ProcessInput() const;

		void			AddObject(std::unique_ptr<SceneObject> sceneObject, const char* guid);

		std::unique_ptr<SceneObject> RemoveObject(const char* guid);

		SceneObject*	getObject(const char* guid);

		inline void		setActive(const bool value) { m_isActive = value; }

		inline bool		isActive() const { return m_isActive; }

		inline int		getObjectCount() const { return static_cast<int>(m_objects.size()); }

		inline void		setGuid(const char* guid) { m_guid = guid; }

		inline std::string getGuid() const { return m_guid; }

		inline void		setParallaxFactor(const Vec2f& parallaxFactor) { m_parallaxFactor = parallaxFactor; }
		inline Vec2f	getParallaxFactor() const { return m_parallaxFactor; }

	};

};


#endif // LAYER_H
