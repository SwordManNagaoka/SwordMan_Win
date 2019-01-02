//----------------------------------------------------
//!@file	Player.hpp
//!@brief	プレイヤーの原型を作成します
//!@author	日比野　真聖
//!@date	2018/9/14
//!@ par history tonarinohito
//! -# 死亡アニメ追加
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/Jump.hpp"
#include "../Components/Think.hpp"
#include "../Components/Behavior.hpp"
#include "../Components/AnimationController.hpp"
#include "../Components/EntityHealthState.hpp"
#include "../Components/CorrectionPosition.hpp"
#include "../Components/OutsideOfScreen.hpp"
#include "../Components/SideHitBase.hpp"
#include "../Components/PlayerDash.hpp"
#include "../Components/PlayerAddComponent.hpp"
#include "../Components/BlendMode.hpp"

namespace ECS
{
	class PlayerArcheType : public IArcheType<const Vec2&,const Vec2&>
	{
	public:
		ECS::Entity* operator()(const Vec2& pos,const Vec2& size)
		{
			ECS::Entity*	entity = &ECS::EcsSystem::GetManager().AddEntityAddTag("player");
			entity->AddComponent<Position>(pos);
			entity->AddComponent<Rotation>();
			entity->AddComponent<HitBase>(size.x, size.y).SetOffset(16.0f, 0.0f);
			entity->AddComponent<FootBase>(32, 1).SetOffset(32.0f, 96.0f);
			entity->GetComponent<FootBase>().SetColor(255,0,0);
			entity->AddComponent<Physics>().SetGravity(9.8f / 60 / 60 * 32 * 5);
			entity->AddComponent<EntityHealthState>(3);
			entity->AddComponent<Direction>();
			entity->AddComponent<AnimationDraw>("player");
			entity->AddComponent<AnimationController>(20, 2);
			entity->AddComponent<PlayerDash>().SetIsMove(true);
			entity->GetComponent<PlayerDash>().SetAddSpeed(0.02f);
			entity->GetComponent<PlayerDash>().SetTargetPos(Vec2(300,pos.y));
			entity->AddComponent<PlayerAddComponent>();
			entity->AddGroup(ENTITY_GROUP::Player);
			return entity;
		}
	};
	class PlayerDeathArcheType
	{
	private:
		class FadeDeath : public Component
		{
		private:
			Position* pos = nullptr;
			AlphaBlend* a = nullptr;
		public:
			void Initialize() override
			{
				pos = &entity->GetComponent<Position>();
				a = &entity->GetComponent<AlphaBlend>();
			}
			void Update() override
			{
				
				if (a->alpha <= 0)
				{
					ResourceManager::GetGraph().RemoveDivGraph("death");
					entity->AddComponent<KillEntity>(0);
				}
				else
				{
					pos->val.y -= 3;
					a->alpha -= 3;
				}
			}
			void Draw2D() override {}
		};
	public:
		Entity* operator()(const char* graphicName, const Vec2& pos)
		{
			Entity*	entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<Transform>().SetPosition(pos.x, pos.y);
			entity->AddComponent<Direction>();
			entity->AddComponent<Color>();
			entity->AddComponent<AlphaBlend>();
			entity->AddComponent<AnimationDraw>(graphicName).SetIndex(0);
			entity->GetComponent<AnimationDraw>().Offset(Vec2{ 0, -96.f });
			entity->AddComponent<AnimationController>(20,2).SetIsWidthAnimation(true);
			entity->GetComponent<AnimationController>().SetWidthAnimation(20,2);
			entity->AddComponent<EntityHealthState>(-2);//これがないと落下死したときに落ちる
			entity->AddComponent<FadeDeath>();
			entity->AddGroup(ENTITY_GROUP::Player);
			return entity;
		}
	};
}