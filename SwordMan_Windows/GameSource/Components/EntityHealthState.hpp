//----------------------------------------------------
//!@file	EntityHealthState.hpp
//!@brief	Entityに健康状態を作成します
//!@author	日比野　真聖
//!@date	2018/9/17
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Components/ComponentDatas/PlayerData.hpp"
#include "../Class/TouchInput.hpp"
#include "../Components/Renderer.hpp"
#include "../Utility/Counter.hpp"
#include "../Components/AnimationController.hpp"
#include "../Components/Think.hpp"
#include "../Utility/FPS.hpp"

namespace ECS
{
	class EntityHealthState final : public Component
	{
	public:
		enum class State
		{
			Non,
			Damage,
			Death,
		};
	public:
		EntityHealthState(const int maxHealth)
		{
			maxValue = maxHealth;
		}
		void Initialize() override
		{
			if (!entity->HasComponent<Health>())
			{
				health = &entity->AddComponent<Health>();
			}
			else
			{
				health = &entity->GetComponent<Health>();
			}
			health->value.SetCounter(maxValue, 1, 0, maxValue);
			state = State::Non;
		}
		void Update() override
		{
			TransitionState();
			DecisionState();
		}
		void Draw2D() override {}
		//現在の体力値の取得
		int		GetCurrentHealth() const
		{
			return health->value.GetCurrentCount();
		}
		//状態の変更
		void ChangeState(const State& state)
		{
			UpdateState(state);
		}
		//現在の状態を取得
		const State& GetCurrentState() const
		{
			return state;
		}
	private:
		void Draw3D() override {}
		void UpdateState(const State& nowState)
		{
			if (state == nowState) { return; }
			state = nowState;
			stateCnt.Reset();
		}
		//状態の遷移を行う
		void TransitionState()
		{
			State nowState = state;

			switch (nowState)
			{
			case State::Non:
			{
				break;
			}
			case State::Damage:
				if (stateCnt.GetCurrentCount() >= maxDamageTime)
				{
					nowState = State::Non;
				}
				if (health->value.GetCurrentCount() == 0)
				{
					nowState = State::Death;
				}
				break;
			case State::Death:
				break;
			}
			stateCnt.Add();
			UpdateState(nowState);
		}
		//現在の状態から動作を決定する
		void DecisionState()
		{
			if (entity->HasComponent<Think>())
			{
				playerState = entity->GetComponent<Think>().GetNowState();
			}
			switch (state)
			{
			case State::Non:
				if (stateCnt.GetCurrentCount() == 1)
				{
					oneFlag = false;
					if (!entity->HasComponent<AnimationController>()) { break; }
					entity->GetComponent<AnimationController>().SetIsHeightAnimation(false);
				}
				break;
			case State::Damage:
				if (stateCnt.GetCurrentCount() == 1)
				{
					health->value.Sub();
					if (!entity->HasComponent<AnimationController>()) { break; }
					//プレイヤーの描画アニメーションをずらす
					if (playerState == PlayerData::State::JumpAttack)
					{
						entity->GetComponent<AnimationController>().SetIsHeightAnimation(true);
						entity->GetComponent<AnimationController>().SetHeightAnimation(1, 2, 1);
						oneFlag = true;
					}
					else
					{
						entity->GetComponent<AnimationController>().SetIsHeightAnimation(true);
						entity->GetComponent<AnimationController>().SetHeightAnimation(10, 2, 1);
					}
					preState = playerState;
				}
				if (stateCnt.GetCurrentCount() >= maxDamageTime)
				{
					entity->GetComponent<AnimationController>().SetIsHeightAnimation(false);
				}
				break;
			case State::Death:
				auto& attack = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Wepon);
				for (auto& a : attack)
				{
					a->Destroy();
				}
				entity->Destroy();
				break;
			}
		}
	private:
		State	state;
		Health* health;
		Counter	stateCnt;
		int maxValue;
		const int maxDamageTime = 40;
		PlayerData::State playerState;
		PlayerData::State preState;
		bool oneFlag = false;
	};
}