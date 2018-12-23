////----------------------------------------------------
////!@file	EventTrigger.hpp
////!@brief	イベントのシステムを作成します
////!@author	日比野　真聖
////!@date	2018/10/22
////----------------------------------------------------
//#pragma once
//#include "../ECS/ECS.hpp"
//#include "../Components/Renderer.hpp"
//#include "../GameController/GameController.h"
//
//
////アーキタイプ
//#include "../ArcheType/Score.hpp"
//#include "../ArcheType/Effect.hpp"
////コンポーネント
//#include "../Components/BasicComponents.hpp"
//#include "../Components/EnemyDefaultMove.hpp"
//#include "../Components/Jump.hpp"
//#include "../Components/EntityHealthState.hpp"
//#include "../Components/EntityCounter.hpp"
//#include "../Components/EnemyEffectMove.hpp"
//#include "../Components/EnemyJumpMove.hpp"
//#include "../Components/TotalScoreDraw.hpp"
//#include "../Class/Sound.hpp"
//
//namespace ECS
//{
//	class EnemyHitWeapon : public Component
//	{
//	public:
//		void Initialize() override {}
//		void Update() override {}
//		void Draw2D() override {}
//		const bool HitWeapon() noexcept
//		{
//			auto& attackCollisions = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::Wepon);
//			for (const auto& ac : attackCollisions)
//			{
//				if (Collision::BoxAndBox<ECS::HitBase, ECS::HitBase>(*ac, *entity))
//				{
//					SetWeapon(*ac);
//					return true;
//				}
//			}
//			return false;
//		}
//		const Entity& GetHitWeapon() const noexcept
//		{
//			return *hitCollision;
//		}
//		void SetWeapon(Entity& weapon) noexcept
//		{
//			hitCollision = &weapon;
//		}
//	private:
//		void Draw3D() override {}
//		Entity* hitCollision;
//	};
//
//	class EnemyEvent : public Component
//	{
//	private:
//		static void Destory()
//		{
//			const auto& enemys = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Enemy);
//			for (const auto& enemy : enemys)
//			{
//				if (enemy->GetComponent<EnemyHitWeapon>().HitWeapon())
//				{
//					const auto& collision = enemy->GetComponent<EnemyHitWeapon>().GetHitWeapon();
//					//敵のエフェクト時の動作
//					EnemyEffectMove(*enemy, *&collision);
//					//スコアの決定
//					DecideScore(*enemy, *&collision);
//					//コンポーネントのつけ外し
//					enemy->DeleteComponent<ECS::HitBase>();
//					enemy->DeleteComponent<ECS::EnemyDefaultMove>();
//					enemy->DeleteComponent<ECS::EnemyJumpMove>();
//					enemy->DeleteComponent<ECS::TriggerJumpMove>();
//					enemy->DeleteComponent<ECS::Physics>();
//					enemy->AddComponent<ECS::EntityCounter>().SetSpecifyCnt(30 - 1);
//					enemy->AddComponent<ECS::KillEntity>(30);
//				}
//			}
//		}
//	private:
//		static void CreateHitEffect(const ECS::Entity& enemy, const int score)
//		{
//			//サウンドをつける
//			Sound s("hit");
//			s.Play(false, true);
//			//エフェクトの作成
//			ECS::EffectData effect;
//			effect.imageName = "hitWeak";
//			effect.pos = enemy.GetComponent<ECS::Position>().val;
//			effect.killTime = 12;
//			effect.changeChipFrameTime = 3;
//			effect.chipNum = 4;
//			if (score >= 200)
//			{
//				effect.imageName = "hitStrong";
//				effect.changeChipFrameTime = 3;
//				effect.chipNum = 5;
//				effect.killTime = 15;
//			}
//			ECS::Entity* effectEntity = ECS::EffectArcheType()(effect);
//			effectEntity->GetComponent<ECS::AnimationDraw>().Offset(Vec2(-48.0f, -48.0f));
//		}
//		static void CreateDestoryEffect()
//		{
//			const auto& enemys = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Enemy);
//			for (const auto& enemy : enemys)
//			{
//				if (!enemy->HasComponent<ECS::KillEntity>()) { continue; }
//				if (enemy->GetComponent<ECS::EntityCounter>().IsSpecifyCnt())
//				{
//					//サウンドをつける
//					Sound s("bomb");
//					s.Play(false, true);
//					//エフェクトをつける
//					ECS::EffectData effect;
//					effect.imageName = "bomb";
//					effect.pos = enemy->GetComponent<ECS::Position>().val;
//					effect.changeChipFrameTime = 3;
//					effect.chipNum = 4;
//					effect.killTime = 12;
//					ECS::Entity* effectEntity = ECS::EffectArcheType()(effect);
//					effectEntity->GetComponent<ECS::AnimationDraw>().Offset(Vec2(-48.0f, -48.0f));
//					break;
//				}
//			}
//		}
//		static void EnemyEffectMove(ECS::Entity& enemy,const ECS::Entity& collision)
//		{
//			//攻撃された方向と逆の方向へ跳ねる
//			Vec2 collisionPos = collision.GetComponent<ECS::Position>().val;
//			collisionPos.x += 96.0f;
//			Vec2 enemyPos = enemy.GetComponent<ECS::Position>().val;
//			//方向ベクトル
//			Vec2 dirVec = (enemyPos - collisionPos).Normalize();
//			enemy.AddComponent<ECS::EnemyEffectMove>().SetDirMove(dirVec, 36.0f);
//		}
//		static void DecideScore(ECS::Entity& enemy, const ECS::Entity& collision)
//		{
//			std::vector<ECS::Entity*> addScores;
//			ECS::Entity* totalScore = nullptr;
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
//			//敵との距離を計算
//			Vec2 enemyPos = enemy.GetComponent<ECS::Position>().val;
//			Vec2 enemySize = Vec2(enemy.GetComponent<ECS::HitBase>().w() / 2.0f, enemy.GetComponent<ECS::HitBase>().h() / 2.0f);
//			float enemyLength = fabsf(enemySize.Length());
//			//コリジョンの幅と高さ
//			Vec2 collisionPos = collision.GetComponent<ECS::Position>().val;
//			collisionPos.x += 96.0f;
//			Vec2 collisionSize = Vec2(collision.GetComponent<ECS::HitBase>().w() / 2.0f, collision.GetComponent<ECS::HitBase>().h() / 2.0f);
//			float collisionLength = fabsf(collisionSize.Length());
//			//プレイヤーの攻撃コリジョンと敵の距離
//			Vec2 distance = enemyPos - collisionPos;
//			float length = fabsf(distance.Length());
//			float scoreLength = collisionLength + enemyLength;
//			//加算スコアの計算
//			int plusScore = CalcScore(length, scoreLength);
//			//追加スコアとスコアの番号を検出
//			int addScoreNum = static_cast<int>(addScores.size());
//			//エフェクト作成
//			CreateHitEffect(enemy, plusScore);
//			//スコアEntityを作成
//			ECS::AddScoreArcheType()("font", Vec2(0, 50 + (addScoreNum % 3) * 32), plusScore);
//			if (totalScore->HasComponent<ECS::TotalScoreDraw>())
//			{
//				totalScore->GetComponent<ECS::TotalScoreDraw>().AddScore(plusScore);
//			}
//		}
//		//スコアの計算
//		static int CalcScore(const float distance, const float scoreLength)
//		{
//			int plusScore = 0;
//			if (distance >= scoreLength * 0.8f)
//			{
//				plusScore = 50;
//			}
//			else if (distance >= scoreLength * 0.6f)
//			{
//				plusScore = 100;
//			}
//			else if (distance >= scoreLength * 0.4f)
//			{
//				plusScore = 150;
//			}
//			else if (distance >= 0)
//			{
//				plusScore = 200;
//			}
//			return plusScore;
//		}
//		//プレイヤーとの当たり判定
//		static void HitPlayer()
//		{
//			const auto& player = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
//			if (player.size() == 0) { return; }
//			const auto& enemys = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::Enemy);
//			for (const auto& enemy : enemys)
//			{
//				if (Collision::BoxAndBox<ECS::HitBase, ECS::HitBase>(*player[0], *enemy))
//				{
//					if (player[0]->GetComponent<ECS::EntityHealthState>().GetCurrentState() == ECS::EntityHealthState::State::Damage) { return; }
//					player[0]->GetComponent<ECS::EntityHealthState>().ChangeState(ECS::EntityHealthState::State::Damage);
//					break;
//				}
//			}
//		}
//	public:
//		void Update() override 
//		{
//			Destory();
//			HitPlayer();
//			CreateDestoryEffect();
//		}
//	public:
//		void Initialize() override {}
//		void Draw2D() override {}
//	private:
//		void Draw3D() override {}
//	};
//}