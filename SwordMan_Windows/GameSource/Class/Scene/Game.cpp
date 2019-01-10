#include "Game.h"
#include "../../Events/AtackEvent.hpp"
#include "../../GameController/GameController.h"
#include "../../ECS/ECS.hpp"
#include "../../ArcheType/Map.hpp"
#include "../../ArcheType/Player.hpp"
#include "../../Events/EventManager.hpp"
#include "../../Class/Scene/SceneManager.hpp"
#include "../../Class/TouchInput.hpp"
#include "../../ArcheType/Enemy.hpp"
#include "../../ArcheType/HealthUI.hpp"
#include "../../ArcheType/Score.hpp"
#include "../../ArcheType/Button.hpp"
#include "../../Events/AtackEvent.hpp"
#include "../../Utility/Input.hpp"
#include "../../Components/GradationColor.hpp"

namespace Scene
{
	Game::Game(IOnSceneChangeCallback* sceneTitleChange, [[maybe_unused]] Parameter* parame)
		: AbstractScene(sceneTitleChange)
	{

		switch (CommonData::StageNum::val)
		{
		case 1:
		{
#ifdef __ANDROID__
			// Android版のコンパイル
			stageLoader.LoadStage("stage/stageparam01.csv");
			ResourceManager::GetSound().Load("sounds/nagaoka.wav", "BGM", SoundType::BGM);
#else
			// Windows版のコンパイル
			stageLoader.LoadStage("Resource/stage/stageparam01.csv");
			ResourceManager::GetSound().Load("Resource/sounds/nagaoka.wav", "BGM", SoundType::BGM);
#endif
			stageLoader.LoadStageConstitution();
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage1";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			stageCreator.FillUpFlatMap();
			Sound s("BGM");
			s.Play(true, false);

			//--- explain backscreen ---//
			ECS::Entity* leftExplainScreen = &ECS::EcsSystem::GetManager().AddEntity();
			leftExplainScreen->AddComponent<ECS::Transform>().SetPosition(-1000,0);
			leftExplainScreen->AddComponent<ECS::Color>(0,128,200);
			leftExplainScreen->AddComponent<ECS::RectDraw>("colorScreen",0,0, System::SCREEN_WIDIH /2,System::SCREEN_HEIGHT);
			leftExplainScreen->AddComponent<ECS::BlendMode>().SetAlpha(128.0f);
			leftExplainScreen->AddComponent<ECS::EasingMove>(Easing::ExpoOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(-1000, 0);
			leftExplainScreen->GetComponent<ECS::EasingMove>().SetTimeToDuration(20.0f, 80.0f);
			leftExplainScreen->AddComponent<ECS::EntityCounter>(200);
			leftExplainScreen->AddGroup(ENTITY_GROUP::GameUI);
			ECS::Entity* rightExplainScreen = &ECS::EcsSystem::GetManager().AddEntity();
			rightExplainScreen->AddComponent<ECS::Transform>().SetPosition(2000,0);
			rightExplainScreen->AddComponent<ECS::Color>(200,128,0);
			rightExplainScreen->AddComponent<ECS::RectDraw>("colorScreen",0,0, System::SCREEN_WIDIH / 2, System::SCREEN_HEIGHT);
			rightExplainScreen->AddComponent<ECS::BlendMode>().SetAlpha(128.0f);
			rightExplainScreen->AddComponent<ECS::EasingMove>(Easing::ExpoOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(2000, System::SCREEN_WIDIH / 2);
			rightExplainScreen->GetComponent<ECS::EasingMove>().SetTimeToDuration(20.0f, 80.0f);
			rightExplainScreen->AddComponent<ECS::EntityCounter>(200);
			rightExplainScreen->AddGroup(ENTITY_GROUP::GameUI);
			//--- explain text ---//
			ECS::Entity* textJump = &ECS::EcsSystem::GetManager().AddEntity();
			textJump->AddComponent<ECS::Transform>().SetPosition(-1000,100);
			textJump->AddComponent<ECS::Color>(50,75,180);
			textJump->GetComponent<ECS::Scale>().val = 2.0f;
			textJump->AddComponent<ECS::GradationColor>().SetGradationPower(Vec3(3, 2, 1));
			textJump->AddComponent<ECS::ImageFontDraw>("font",Vec2(32,32),16).SetDrawData("Jump");
			textJump->AddComponent<ECS::EasingMove>(Easing::ExpoOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(-1000, 200);
			textJump->GetComponent<ECS::EasingMove>().SetTimeToDuration(20.0f, 80.0f);
			textJump->AddComponent<ECS::EntityCounter>(200);
			textJump->AddGroup(ENTITY_GROUP::GameUI);
			ECS::Entity* textAttack = &ECS::EcsSystem::GetManager().AddEntity();
			textAttack->AddComponent<ECS::Transform>().SetPosition(2000,100);
			textAttack->AddComponent<ECS::Color>(90,180,35);
			textAttack->GetComponent<ECS::Scale>().val = 2.0f;
			textAttack->AddComponent<ECS::GradationColor>().SetGradationPower(Vec3(2, 1, 3));
			textAttack->AddComponent<ECS::ImageFontDraw>("font", Vec2(32, 32), 16).SetDrawData("Attack");
			textAttack->AddComponent<ECS::EasingMove>(Easing::ExpoOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(2000, 800);
			textAttack->GetComponent<ECS::EasingMove>().SetTimeToDuration(20.0f, 80.0f);
			textAttack->AddComponent<ECS::EntityCounter>(200);
			textAttack->AddGroup(ENTITY_GROUP::GameUI);
			//--- explain tapImage ---//
			ECS::Entity* tapLeftImage = &ECS::EcsSystem::GetManager().AddEntity();
			tapLeftImage->AddComponent<ECS::Transform>().SetPosition(-1000,300);
			tapLeftImage->AddComponent<ECS::Direction>();
			tapLeftImage->AddComponent<ECS::Color>(255, 255, 255);
			tapLeftImage->AddComponent<ECS::AlphaBlend>().alpha = 200.0f;
			tapLeftImage->AddComponent<ECS::AnimationDraw>("arrowKey");
			tapLeftImage->AddComponent<ECS::AnimationController>(30,2);
			tapLeftImage->AddComponent<ECS::EasingMove>(Easing::ExpoOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(-1000, 250);
			tapLeftImage->GetComponent<ECS::EasingMove>().SetTimeToDuration(20.0f, 80.0f);
			tapLeftImage->AddComponent<ECS::EntityCounter>(200);
			tapLeftImage->AddGroup(ENTITY_GROUP::GameUI);
			ECS::Entity* tapRightImage = &ECS::EcsSystem::GetManager().AddEntity();
			tapRightImage->AddComponent<ECS::Transform>().SetPosition(2000, 300);
			tapRightImage->AddComponent<ECS::Direction>();
			tapRightImage->AddComponent<ECS::Color>(255,255,255);
			tapRightImage->AddComponent<ECS::AlphaBlend>().alpha = 200.0f;
			tapRightImage->AddComponent<ECS::AnimationDraw>("arrowKey");
			tapRightImage->AddComponent<ECS::AnimationController>(30,2).SetIsHeightAnimation(true);
			tapRightImage->GetComponent<ECS::AnimationController>().SetHeightAnimation(60,1,1);
			tapRightImage->AddComponent<ECS::EasingMove>(Easing::ExpoOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(2000, 950);
			tapRightImage->GetComponent<ECS::EasingMove>().SetTimeToDuration(20.0f, 80.0f);
			tapRightImage->AddComponent<ECS::EntityCounter>(200);
			tapRightImage->AddGroup(ENTITY_GROUP::GameUI);
			break;
		}
		case 2:
		{
#ifdef __ANDROID__
			// Android版のコンパイル
			stageLoader.LoadStage("stage/stageparam02.csv");
			ResourceManager::GetSound().Load("sounds/nagaoka.wav", "BGM", SoundType::BGM);
#else
			// Windows版のコンパイル
			stageLoader.LoadStage("Resource/stage/stageparam02.csv");
			ResourceManager::GetSound().Load("Resource/sounds/nagaoka.wav", "BGM", SoundType::BGM);
#endif

			stageLoader.LoadStageConstitution();
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage2";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			stageCreator.FillUpFlatMap();
			Sound s("BGM");
			s.Play(true, false);
			break;
		}
		case 3:
		{
#ifdef __ANDROID__
			// Android版のコンパイル
			stageLoader.LoadStage("stage/stageparam03.csv");
			ResourceManager::GetSound().Load("sounds/nagaoka.wav", "BGM", SoundType::BGM);
#else
			// Windows版のコンパイル
			stageLoader.LoadStage("Resource/stage/stageparam03.csv");
			ResourceManager::GetSound().Load("Resource/sounds/nagaoka.wav", "BGM", SoundType::BGM);
#endif
			stageLoader.LoadStageConstitution();
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage3";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			stageCreator.FillUpFlatMap();
			Sound s("BGM");
			s.Play(true, false);
			break;
		}
		default:
			break;
		}

		//Entityの生成
		ECS::PlayerArcheType()(Vec2(-150.f, 300.f), Vec2(64, 96));
		for (float i = 0; i < 3; ++i)
		{
			ECS::HealthUIArcheType()(i, Vec2(450.f + i * 144.f, 640.f));
		}
		//トータルスコアの生成
		ECS::TotalScoreArcheType()("font", Vec2(0, 0));
		//ポーズボタン生成
		ECS::Entity* pauseBtn = ECS::ButtonArcheType()("pauseButton", Vec2(1280.f - 96.f, 0.f), Vec2(0.f, 0.f), Vec2(96.f, 96.f), 50.f);
		pauseBtn->AddComponent<ECS::PauseButtonTag>();
		pauseBtn->AddGroup(ENTITY_GROUP::GameUI);
#ifdef __ANDROID__
		// Android版のコンパイル
		//非アクティブ時にポーズ画面に移行する
		SetAndroidLostFocusCallbackFunction([](void* ptr)
		{
			auto callback = static_cast<IOnSceneChangeCallback*>(ptr);
			if (CommonData::CurrentScene::val == SceneName::Game)
			{
				callback->OnSceneChange(SceneName::Pause, nullptr, SceneStack::Non);
				return;
			}
		},
			&GetCallback());
#endif

	}
	Game::~Game()
	{
		ECS::EcsSystem::GetManager().AllKill();
	}

	void Game::Update()
	{
		cloud.Run();
		stageCreator.Run(&stageLoader.GetStageData(), &stageLoader.GetSkyData(), &stageLoader.GetEnemyData());
		auto& player = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
		auto& ground = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground);
		auto& gameUI = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		//まれにめり込んだ状態から始まり、ジャンプできなくなるので苦肉の策としてこうしてある
		if (!isIntrusion)
		{
			for (const auto& p : player)
			{
				for (const auto& g : ground)
				{
					if (Collision::BoxAndBox<ECS::FootBase, ECS::HitBase>(*p, *g))
					{
						p->GetComponent<ECS::Position>().val.y -= 60;
						isIntrusion = true;
					}
				}
			}
		}
		//地形との衝突応答を行う
		for (const auto& p : player)
		{
			p->GetComponent<ECS::Physics>().PushOutEntity(ground);
			p->GetComponent<ECS::Physics>().SetCollisionFunction(Collision::BoxAndBox<ECS::FootBase, ECS::HitBase>);
		}
		auto& enemy = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Enemy);
		for (const auto& e : enemy)
		{
			if (e->HasComponent<ECS::Physics>())
			{
				e->GetComponent<ECS::Physics>().PushOutEntity(ground);
				e->GetComponent<ECS::Physics>().SetCollisionFunction(Collision::BoxAndBox<ECS::HitBase, ECS::HitBase>);
			}
		}
		Event::CollisionEvent::AttackCollisionToEnemy();
		Event::CollisionEvent::PlayerToEnemy();
		ECS::EcsSystem::GetManager().Update();

		//ゲーム説明が終了したときの処理
		for (const auto& ui : gameUI)
		{
			if (!ui->HasComponent<ECS::EntityCounter>()) { continue; }
			if (!ui->HasComponent<ECS::EasingMove>()) { continue; }

			if (ui->GetComponent<ECS::EntityCounter>().IsSpecifyCnt())
			{
				//画面の右側か左側でオブジェクトを判断
				int moveEndPos = 2000;
				if (ui->GetComponent<ECS::Position>().val.x < System::SCREEN_WIDIH / 2) { moveEndPos = -1000; }
				ui->DeleteComponent<ECS::EasingMove>();
				ui->AddComponent<ECS::EasingMove>(Easing::ExpoIn, ECS::EasingMove::DirectionState::LeftAndRight);
				ui->GetComponent<ECS::EasingMove>().SetBeginToEndPoint(ui->GetComponent<ECS::Position>().val.x, moveEndPos);
				ui->GetComponent<ECS::EasingMove>().SetTimeToDuration(20.0f, 80.0f);
				ui->AddComponent<ECS::KillEntity>(180);
			}
		}

		//キーイベント(仮)
		if (Input::Get().GetKeyFrame(KEY_INPUT_SPACE) == 1)
		{
			GetCallback().OnSceneChange(SceneName::Pause, nullptr, SceneStack::Non);
			return;
		}
		//ボタンイベント
		for (auto& b : gameUI)
		{
			if (b->HasComponent<ECS::PauseButtonTag>())
			{
				b->GetComponent<ECS::PushButton>().SetSceneCallBack(&GetCallback());
				auto changeFunc = [](Scene::IOnSceneChangeCallback* callBack)
				{
					callBack->OnSceneChange(SceneName::Pause, nullptr, SceneStack::Non);
				/*	auto& gameUI = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
					for (auto& b : gameUI)
					{
						if (b->HasComponent<ECS::PauseButtonTag>()) { b->Destroy(); }
					}*/
					return;
				};
				b->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
			//スコア
			for (auto& ui : gameUI)
			{
				if (ui->HasComponent<ECS::TotalScoreDraw>())
				{
					CommonData::TotalScore::val = ui->GetComponent<ECS::TotalScoreDraw>().GetTotalScore();
				}
			}
		}
		//プレイヤー死亡時
		if (!player[0]->IsActive() && !player.empty())
		{
			for (auto& ui : gameUI)
			{
				if (ui->HasComponent<ECS::PauseButtonTag>())
				{
					ui->Destroy();
				}
				else if (ui->HasComponent<ECS::TotalScoreDraw>())
				{
					CommonData::TotalScore::val = ui->GetComponent<ECS::TotalScoreDraw>().GetTotalScore();
				}
			}
			//クリアフラグ(失敗)
			if (CommonData::ClearFlagData::val != 1)
			{
				CommonData::ClearFlagData::val = 0;
			}
#ifdef __ANDROID__
			ResourceManager::GetGraph().LoadDiv("image/death.png", "death", 2, 2, 1, 96, 192);
#else
			ResourceManager::GetGraph().LoadDiv("Resource/image/death.png", "death", 2, 2, 1, 96, 192);
#endif
			ECS::PlayerDeathArcheType()("death", Vec2{ player[0]->GetComponent<ECS::Position>().val });
			GetCallback().OnSceneChange(SceneName::Result, nullptr, SceneStack::Non);
			return;
		}
	}
	void Game::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}
