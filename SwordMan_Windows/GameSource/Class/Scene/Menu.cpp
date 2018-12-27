#include "Menu.h"
#include "../../ArcheType/ArcheType.hpp"
#include "../../Class/DXFilieRead.hpp"
#include "../../Utility/Input.hpp"
namespace Scene
{
	void Menu::easingMove()
	{
		cL[0].Run(Easing::QuadOut, 60);
		cR[0].Run(Easing::QuadOut, 60);
		logo[0].Run(Easing::QuadOut, 60);
		hiscore[0].Run(Easing::QuadOut, 60);
		cursor_L->GetComponent<ECS::Position>().val.x = cL[0].GetVolume(-160, 0-(-160));
		cursor_R->GetComponent<ECS::Position>().val.x = cR[0].GetVolume(System::SCREEN_WIDIH, (System::SCREEN_WIDIH - 160.f) - (System::SCREEN_WIDIH));
		for (auto& it : stageUI)
		{
			it->GetComponent<ECS::Position>().val.y = logo[0].GetVolume(-160.f, System::SCREEN_HEIGHT / 3.f - (-160.f));
		}
		scoreBoard->GetComponent<ECS::Position>().val.y = hiscore[0].GetVolume(System::SCREEN_HEIGHT + 150.f, (System::SCREEN_HEIGHT - 140.f) - (System::SCREEN_HEIGHT + 150));

	}
	void Menu::easingOutMove()
	{
		cL[1].Run(Easing::QuadOut, 60);
		cR[1].Run(Easing::QuadOut, 60);
		logo[1].Run(Easing::QuadOut, 60);
		hiscore[1].Run(Easing::QuadOut, 60);
		cursor_L->GetComponent<ECS::Position>().val.x = cL[1].GetVolume(0, (-160) - (0));
		cursor_R->GetComponent<ECS::Position>().val.x = cR[1].GetVolume(System::SCREEN_WIDIH - 160.f, (System::SCREEN_WIDIH) - (System::SCREEN_WIDIH - 160.f));
		for (auto& it : stageUI)
		{
			it->GetComponent<ECS::Position>().val.y = logo[1].GetVolume(System::SCREEN_HEIGHT / 3.f, (-160.f) - (System::SCREEN_HEIGHT / 3.f));
		}
		scoreBoard->GetComponent<ECS::Position>().val.y = hiscore[1].GetVolume(System::SCREEN_HEIGHT - 140.f, (System::SCREEN_HEIGHT + 150.f) - (System::SCREEN_HEIGHT - 140.f));		
	}
	void Menu::indexAdd()
	{
		++index;
		if (index % MAX_STAGE_NUM == 0)
		{
			index = 0;
		}
	}
	void Menu::indexSub()
	{
		--index;
		if (index < 0)
		{
			index = 2;
		}
	}

