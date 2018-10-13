//----------------------------------------------------
//!@file	EnemyJumpMove.hpp
//!@brief	敵のジャンプ動作を作成します
//!@author	日比野　真聖
//!@date	2018/10/1
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/AnimationController.hpp"


namespace ECS
{
	class EnemyJumpMove : public Component
	{
	public:
		EnemyJumpMove(const int jumpTime)
			: jumpTime(jumpTime)
		{

		}
		void Initialize() override
		{
			jumpCounter.SetCounter(0, 1, 0, jumpTime);
			if (entity->HasComponent<TriggerJumpMove>())
			{
				jumpMove = &entity->GetComponent<TriggerJumpMove>();
			}
		}
		void Update() override
		{
			if (jumpMove == nullptr) { return; }
			if (entity->HasComponent<TriggerJumpMove>())
			{
				jumpMove = &entity->GetComponent<TriggerJumpMove>();
			}
			jumpCounter.Add();
			if (jumpCounter.IsMax())
			{
				jumpMove->SetJumpTrigger(true);
				jumpCounter.Reset();
			}
			else
			{
				jumpMove->SetJumpTrigger(false);
			}
		}
		void Draw2D() override {}
	private:
		void Draw3D() override {}
	private:
		Counter jumpCounter;
		TriggerJumpMove* jumpMove = nullptr;
		const int jumpTime;
	};

}