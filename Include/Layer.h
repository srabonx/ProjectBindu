#ifndef LAYER_H
#define LAYER_H
#include <memory>
#include <string>
#include <vector>

#include "SceneObject.h"
#include "Vector.h"

namespace BINDU
{
	class Layer
	{
	protected:

		bool	m_isActive{};

		Vec2f   m_parallaxFactor{ 1.f,1.f };

		std::string m_guid{};

		std::vector<std::unique_ptr<SceneObject>> m_objects;

		float		m_offsetX{};
		float		m_offsetY{};

		bool	m_showDebug{false};

	public:
		
		Layer() = default;
		Layer(const Layer&) = delete;
		Layer& operator = (const Layer&) = delete;
		virtual ~Layer() = default;

		virtual void			Update(float dt);

		virtual void			Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) const;
		virtual void			ProcessInput() const;

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

		void			CheckCollision();

		inline void		setOffsetX(const float x) { m_offsetX = x; }
		inline float	getOffsetX() const { return m_offsetX; }

		inline void		setOffsetY(const float y) { m_offsetY = y; }
		inline float	getOffsetY() const { return m_offsetY; }


		inline void		showDebug(const bool value) { m_showDebug = value; }

	};

};


#endif // LAYER_H
