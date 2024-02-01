
#include "Include/SceneObject.h"

namespace BINDU
{

	void SceneObject::UpdateWithChild(float dt)
	{

			m_collider.x = (m_position.x) + m_colliderOffsetRect.x;
			m_collider.y = (m_position.y) + m_colliderOffsetRect.y;
			m_collider.w = (m_size.width) - m_colliderOffsetRect.w;
			m_collider.h = (m_size.height) - m_colliderOffsetRect.h;
		

		Update(dt);

		for(const auto& m: m_childs)
		{
			if (m->isActive())
			{
				m->Update(dt);
			}
		}
		
	}

	void SceneObject::DrawWithChild(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
	{

		Draw(graphics,cameraMatrix);
		graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());

		for (const auto& m : m_childs)
		{
			if (m->isActive())
			{
				m->setTranslation(m_position);
				m->Draw(graphics,getTransform() * cameraMatrix);
				graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
			}
		}
	}

	void SceneObject::ProcessAllInput()
	{
		ProcessInput();
		for (const auto& m : m_childs)
		{
			if (m->isActive())
				m->ProcessInput();
		}
	}

	void SceneObject::AddChild(std::unique_ptr<SceneObject> sceneObject, const char* guid)
	{
		sceneObject->setGuid(guid);
		m_childs.push_back(std::move(sceneObject));
	}

	std::unique_ptr<SceneObject> SceneObject::RemoveChild(const char* guid)
	{
		std::unique_ptr<SceneObject> result = nullptr;

		if (getChildCount() > 0)
		{
			const auto found = std::find_if(m_childs.begin(), m_childs.end(),
				[&](const std::unique_ptr<SceneObject>& obj) -> bool { return obj->getGuid() == guid; });

			//assert(found != m_childs.end());

			if (found != m_childs.end())
			{
				result = std::move(*found);

				m_childs.erase(found);
			}
		}

		return result;

	}

	void SceneObject::Clear()
	{

		m_childs.clear();
	}

	SceneObject* SceneObject::GetChild(const char* guid) const
	{
		for(const auto& m: m_childs)
		{
			if(m->getGuid() == guid)
			{
				return m.get();
			}
		}
		return nullptr;
	}
};