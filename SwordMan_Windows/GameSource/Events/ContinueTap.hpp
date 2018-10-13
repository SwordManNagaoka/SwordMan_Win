//----------------------------------------------------
//!@file	ContinueTap.hpp
//!@brief	続けるボタンのイベントを作成します
//!@author	日比野　真聖
//!@date	2018/9/16
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../GameController/GameController.h"
#include "../Collision/Collision.hpp"
#include "../Components/BasicComponents.hpp"
#include "../System/System.hpp"
#include "../../Components/EntityHealthState.hpp"
#include ".././ArcheType/Effect.hpp"
#include "../Components/EntityCounter.hpp"
#include "../Components/EnemyEffectMove.hpp"
#include "../../Class/Scene/SceneManager.hpp"
#include "../../Components/ComponentDatas/ButtonTag.hpp"

namespace Event
{
	class ContinueButtonEvent
	{
	public:
		//続けるボタンのタップ
		static void ContinueButtonTap()
		{
			if (Input::Get().GetKeyFrame(KEY_INPUT_Z) == 1)
			{
				const auto& events = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
				for (auto& e : events)
				{
					if (!e->HasComponent<ECS::ContinueButtonTag>()) { continue; }
					auto circle = e->GetComponent<ECS::CircleColiider>();
					Vec2 circlePos = Vec2(circle.x(), circle.y());
					float radius = circle.radius();
					auto touchPos = TouchInput::GetInput().GetTouchIDPos(0);

					//if (Collision::CircleAndPoint(circlePos, radius, touchPos))
					{
						//シーンをゲームシーンへ
						printfDx("続けるからゲームへ");
						//Scene::SceneManager::Get().ChangeScene(Scene::SceneManager::State::Game);
						break;
					}
				}
			}
		}
	};
}