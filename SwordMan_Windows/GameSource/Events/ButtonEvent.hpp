//#pragma once
//#include "../ECS/ECS.hpp"
//#include "../GameController/GameController.h"
//#include "../Collision/Collision.hpp"
//#include "../Components/BasicComponents.hpp"
//#include "../System/System.hpp"
//#include "../../Components/EntityHealthState.hpp"
//#include ".././ArcheType/Effect.hpp"
//#include "../Components/EntityCounter.hpp"
//#include "../Components/EnemyEffectMove.hpp"
//#include "../ArcheType/Score.hpp"
//
//
//namespace Event
//{
//	class ButtonEvent
//	{
//	public:
//		//ポーズボタンタップ
//		static void PauseButtonTap()
//		{
//			const auto& events = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
//			for (auto& e : events)
//			{
//				if (TouchInput::GetInput().Push(0))
//				{
//					auto& circle = e->GetComponent<ECS::CircleCollider>();
//					auto& pos = TouchInput::GetInput().GetPosition();
//					if (Collision::CircleAndPoint<ECS::CircleCollider, ECS::Position>(circle, pos)
//					{
//						//シーンをポーズへ
//						break;
//					}
//				}
//			}
//		}
//		//続けるボタンのタップ
//		static void ContinueButtonTap()
//		{
//			const auto& events = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::Event);
//			for (auto& e : events)
//			{
//				if (TouchInput::GetInput().Push(0))
//				{
//					auto& circle = e->GetComponent<ECS::CircleCollider>();
//					auto& pos = TouchInput::GetInput().GetPosition();
//					if (Collision::CircleAndPoint<ECS::CircleCollider, ECS::Position>(circle, pos)
//					{
//						//シーンをゲームシーンへ
//						break;
//					}
//				}
//			}
//		}
//		//最初から始めるボタンのタップ
//		static void StartFromBeginButtonTap()
//		{
//			const auto& events = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::Event);
//			for (auto& e : events)
//			{
//				if (TouchInput::GetInput().Push(0))
//				{
//					auto& circle = e->GetComponent<ECS::CircleCollider>();
//					auto& pos = TouchInput::GetInput().GetPosition();
//					if (Collision::CircleAndPoint<ECS::CircleCollider, ECS::Position>(circle, pos)
//					{
//						//シーンをゲームシーンの最初へ
//						break;
//					}
//				}
//			}
//		}
//		//メニューに戻るボタンのタップ
//		static void BackMenuButtonTap()
//		{
//			const auto& events = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::Event);
//			for (auto& e : events)
//			{
//				if (TouchInput::GetInput().Push(0))
//				{
//					auto& circle = e->GetComponent<ECS::CircleCollider>();
//					auto& pos = TouchInput::GetInput().GetPosition();
//					if (Collision::CircleAndPoint<ECS::CircleCollider, ECS::Position>(circle, pos)
//					{
//						//シーンをメニューシーンへ
//						break;
//					}
//				}
//			}
//		}
//		//タイトルに戻るボタンのタップ
//		static void BackTitleButtonTap()
//		{
//			const auto& events = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::Event);
//			for (auto& e : events)
//			{
//				if (TouchInput::GetInput().Push(0))
//				{
//					auto& circle = e->GetComponent<ECS::CircleCollider>();
//					auto& pos = TouchInput::GetInput().GetPosition();
//					if (Collision::CircleAndPoint<ECS::CircleCollider, ECS::Position>(circle, pos)
//					{
//						//シーンをタイトルへ
//						break;
//					}
//				}
//			}
//		}
//	};
//}