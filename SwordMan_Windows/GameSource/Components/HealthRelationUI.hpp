//----------------------------------------------------
//!@file	HealthRelationUI.hpp
//!@brief	体力UIとの関係を扱うコンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/9/26
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../Components/BasicComponents.hpp"
#include "../../Components/ComponentDatas/PlayerData.hpp"
#include "../../Class/TouchInput.hpp"
#include "../Components/Renderer.hpp"
#include "../Utility/Counter.hpp"
#include "../../Components/EntityHealthState.hpp"
#include "../../GameController/GameController.h"


namespace ECS
{
	//----------------------------------------------------
		//コンストラクタ
		//
		//HealthRelationUI( 体力ID );
		//体力ID : 体力に応じてつけるID (例えば、体力UIが3つ目なら3とつける)
		//----------------------------------------------------
	class HealthRelationUI : public Component
	{
	public:
		HealthRelationUI(const int objectID)
			: id(objectID)
		{

		}
		void Initialize() override
		{
			
		}
		void Update() override
		{
			auto& players = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			if (players.size() == 0) { return; }
			health = &players[0]->GetComponent<EntityHealthState>();
			if (id == health->GetCurrentHealth())
			{
				entity->Destroy();
			}
		}
		void Draw2D() override
		{
		}
	private:
		void Draw3D() override {}
	private:
		EntityHealthState* health;
		int id;
	};
}