#ifndef SCENE_H
#define SCENE_H

#include "Bindu.h"

namespace BINDU
{
	class Scene
	{
	private:


		bool	m_isActive{};

		std::string m_guid{};

		std::vector<std::unique_ptr<Layer>> m_layers;

	public:

		Scene() = default;
		~Scene() = default;

		void			Update(float dt) const;
		void			Draw(Graphics* graphics, const Vec2f& cameraOffset) const;
		void			ProcessInput() const;

		void			AddLayer(std::unique_ptr<Layer> sceneLayer, const char* guid);

		std::unique_ptr<Layer> RemoveLayer(const char* guid);

		Layer*			getLayer(const char* guid);

		inline void		setActive(const bool value) { m_isActive = value; }

		inline bool		isActive() const { return m_isActive; }

		inline int		getLayerCount() const { return static_cast<int>(m_layers.size()); }

		inline void		setGuid(const char* guid) { m_guid = guid; }

		inline std::string getGuid() const { return m_guid; }

	};// Class

};

#endif // SCENE_H
