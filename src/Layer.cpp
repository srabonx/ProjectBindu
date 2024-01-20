
#include "Include/Bindu.h"

namespace BINDU
{
	void Layer::Update(float dt) const
	{
		for(const auto& m:m_objects)
		{
			if (m->isActive())
				m->UpdateWithChild(dt);
		}

	}

	void Layer::Draw(Graphics* graphics, const Vec2f& cameraOffset) const
	{
		for (const auto& m : m_objects)
		{
			if (m->isActive())
				m->DrawWithChild(graphics,cameraOffset);
		}
	}

	void Layer::ProcessInput() const
	{
		for (const auto& m : m_objects)
		{
			if (m->isActive())
				m->ProcessAllInput();
		}
	}

	void Layer::AddObject(std::unique_ptr<SceneObject> sceneObject, const char* guid)
	{
		sceneObject->setGuid(guid);
		m_objects.push_back(std::move(sceneObject));
	}

	std::unique_ptr<SceneObject> Layer::RemoveObject(const char* guid)
	{
		std::unique_ptr<SceneObject> result = nullptr;

		if (getObjectCount() > 0)
		{
			const auto found = std::find_if(m_objects.begin(), m_objects.end(),
				[&](const std::unique_ptr<SceneObject>& obj) -> bool {return obj->getGuid() == guid; });

			if (found != m_objects.end())
			{
				result = std::move(*found);

				m_objects.erase(found);
			}
		}

		return result;
	}

	SceneObject* Layer::getObject(const char* guid)
	{
		if (getObjectCount() > 0)
		{
			const auto found = std::find_if(m_objects.begin(), m_objects.end(),
				[&](const std::unique_ptr<SceneObject>& obj) -> bool {return obj->getGuid() == guid; });

			if (*found)
				return found->get();
		}

		return nullptr;
	}
};
