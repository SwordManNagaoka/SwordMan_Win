//----------------------------------------------------
//!@file	PlayerAddComponent.hpp
//!@brief	ゲーム中のプレイヤーに必要な機能を追加するコンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/10/14
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../GameController/GameController.h"
//コンポーネント
#include "../Components/Jump.hpp"
#include "../Components/Think.hpp"
#include "../Components/Behavior.hpp"
#include "../Components/CorrectionPosition.hpp"
#include "../Components/OutsideOfScreen.hpp"
#include "../Components/SideHitBase.hpp"
#include "../Components/PlayerDash.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"


namespace ECS
{
	class PlayerAddComponent : public Component
	{
	public:
		void Initialize() override
		{
			if (entity->HasComponent<PlayerDash>())
			{
				dash = &entity->GetComponent<PlayerDash>();
			}
		}
		void Update() override
		{
			if (dash == nullptr) { return; }
			if (dash->IsTargetPos())
			{
				if (onlyOne) { return; }
				entity->GetComponent<PlayerDash>().SetIsMove(false);
				entity->AddComponent<TriggerJumpMove>(-40);
				entity->AddComponent<SideHitBase>(1.0f, 96.0f).SetColor(0, 255, 0);
				entity->GetComponent<SideHitBase>().SetOffset(84.0f, -1.0f);
				entity->AddComponent<CorrectionPosition>(entity->GetComponent<Position>().val);
				entity->AddComponent<OutsideOfScreen>();
				entity->AddComponent<Think>();
				entity->AddComponent<Behavior>();
				entity->DeleteComponent<PlayerAddComponent>();
				onlyOne = true;
			}
		}
		void Draw2D() override {}
	private:
		void Draw3D() override {}
	private:
		PlayerDash* dash = nullptr;
		bool onlyOne = false;
	};
}