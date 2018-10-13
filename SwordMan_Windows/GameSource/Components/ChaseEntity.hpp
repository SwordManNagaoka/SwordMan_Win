//----------------------------------------------------
//!@file	ChaseEntity.hpp
//!@brief	指定したEntityを追尾するコンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/9/25
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/AnimationController.hpp"


namespace ECS
{
	//-----------------------------------------------------------------
	//コンストラクタ
	//
	//ChaseEntity( ターゲットのEntityの参照 );
	//-----------------------------------------------------------------
	class ChaseEntity : public Component
	{
	public:
		ChaseEntity(Entity& targetEntity)
			: targetEntity(targetEntity)
		{
		}
		void	Initialize() override
		{
		}
		void	Update() override
		{
			Vec2& myPos = entity->GetComponent<Position>().val;
			if (targetEntity.HasComponent<Position>())
			{
				myPos = targetEntity.GetComponent<Position>().val;
			}
		}
		void	Draw2D() override {}
	private:
		void	Draw3D() override {}
	private:
		Entity&	targetEntity;
	};
}