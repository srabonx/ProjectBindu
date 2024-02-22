
#include "Include/SceneManager.h"

namespace BINDU
{

	void SceneManager::onLoadResource()
	{
		for (const auto& scene : m_scenes)
			scene->onLoadResource();
	}

	void SceneManager::onReleaseResource()
	{
		for (const auto& scene : m_scenes)
			scene->onReleaseResource();

		m_scenes.clear();
	}

	void SceneManager::Update(float dt) const
	{
		for(const auto& m: m_scenes)
		{
			if (m->isActive())
				m->Update(dt);
		}
	}

	std::unique_ptr<Scene> SceneManager::RemoveScene(const char* guid)
	{
		std::unique_ptr<Scene> result = nullptr;

		if (getSceneCount() > 0)
		{
			const auto found = std::find_if(m_scenes.begin(), m_scenes.end(),
				[&](const std::unique_ptr<Scene>& scene) -> bool {return scene->getGuid() == guid; });

			if (found != m_scenes.end())
			{
				result = std::move(*found);

				m_scenes.erase(found);
			}
		}

		return result;
	}

	Scene* SceneManager::getScene(const char* guid)
	{

		if (getSceneCount() > 0)
		{
			const auto found = std::find_if(m_scenes.begin(), m_scenes.end(),
				[&](const std::unique_ptr<Scene>& scene) -> bool {return scene->getGuid() == guid; });

			if (*found)
				return found->get();
		}

		return nullptr;
	}

	void SceneManager::Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) const
	{
		for (const auto& m : m_scenes)
		{
			if (m->isActive())
				m->Draw(graphics,cameraMatrix);
		}
	}

	void SceneManager::ProcessInput() const
	{
		for (const auto& m : m_scenes)
		{
			if (m->isActive())
				m->ProcessInput();
		}
	}


	void SceneManager::AddScene(std::unique_ptr<Scene> scene, const char* guid)
	{
		scene->setGuid(guid);
		m_scenes.push_back(std::move(scene));
	}

};