	Menu::Menu(IOnSceneChangeCallback * sceneTitleChange, [[maybe_unused]]Parameter * parame)
		: AbstractScene(sceneTitleChange)
	{
#ifdef __ANDROID__
		// Android版のコンパイル
		stageLoader.LoadStage("stage/stageparam03.csv");
		ResourceManager::GetGraph().Load("image/menu/cursor.png", "cursor");
		ResourceManager::GetGraph().Load("image/menu/stage1.png", "stage1UI");
		ResourceManager::GetGraph().Load("image/menu/stage2.png", "stage2UI");
		ResourceManager::GetGraph().Load("image/menu/stage3.png", "stage3UI");
		ResourceManager::GetGraph().Load("image/menu/hiscore.png", "hiscore");
#else
		// Windows版のコンパイルだったら
		stageLoader.LoadStage("Resource/stage/stageparam03.csv");
		ResourceManager::GetGraph().Load("Resource/image/menu/cursor.png", "cursor");
		ResourceManager::GetGraph().Load("Resource/image/menu/stage1.png", "stage1UI");
		ResourceManager::GetGraph().Load("Resource/image/menu/stage2.png", "stage2UI");
		ResourceManager::GetGraph().Load("Resource/image/menu/stage3.png", "stage3UI");
		ResourceManager::GetGraph().Load("Resource/image/menu/hiscore.png", "hiscore");
#endif
		//平坦なのしか出さないのでステージパラメーターはなんでもいい
		
		stageLoader.LoadStageConstitution();
		//以下のようにしないと動的にマップチップを切り替えられない
		ResourceManager::GetGraph().RemoveGraph(stageLoader.GetStageParam().mapImage);
	
		const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage1";
		stageCreator.SetMapParam(stageLoader.GetStageParam());
		stageCreator.FillUpFlatMap();
		//ステージの生成
		stageCreator.Run(nullptr, nullptr, nullptr);
		cursor_L = ECS::ArcheType()("cursor", Vec2{ -160.f,300 }, ENTITY_GROUP::GameUI);
		cursor_R = ECS::ArcheType()("cursor", Vec2{ (float)System::SCREEN_WIDIH,300 }, ENTITY_GROUP::GameUI);
		cursor_R->GetComponent<ECS::SimpleDraw>().DoTurn(true);
		stageUI[0] = ECS::ArcheType()("stage1UI", Vec2{ System::SCREEN_WIDIH / 2.f, -160.f }, ENTITY_GROUP::GameUI);
		stageUI[0]->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		stageUI[1] = ECS::ArcheType()("stage2UI", Vec2{ System::SCREEN_WIDIH / 2.f, -160.f }, ENTITY_GROUP::GameUI);
		stageUI[1]->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		stageUI[1]->GetComponent<ECS::SimpleDraw>().DrawDisable();
		stageUI[2] = ECS::ArcheType()("stage3UI", Vec2{ System::SCREEN_WIDIH / 2.f, -160.f }, ENTITY_GROUP::GameUI);
		stageUI[2]->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		stageUI[2]->GetComponent<ECS::SimpleDraw>().DrawDisable();
		scoreBoard = ECS::ArcheType()("hiscore", Vec2{ System::SCREEN_WIDIH / 2.f,System::SCREEN_HEIGHT + 150.f }, ENTITY_GROUP::GameUI);
			scoreBoard->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		ECS::Cloud()("cloud");

		number = &ECS::EcsSystem::GetManager().AddEntity();
		number->AddComponent<ECS::Transform>().SetPosition(-220.f,0.f);
		number->AddComponent<ECS::ImageFontDraw>("font", Vec2(32, 32), 16);
		number->AddGroup(ENTITY_GROUP::GameUI);
		
		scoreBoard->AddComponent<ECS::Canvas>().AddChild(number);
		scoreBoard->GetComponent<ECS::Canvas>().OffsetChildScale(0,2.0f);

#ifdef __ANDROID__
		//セーブデータのロード
		//1
		{
			int stageNo = 1;
			std::string stageName = "stage" + stageNo;
			stageName += ".dat";
			FileSystem().Load(stageName, &score[0]);
			number->GetComponent<ECS::ImageFontDraw>().SetDrawData(Converter::ToString(score[0]).c_str());
		}
		//2
		{
			int stageNo = 2;
			std::string stageName = "stage" + stageNo;
			stageName += ".dat";
			FileSystem().Load(stageName, &score[1]);
		}
		//3
		{
			int stageNo = 3;
			std::string stageName = "stage" + stageNo;
			stageName += ".dat";
			FileSystem().Load(stageName, &score[2]);
		}
#else
		//セーブデータのロード
		//1
		{
			int stageNo = 1;
			std::string stageName = std::string("Resource/score/stage") + std::to_string(stageNo);
			stageName += ".dat";
			FileSystem().Load(stageName, &score[0]);
			number->GetComponent<ECS::ImageFontDraw>().SetDrawData(std::to_string(score[0]).c_str());
		}
		//2
		{
			int stageNo = 2;
			std::string stageName = std::string("Resource/score/stage") + std::to_string(stageNo);
			stageName += ".dat";
			FileSystem().Load(stageName, &score[1]);
		}
		//3
		{
			int stageNo = 3;
			std::string stageName = std::string("Resource/score/stage") + std::to_string(stageNo);
			stageName += ".dat";
			FileSystem().Load(stageName, &score[2]);
		}
#endif
	}
	void Menu::Finalize()
	{
		ResourceManager::GetGraph().RemoveGraph("cursor");
		ResourceManager::GetGraph().RemoveGraph("stage1UI");
		ResourceManager::GetGraph().RemoveGraph("stage2UI");
		ResourceManager::GetGraph().RemoveGraph("stage3UI");
		ResourceManager::GetGraph().RemoveGraph("hiscore");
		auto ui = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		for (auto& e : ui)
		{
			e->Destroy();
		}
		auto ground = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground);
		for (auto& e : ground)
		{
			e->Destroy();
		}
		auto sky = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back1);
		for (auto& e : sky)
		{
			e->Destroy();
		}
	}
	void Menu::Update()
	{
		ECS::EcsSystem::GetManager().Update();
		easingMove();
		for (auto& it : stageUI)
		{
			it->GetComponent<ECS::SimpleDraw>().DrawDisable();
		}
		preIndex = index;
		stageUI[index]->GetComponent<ECS::SimpleDraw>().DrawEnable();
		if (logo[0].IsEaseEnd())
		{
			//左端
			if (TouchInput::GetInput().GetBtnPress(0) == 1 &&
				Collision::BoxAndBox(
					TouchInput::GetInput().GetTouchIDPos(0), 
					Vec2{ 1.f,1.f },
					Vec2{ 0.f,0.f }, Vec2{ 160.f,720.f }) ||
				Input::Get().GetKeyFrame(KEY_INPUT_LEFT) == 1)
			{
				indexSub();
			}
			//右端
			else if (TouchInput::GetInput().GetBtnPress(0) == 1 &&
				Collision::BoxAndBox(
					TouchInput::GetInput().GetTouchIDPos(0), Vec2{ 1.f,1.f },
					Vec2{ System::SCREEN_WIDIH - 160.f,0.f },
					Vec2{ (float)System::SCREEN_WIDIH ,720.f }) ||
				Input::Get().GetKeyFrame(KEY_INPUT_RIGHT) == 1)
			{
				indexAdd();
			}
			//真ん中
			else if ((TouchInput::GetInput().GetBtnPress(0) == 1 ||
				Input::Get().GetKeyFrame(KEY_INPUT_Z) == 1))
			{
				isPlay = true;
			}
			
		}

		if (isPlay)
		{
			easingOutMove();

			if (logo[1].IsEaseEnd())
			{
				switch (index)
				{
				case 0:
				{
					CommonData::StageNum::val = 1;
					Finalize();
					GetCallback().OnSceneChange(SceneName::Game, nullptr, SceneStack::OneClear);
					return;
					break;
				}
				case 1:
				{
					CommonData::StageNum::val = 2;
					Finalize();
					GetCallback().OnSceneChange(SceneName::Game, nullptr, SceneStack::OneClear);
					return;
					break;
				}
				case 2:
				{
					CommonData::StageNum::val = 3;
					Finalize();
					GetCallback().OnSceneChange(SceneName::Game, nullptr, SceneStack::OneClear);
					return;
					break;
				}
				}
			}
		}

		if (index != preIndex && index == 2)
		{
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage3";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			auto entity = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground);

			for (auto& e : entity)
			{
				e->Destroy();
			}
			auto back = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back1);
			for (auto& b : back)
			{
				b->Destroy();
			}
			number->GetComponent<ECS::ImageFontDraw>().SetDrawData(std::to_string(score[2]).c_str());
			stageCreator.FillUpFlatMap();
		}
		else if (index != preIndex && index == 1)
		{
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage2";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			auto entity = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground);
			for (auto& e : entity)
			{
				e->Destroy();
			}
			auto back = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back1);
			for (auto& b : back)
			{
				b->Destroy();
			}
			number->GetComponent<ECS::ImageFontDraw>().SetDrawData(std::to_string(score[1]).c_str());
			stageCreator.FillUpFlatMap();
		}
		else if (index != preIndex && index == 0)
		{
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage1";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			auto entity = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground);
			for (auto& e : entity)
			{
				e->Destroy();
			}
			auto back = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back1);
			for (auto& b : back)
			{
				b->Destroy();
			}
			number->GetComponent<ECS::ImageFontDraw>().SetDrawData(std::to_string(score[0]).c_str());
			stageCreator.FillUpFlatMap();
		}
		stageCreator.Run(nullptr, nullptr, nullptr);
		cloud.Run();
	}
	void Menu::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}