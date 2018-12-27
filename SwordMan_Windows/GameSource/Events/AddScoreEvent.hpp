////----------------------------------------------------
////!@file	AddScoreEvent.hpp
////!@brief	スコアの追加イベントを作成します
////!@author	日比野　真聖
////!@date	2018/10/7
////----------------------------------------------------
//#pragma once
//#include "../ECS/ECS.hpp"
//#include "../Collision/Collision.hpp"
//#include "../Components/BasicComponents.hpp"
//#include "../ArcheType/Score.hpp"
//#include "../GameController/GameController.h"
//#include "../Components/EntityCounter.hpp"
//
//namespace Event
//{
//	class AddScoreEvent
//	{
//	public:
//		static void Do()
//		{
//			std::vector<ECS::Entity*> addScores;
//			ECS::Entity* totalScore;
//			const auto& gameUI = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
//			for (const auto& ui : gameUI)
//			{
//				if (ui->HasComponent<ECS::AddScoreTag>())
//				{
//					addScores.push_back(ui);
//				}
//				if (ui->HasComponent<ECS::TotalScoreDraw>())
//				{
//					totalScore = ui;
//				}
//			}
//			const auto& attackCollisions = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Wepon);
//			const auto& enemys = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Enemy);
//			if (attackCollisions.size() == 0) { return; }
//			if (enemys.size() == 0) { return; }
//			
//			for (const auto& collision : attackCollisions)
//			{
//				if (!collision->HasComponent<ECS::HitBase>()) { continue; }
//				for (const auto& enemy : enemys)
//				{
//					if (!enemy->HasComponent<ECS::HitBase>()) { continue; }
//					if (Collision::BoxAndBox<ECS::HitBase, ECS::HitBase>(*collision, *enemy))
//					{
//						const auto& players = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
//						if (players.size() == 0) { return; }
//						Vec2 playerPos = players[0]->GetComponent<ECS::Position>().val;
//
//						Vec2 enemyPos = enemy->GetComponent<ECS::Position>().val;
//						Vec2 enemySize = Vec2(enemy->GetComponent<ECS::HitBase>().w() / 2.0f, enemy->GetComponent<ECS::HitBase>().h() / 2.0f);
//						float enemyLength = fabsf(enemySize.Length());
//
//						//コリジョンの幅と高さ
//						Vec2 collisionPos = collision->GetComponent<ECS::Position>().val;
//						collisionPos.x += 96.0f;
//						Vec2 collisionSize = Vec2(collision->GetComponent<ECS::HitBase>().w() / 2.0f, collision->GetComponent<ECS::HitBase>().h() / 2.0f);
//						float collisionLength = fabsf(collisionSize.Length());
//
//						//プレイヤーと敵の距離
//						Vec2 distance = enemyPos - collisionPos;
//						float length = fabsf(distance.Length());
//						float scoreLength = collisionLength + enemyLength;
//
//						int plusScore = CalcScore(length,scoreLength);
//						//追加スコアとスコアの番号を検出
//						int addScoreNum = static_cast<int>(addScores.size());
//						//エフェクト作成
//						//EnemyHitEffect(*enemy, plusScore);
//						//スコアEntityを作成
//						ECS::AddScoreArcheType()("font", Vec2(0, 50 + (addScoreNum % 3) * 32), plusScore);
//						totalScore->GetComponent<ECS::TotalScoreDraw>().AddScore(plusScore);
//						break;
//					}
//				}
//			}
//			//EnemyDestroyEffect();
//		}
//		private:
//			static int CalcScore(const float distance, const float scoreLength)
//			{
//				int plusScore = 0;
//				if (distance >= scoreLength * 0.8f)
//				{
//					plusScore = 50;
//				}
//				else if (distance >= scoreLength * 0.6f)
//				{
//					plusScore = 100;
//				}
//				else if (distance >= scoreLength * 0.3f)
//				{
//					plusScore = 150;
//				}
//				else if (distance >= 0)
//				{
//					plusScore = 200;
//				}
//				return plusScore;
//			}
//	private:
//		//敵の当たり判定時のエフェクト
//		static void EnemyHitEffect(ECS::Entity& enemy,const int score)
//		{
//			//エフェクトの作成
//			ECS::EffectData effect;
//			effect.imageName = "hitWeak";
//			effect.pos = enemy.GetComponent<ECS::Position>().val;
//			effect.killTime = 32;
//			effect.changeChipFrameTime = 8;
//			effect.chipNum = 4;
//			if (score >= 200)
//			{
//				effect.imageName = "hitStrong";
//				effect.changeChipFrameTime = 6;
//				effect.chipNum = 5;
//				effect.killTime = 30;
//			}
//			ECS::Entity* effectEntity = ECS::EffectArcheType()(effect);
//			effectEntity->GetComponent<ECS::AnimationDraw>().Offset(Vec2(-48.0f, -48.0f));
//		}
//		//敵の爆発時のエフェクト
//		static void EnemyDestroyEffect()
//		{
//			const auto& enemys = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Enemy);
//			for (const auto& enemy : enemys)
//			{
//				if (!enemy->HasComponent<ECS::KillEntity>()) { continue; }
//				if (enemy->GetComponent<ECS::EntityCounter>().IsSpecifyCnt())
//				{
//					ECS::EffectData effect;
//					effect.imageName = "bomb";
//					effect.pos = enemy->GetComponent<ECS::Position>().val;
//					effect.changeChipFrameTime = 8;
//					effect.chipNum = 4;
//					effect.killTime = 32;
//					ECS::Entity* effectEntity = ECS::EffectArcheType()(effect);
//					effectEntity->GetComponent<ECS::AnimationDraw>().Offset(Vec2(-48.0f, -48.0f));
//					break;
//				}
//			}
//		}
//	};
//}