
#include "Include/Bindu.h"

namespace BINDU
{

	void SceneObject::UpdateWithChild(float dt)
	{
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
				m->Draw(graphics,getTransforms() * cameraMatrix);
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

	void SceneObject::UpdateTransforms()
	{
		m_origin = { m_position.x + (m_size.width / 2.f), m_position.y + (m_size.height / 2.f) };

		m_translateMatrix = D2D1::Matrix3x2F::Translation(m_translation.x, m_translation.y);				// TODO: Add functionality?
		m_rotationMatrix = D2D1::Matrix3x2F::Rotation(m_rotation, m_origin);
		m_scalingMatrix = D2D1::Matrix3x2F::Scale(m_scale.x, m_scale.y, m_origin);

		m_transforms = m_scalingMatrix * m_rotationMatrix * m_translateMatrix;
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