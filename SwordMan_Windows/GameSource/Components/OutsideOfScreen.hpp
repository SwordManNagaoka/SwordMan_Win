//----------------------------------------------------
//!@file	OutsideOfScreen.hpp
//!@brief	画面外に出たら、Entityをなくすコンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/9/28
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Components/ComponentDatas/PlayerData.hpp"
#include "../Components/Renderer.hpp"
#include "../Components/EntityHealthState.hpp"

namespace ECS
{
	//--------------------------------------------------------------
	//画面外(左側のみ+落下)に出たら、プレイヤーの状態をDeathにします
	//--------------------------------------------------------------
	class OutsideOfScreen : public Component
	{
	public:
		~OutsideOfScreen()
		{
			pos = nullptr;
		}
		void Initialize() override
		{
			pos = nullptr;
		}
		void Update() override
		{
			pos = &entity->GetComponent<Position>();
			float sizeX = entity->GetComponent<HitBase>().w();
			float sizeY = entity->GetComponent<HitBase>().h();
			if (pos->val.x < 0 - sizeX || pos->val.y > System::SCREEN_HEIGHT + sizeY)
			{
				entity->GetComponent<EntityHealthState>().ChangeState(EntityHealthState::State::Death);
			}
		}
		void Draw2D() override {}
	private:
		void Draw3D() override {}
	private:
		Position* pos;
	};
}