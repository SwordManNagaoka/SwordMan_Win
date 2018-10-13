//----------------------------------------------------
//!@file	PauseTap.hpp
//!@brief	ポーズボタンのイベントを作成します
//!@author	日比野　真聖
//!@date	2018/9/16
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../GameController/GameController.h"
#include "../Collision/Collision.hpp"
#include "../Components/BasicComponents.hpp"
#include "../System/System.hpp"
#include "../Components/EntityHealthState.hpp"
#include ".././ArcheType/Effect.hpp"
#include "../Components/EntityCounter.hpp"
#include "../Components/EnemyEffectMove.hpp"
#include "../Class/Scene/SceneManager.hpp"


namespace Event
{
	class PouseButtonEvent
	{
	public:
		//続けるボタンのタップ
		static void PouseButtonTap()
		{
			if (Input::Get().GetKeyFrame(KEY_INPUT_X) == 1)
			{
				const auto& events = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
				for (auto& e : events)
				{
					if (!e->HasComponent<ECS::CircleColiider>()) { continue; }
					auto circle = e->GetComponent<ECS::CircleColiider>();
					Vec2 circlePos = Vec2(circle.x(), circle.y());
					float radius = circle.radius();
					auto touchPos = TouchInput::GetInput().GetTouchIDPos(0);
					printfDx("ゲームシーンからポーズへ\n");
					//if (Collision::CircleAndPoint(circlePos, radius, touchPos))
					{
						//シーンをゲームシーンへ
						Scene::SceneManager::Get().ChangeScene(Scene::SceneManager::State::Pause);
						break;
					}
				}
			}
		}
	};
}