
#include "Include/Bindu.h"

namespace BINDU
{
	void Scene::Update(float dt) const
	{
		for(const auto& m: m_layers)
		{
			if (m->isActive())
				m->Update(dt);
		}
	}

	void Scene::Draw(Graphics* graphics) const
	{
		for (const auto& m : m_layers)
		{
			if (m->isActive())
				m->Draw(graphics);
		}
	}

	void Scene::ProcessInput() const
	{
		for (const auto& m : m_layers)
		{
			if (m->isActive())
				m->ProcessInput();
		}
	}

	void Scene::AddLayer(std::unique_ptr<Layer> sceneLayer, const char* guid)
	{
		sceneLayer->setGuid(guid);
		m_layers.push_back(std::move(sceneLayer));
	}

	std::unique_ptr<Layer> Scene::RemoveLayer(const char* guid)
	{
		std::unique_ptr<Layer> result = nullptr;

		if(getLayerCount() > 0)
		{
			const auto found = std::find_if(m_layers.begin(), m_layers.end(),
				[&](const std::unique_ptr<Layer>& layer) -> bool {return layer->getGuid() == guid; });


			if (found != m_layers.end())
			{
				result = std::move(*found);

				m_layers.erase(found);
			}

		}

		return result;

	}

	Layer* Scene::getLayer(const char* guid)
	{
		if (getLayerCount() > 0)
		{
			const auto found = std::find_if(m_layers.begin(), m_layers.end(),
				[&](const std::unique_ptr<Layer>& layer) -> bool {return layer->getGuid() == guid; });

			if (*found)
				return found->get();
		}

		return nullptr;
	}

};