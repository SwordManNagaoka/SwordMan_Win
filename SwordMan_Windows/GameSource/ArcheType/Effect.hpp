//----------------------------------------------------
//!@file	Effect.hpp
//!@brief	エフェクトの原型を作成します
//!@author	日比野　真聖
//!@date	2018/9/16
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/AnimationController.hpp"


namespace ECS
{
	struct EffectData
	{
		std::string imageName;	//画像名
		Vec2 pos;				//位置
		int killTime;			//生存時間
		int changeChipFrameTime;//チップ切り替えフレーム時間
		int chipNum;			//チップ枚数
	};

	class EffectArcheType : public IArcheType<const EffectData&>
	{
	public:
		ECS::Entity* operator()(const EffectData& effectData)
		{
			ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<Position>(effectData.pos);
			entity->AddComponent<Direction>();
			entity->AddComponent<AnimationDraw>(effectData.imageName.c_str());
			entity->AddComponent<AnimationController>(effectData.changeChipFrameTime,effectData.chipNum);
			entity->AddComponent<KillEntity>(effectData.killTime);
			entity->AddGroup(ENTITY_GROUP::Effect);
			return entity;
		}
	};
}