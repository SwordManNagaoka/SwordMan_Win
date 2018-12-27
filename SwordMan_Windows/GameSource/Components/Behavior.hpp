//----------------------------------------------------
//!@file	Think.hpp
//!@brief	思考(状態に応じた遷移)部分を作成します
//!@author	日比野　真聖
//!@date	2018/9/15
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Components/ComponentDatas/PlayerData.hpp"
#include "../Components/Think.hpp"
#include "../Components/Jump.hpp"
#include "../Components/AnimationController.hpp"
#include "../ArcheType/AttackCollision.hpp"
#include "../Class/Sound.hpp"

namespace ECS
{
	class Behavior : public Component
	{
	public:
		void	Initialize() override
		{
			if (entity->HasComponent<TriggerJumpMove>())
			{
				jumpMove = &entity->GetComponent<TriggerJumpMove>();
			}
			if (entity->HasComponent<Think>())
			{
				think = &entity->GetComponent<Think>();
			}
		}
		void	Update() override
		{
			if (entity->HasComponent<Think>())
			{
				think = &entity->GetComponent<Think>();

				switch (think->GetNowState())
				{
				case PlayerData::State::Walk:
					jumpMove->SetJumpTrigger(false);
					jumpMove->SetFallTrigger(false);
					break;
				case PlayerData::State::Jump:
				{
					if (think->GetNowMotionCnt() == 0)
					{
						if (!jumpMove->IsJumping())
						{
							jumpMove->SetJumpTrigger(true);
						}
						else
						{
							jumpMove->SetJumpTrigger(false);
						}
						//サウンドの追加
						Sound s("jump");
						s.Play(false, true);
					}
					else
					{
						jumpMove->SetJumpTrigger(false);
					}
				}
				break;
				case PlayerData::State::Airworthiness:
					jumpMove->SetFallTrigger(true);
					jumpMove->SetJumpTrigger(false);
					break;
				case PlayerData::State::Attack:
					if (think->GetNowMotionCnt() == 0)
					{
						auto pos = entity->GetComponent<Position>().val;
						SwordAttackCollision()(Vec2(pos.x ,pos.y), Vec2(96.0f, 96.0f), 15);
						//サウンドの追加
						Sound s("rolling");
						s.Play(false, true);
					
					}
					if (think->CheckMotionCancel())
					{
						auto weapon = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Wepon);
						for (const auto& w : weapon)
						{
							w->Destroy();
						}
					}
					break;
				case PlayerData::State::JumpAttack:
					if (think->GetNowMotionCnt() == 0)
					{
						JumpAttackAnimation();
						auto pos = entity->GetComponent<Position>().val;
						JumpAttackCollision()(Vec2(pos.x,pos.y), Vec2(3*96.0f, 3*96.0f), 20);
						//サウンドの追加
						Sound s("rolling");
						s.Play(false, true);
					}
					if (think->CheckMotionCancel())
					{
						NormalAnimation();
						auto weapon = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Wepon);
						for (const auto& w : weapon)
						{
							w->Destroy();
						}
					}
					break;
				}
			}
		}
		void	Draw2D() override {}
	private:
		void	Draw3D() override {}
		void NormalAnimation()
		{
			entity->DeleteComponent<ECS::AnimationDraw>();
			entity->AddComponent<ECS::AnimationDraw>("player");
			entity->GetComponent<AnimationController>().SetWidthAnimation(20, 2);
			entity->GetComponent<AnimationController>().SetIsHeightAnimation(false);
		}
		void JumpAttackAnimation()
		{
			entity->DeleteComponent<AnimationDraw>();
			entity->AddComponent<AnimationDraw>("rolling").Offset(Vec2(-96.0f, -96.0f));
			entity->GetComponent<AnimationController>().SetWidthAnimation(2, 4);
			entity->GetComponent<AnimationController>().SetIsHeightAnimation(false);
		}
	private:
		TriggerJumpMove* jumpMove;
		Think* think;
	};
}