#include "Result.h"

//アーキタイプ
#include "../../ArcheType/Button.hpp"
//コンポーネント
#include "../../Components/ImageFontDraw.hpp"
#include "../../Components/BlendMode.hpp"
#include "../../Utility/Converter.hpp"
#include "../../Components/GradationColor.hpp"
#include "../../Components/EasingMove.hpp"

#include <fstream>
#include "../../Class/DXFilieRead.hpp"
#include "../../Class/RankSelector.hpp"
#include "../../Utility/Input.hpp"

namespace Scene
{
	Result::Result(IOnSceneChangeCallback* sceneTitleChange, Parameter* parame)
		: AbstractScene(sceneTitleChange)
	{
		

		backFade = &ECS::EcsSystem::GetManager().AddEntity();
		backFade->AddComponent<ECS::Position>(0, 0);
		backFade->AddComponent<ECS::AlphaBlend>().alpha = 60;
		backFade->AddComponent<ECS::SimpleDraw>("fade");
		
		backFade->AddGroup(ENTITY_GROUP::Fade0);

		scoreData = CommonData::TotalScore::val;
		int stageNo = CommonData::StageNum::val;

		//--- クリアフラグのセーブ ---//
		std::string stageClearFileName = "stageClearFile" + stageNo;
		stageClearFileName += std::string(".dat");
		if (CommonData::ClearFlagData::val == 1)
		{
			FileSystem().Save(stageClearFileName, &CommonData::ClearFlagData::val);
		}
		CommonData::ClearFlagData::val = 0;

		//---スコアの表示---//
		scoreUI = &ECS::EcsSystem::GetManager().AddEntity();
		scoreUI->AddComponent<ECS::Color>(0,0,255);
		scoreUI->AddComponent<ECS::Position>(Vec2(-300,100));
		scoreUI->AddComponent<ECS::ImageFontDraw>("font",Vec2(32,32),16).SetFontImageKind(false);
		scoreUI->GetComponent<ECS::ImageFontDraw>().SetDrawData("Score");
		scoreUI->GetComponent<ECS::Scale>().val = 2.0f;
		scoreUI->AddComponent<ECS::EasingMove>(Easing::ExpoInOut,ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(-300, 200);
		scoreUI->GetComponent<ECS::EasingMove>().SetTimeToDuration(20.0f, 80.0f);
		scoreUI->AddGroup(ENTITY_GROUP::GameUI);

		ECS::Entity* scoreParam = &ECS::EcsSystem::GetManager().AddEntity();
		scoreParam->AddComponent<ECS::Color>(255, 0, 0);
		scoreParam->AddComponent<ECS::Position>(Vec2(1400, 100));
		scoreParam->AddComponent<ECS::ImageFontDraw>("font", Vec2(32, 32), 16).SetFontImageKind(false);
#if _Android_
		scoreParam->GetComponent<ECS::ImageFontDraw>().SetDrawData(Converter::ToString(scoreData).c_str());
#else
		scoreParam->GetComponent<ECS::ImageFontDraw>().SetDrawData(std::to_string(scoreData).c_str());
#endif
		scoreParam->GetComponent<ECS::Scale>().val = 2.0f;
		scoreParam->AddComponent<ECS::GradationColor>().SetGradationPower(Vec3(2, 1, 4));
		scoreParam->AddComponent<ECS::EasingMove>(Easing::ExpoInOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(1400, 800);
		scoreParam->GetComponent<ECS::EasingMove>().SetTimeToDuration(20.0f, 80.0f);
		scoreParam->AddGroup(ENTITY_GROUP::GameUI);
		
		//---ランク表示---//
		rankUI = &ECS::EcsSystem::GetManager().AddEntity();
		rankUI->AddComponent<ECS::Color>(0, 0, 255);
		rankUI->AddComponent<ECS::Position>(Vec2(-400,300));
		rankUI->AddComponent<ECS::ImageFontDraw>("font", Vec2(32, 32), 16).SetFontImageKind(false);
		rankUI->GetComponent<ECS::ImageFontDraw>().SetDrawData("Rank");
		rankUI->GetComponent<ECS::Scale>().val = 2.0f;
		rankUI->AddComponent<ECS::EasingMove>(Easing::ExpoInOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(-400, 200);
		rankUI->GetComponent<ECS::EasingMove>().SetTimeToDuration(40.0f, 130.0f);
		rankUI->AddGroup(ENTITY_GROUP::GameUI);
		
		std::string rankName = RankSelector().execute(scoreData);

		ECS::Entity* rankData = &ECS::EcsSystem::GetManager().AddEntity();
		rankData->AddComponent<ECS::Color>(255, 0, 0);
		rankData->AddComponent<ECS::Position>(Vec2(1400, 300));
		rankData->AddComponent<ECS::ImageFontDraw>("font", Vec2(32, 32), 16).SetFontImageKind(false);
		rankData->AddComponent<ECS::EasingMove>(Easing::ExpoInOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(1400, 800);
		rankData->GetComponent<ECS::EasingMove>().SetTimeToDuration(40.0f, 130.0f);
		rankData->GetComponent<ECS::ImageFontDraw>().SetDrawData(rankName.c_str());
		rankData->GetComponent<ECS::Scale>().val = 2.0f;
		rankData->AddComponent<ECS::GradationColor>().SetGradationPower(Vec3(1,3,5));
		rankData->AddGroup(ENTITY_GROUP::GameUI);
#ifdef __ANDROID__
		stageName = "stage" + stageNo;
		stageName += ".dat";
#else
		stageName = std::string("Resource/score/stage") + std::to_string(stageNo);
		stageName += ".dat";
#endif
	}

	Result::~Result()
	{
		ECS::EcsSystem::GetManager().AllKill();
		ResourceManager::GetSound().Remove("BGM");
	}
	
	void Result::Update()
	{	
		const auto& button = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		const auto& player = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
		for (const auto& it : player) { it->Update(); }
		for (auto& b : button) { b->Update(); }

		//---更新した場合に新記録の表示---//
		if (FileSystem().HighScoreSave(stageName, &scoreData) && scoreUI->GetComponent<ECS::EasingMove>().IsEnd() && !isScoreMoveEnd)
		{
			isScoreMoveEnd = true;
			ECS::Entity* newRecord = &ECS::EcsSystem::GetManager().AddEntity();
			newRecord->AddComponent<ECS::Color>(128, 128, 0);
			newRecord->AddComponent<ECS::Position>(Vec2(1400, 200));
			newRecord->AddComponent<ECS::ImageFontDraw>("font", Vec2(32, 32), 16).SetFontImageKind(false);
			newRecord->GetComponent<ECS::ImageFontDraw>().SetDrawData("NEW RECORD");
			newRecord->GetComponent<ECS::Scale>().val = 2.0f;
			newRecord->AddComponent<ECS::GradationColor>().SetGradationPower(Vec3(3, 2, 1));
			newRecord->AddComponent<ECS::EasingMove>(Easing::ExpoInOut, ECS::EasingMove::DirectionState::LeftAndRight).SetBeginToEndPoint(1400, 350);
			newRecord->GetComponent<ECS::EasingMove>().SetTimeToDuration(40.0f, 130.0f);
			newRecord->AddGroup(ENTITY_GROUP::GameUI);
		}

		//ボタン表示
		if (rankUI->GetComponent<ECS::EasingMove>().IsEnd() && !isAddbtn)
		{
			isAddbtn = true;
			//ボタン生成
			ECS::Entity* retry = ECS::ButtonArcheType()("pauseUI", Vec2{ 680.f, 430.f }, Vec2{ 0.f, 0.f }, Vec2{ 96.f, 144.f }, 50.f);
			retry->GetComponent<ECS::CircleColiider>().SetOffset(48.f, 48.f);
			retry->AddComponent<ECS::RetryButtonTag>();
			retry->AddGroup(ENTITY_GROUP::GameUI);

			ECS::Entity* menuBtn = ECS::ButtonArcheType()("pauseUI", Vec2{ 480.f, 430.f }, Vec2{ 192.f, 0.f }, Vec2{ 96.f, 144.f }, 50.f);
			menuBtn->GetComponent<ECS::CircleColiider>().SetOffset(48.f, 48.f);
			menuBtn->AddComponent<ECS::BackMenuButtonTag>();
			menuBtn->AddGroup(ENTITY_GROUP::GameUI);
			//Retryボタンにイベントを設定
			{
				auto changeFunc = [=]([[maybe_unused]] Scene::IOnSceneChangeCallback* callBack)
				{
					//ここでシーン切り替えするとなぜか落ちるのでこうなった
					isGame = true;
				};
				retry->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
			//メニューボタンにイベントを設定
			{
				auto changeFunc = [=]([[maybe_unused]] Scene::IOnSceneChangeCallback* callBack)
				{
					//ここでシーン切り替えするとなぜか落ちるのでこうなった
					isMenu = true;
				};
				menuBtn->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
		}
		if (Input::Get().GetKeyFrame(KEY_INPUT_S) == 1 || isGame)
		{
			GetCallback().OnSceneChange(SceneName::Game, nullptr, SceneStack::AllClear);
			return;
		}
		else if (Input::Get().GetKeyFrame(KEY_INPUT_A) == 1 || isMenu)
		{
			GetCallback().OnSceneChange(SceneName::Menu, nullptr, SceneStack::AllClear);
			return;
		}
	}
	void Result::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}