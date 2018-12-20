//----------------------------------------------------
//!@file	Button.hpp
//!@brief	ボタンの原型を作成します
//!@author	日比野　真聖
//!@date	2018/10/8
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/ComponentDatas/ButtonTag.hpp"
#include "../Components/PushButton.hpp"

namespace ECS
{
	class ButtonArcheType : public IArcheType<const std::string&, const Vec2&,const Vec2&,const Vec2&, const float>
	{
	public:
		ECS::Entity*	operator()(const std::string& texName, const Vec2& pos,const Vec2& srcPos,const Vec2& srcSize, const float radius)
		{
			ECS::Entity*	entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<Position>(pos);
			entity->AddComponent<CircleColiider>(radius);
			entity->GetComponent<CircleColiider>().SetOffset(srcSize.x / 2.0f, srcSize.y / 2.0f);
			entity->AddComponent<RectDraw>(texName.c_str(),srcPos.x,srcPos.y,srcSize.x,srcSize.y);
			entity->AddComponent<PushButton>();
			return entity;
		}
	};
}

