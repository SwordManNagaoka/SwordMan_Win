#include "Menu.h"
#include "../../ArcheType/ArcheType.hpp"
#include "../../Utility/Input.hpp"
namespace Scene
{
	void Menu::indexAdd()
	{
		++index;
		if (index % 3 == 0)
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

	Menu::Menu(IOnSceneChangeCallback * sceneTitleChange, Parameter * parame)
		: AbstractScene(sceneTitleChange)
	{
		//平坦なのしか出さないのでステージパラメーターはなんでもいい
		stageLoader.LoadStage("Resource/stage/stageparam03.csv");
		stageLoader.LoadStageConstitution();
		//以下のようにしないと動的にマップチップを切り替えられない
		ResourceManager::GetGraph().RemoveGraph(stageLoader.GetStageParam().mapImage);
		ResourceManager::GetGraph().Load("Resource/image/ground01.png", "stage1");
		ResourceManager::GetGraph().Load("Resource/image/ground02.png", "stage2");
		ResourceManager::GetGraph().Load("Resource/image/ground03.png", "stage3");
		ResourceManager::GetGraph().Load("Resource/image/menu/cursor.png", "cursor");
		ResourceManager::GetGraph().Load("Resource/image/menu/stage1.png", "stage1UI");
		ResourceManager::GetGraph().Load("Resource/image/menu/stage2.png", "stage2UI");
		ResourceManager::GetGraph().Load("Resource/image/menu/stage3.png", "stage3UI");
		ResourceManager::GetGraph().Load("Resource/image/menu/hiscore.png", "hiscore");
		const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage1";
		stageCreator.SetMapParam(stageLoader.GetStageParam());
		stageCreator.FillUpFlatMap();
		//ステージの生成
		stageCreator.Run(nullptr, nullptr, nullptr);
		cursor_L = ECS::ArcheType()("cursor", Vec2{ 0.f,300 }, ENTITY_GROUP::GameUI);
		cursor_R = ECS::ArcheType()("cursor", Vec2{ System::SCREEN_WIDIH - 160.f,300 }, ENTITY_GROUP::GameUI);
		cursor_R->GetComponent<ECS::SimpleDraw>().DoTurn(true);
		stageUI[0] = ECS::ArcheType()("stage1UI", Vec2{ System::SCREEN_WIDIH / 2.f,System::SCREEN_HEIGHT / 2.6f }, ENTITY_GROUP::GameUI);
		stageUI[0]->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		stageUI[1] = ECS::ArcheType()("stage2UI", Vec2{ System::SCREEN_WIDIH / 2.f,System::SCREEN_HEIGHT / 2.6f }, ENTITY_GROUP::GameUI);
		stageUI[1]->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		stageUI[1]->GetComponent<ECS::SimpleDraw>().DrawDisable();
		stageUI[2] = ECS::ArcheType()("stage3UI", Vec2{ System::SCREEN_WIDIH / 2.f,System::SCREEN_HEIGHT / 2.6f }, ENTITY_GROUP::GameUI);
		stageUI[2]->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		stageUI[2]->GetComponent<ECS::SimpleDraw>().DrawDisable();
		ECS::ArcheType()("hiscore", Vec2{ System::SCREEN_WIDIH / 2.f,System::SCREEN_HEIGHT - 140.f}, ENTITY_GROUP::GameUI)
			->GetComponent<ECS::SimpleDraw>().DoCenter(true);
		ECS::Cloud()("cloud");
	}
	void Menu::Finalize()
	{
		ResourceManager::GetGraph().RemoveGraph("cursor");
		auto entity = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		for (auto& e : entity)
		{
			e->Destroy();
		}
	}
	void Menu::Update()
	{
		ECS::EcsSystem::GetManager().Update();
		for (auto& it : stageUI)
		{
			it->GetComponent<ECS::SimpleDraw>().DrawDisable();
		}
		preIndex = index;
		stageUI[index]->GetComponent<ECS::SimpleDraw>().DrawEnable();
		//左端
		if ((TouchInput::GetInput().GetBtnPress(0) == 1 &&
			Collision::BoxAndBox(
				TouchInput::GetInput().GetTouchIDPos(0), Vec2{ 1.f,1.f },
				Vec2{0.f,0.f}, Vec2{ 160.f,720.f })) ||
			Input::Get().GetKeyFrame(KEY_INPUT_LEFT) == 1)
		{
			indexSub();
		}
		//右端
		else if ((TouchInput::GetInput().GetBtnPress(0) == 1 &&
			Collision::BoxAndBox(
				TouchInput::GetInput().GetTouchIDPos(0), Vec2{ 1.f,1.f }, Vec2{ System::SCREEN_WIDIH - 160.f,0.f }, 
				Vec2{ (float)System::SCREEN_WIDIH ,720.f })) ||
			 Input::Get().GetKeyFrame(KEY_INPUT_RIGHT) == 1 )
		{
			indexAdd();
		}
		//真ん中
		else if(TouchInput::GetInput().GetBtnPress(0) == 1 ||
			Input::Get().GetKeyFrame(KEY_INPUT_Z) == 1)
		{
			switch (index)
			{
			case 0:
			{
				auto param = std::make_unique<Parameter>();
				param->Set<int>("stageNum", 1);
				CommonData::StageNum::val = 1;
				Finalize();
				GetCallback().OnSceneChange(SceneName::Game, param.get(), SceneStack::Non);
				return;
				break;
			}
			case 1:
			{
				auto param = std::make_unique<Parameter>();
				param->Set<int>("stageNum", 2);
				CommonData::StageNum::val = 2;
				Finalize();
				GetCallback().OnSceneChange(SceneName::Game, param.get(), SceneStack::Non);
				return;
				break;
			}
			case 2:
			{
				auto param = std::make_unique<Parameter>();
				param->Set<int>("stageNum", 3);
				CommonData::StageNum::val = 3;
				Finalize();
				GetCallback().OnSceneChange(SceneName::Game, param.get(), SceneStack::Non);
				return;
				break;
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
			stageCreator.FillUpFlatMap();
		}
		else if (index != preIndex &&  index == 0)
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
			stageCreator.FillUpFlatMap();
		}
		else if(index != preIndex && index == 1)
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
			stageCreator.FillUpFlatMap();
		}
		stageCreator.Run(nullptr, nullptr, nullptr);
		cloud.Run();
	}
	void Menu::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
		DrawFormatString(0, 120, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground).size());
		DrawFormatString(0, 140, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back0).size());
		DrawFormatString(0, 160, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back1).size());
		DrawFormatString(0, 180, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back2).size());
		DrawFormatString(0, 200, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back3).size());
		DrawFormatString(0, 220, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Gimmick).size());
		DrawFormatString(0, 240, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Wepon).size());
		DrawFormatString(0, 260, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player).size());
		DrawFormatString(0, 280, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Enemy).size());
		DrawFormatString(0, 300, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Effect).size());
		DrawFormatString(0, 320, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI).size());
		DrawFormatString(0, 340, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Fade1).size());
		DrawFormatString(0, 360, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::PauseUI).size());
		DrawFormatString(0, 380, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Fade2).size());
		DrawFormatString(0, 400, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Max).size());
		DrawFormatString(0, 420, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetMaxEntityesSize());

	}
}