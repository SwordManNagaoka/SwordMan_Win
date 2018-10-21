#include "Title.h"
#include "../../GameController/GameController.h"
#include "../../ECS/ECS.hpp"

//クラス
#include "../../Class/TouchInput.hpp"
#include "../../Class/Scene/SceneManager.hpp"
#include "../../Utility/Input.hpp"
#include "../../ResourceManager/ResourceManager.hpp"
//アーキタイプ
#include "../../ArcheType/Cloud.hpp"
#include "../../ArcheType/Button.hpp"
#include "../../ArcheType/Player.hpp"
//コンポーネント


namespace Scene
{
	Title::Title(IOnSceneChangeCallback* sceneTitleChange, const Parameter& parame)
		: AbstractScene(sceneTitleChange)
	{
		stageLoader.LoadStage("Resource/stage/mapparamtest.csv");
		stageLoader.LoadStageConstitution();
		stageCreator.SetMapParam(stageLoader.GetStageParam());
		stageCreator.FillUpFlatMap();
		//ステージの生成
		stageCreator.Run(nullptr, nullptr, nullptr);
		ECS::Cloud()("cloud");
		ECS::Entity* startLogo = &ECS::EcsSystem::GetManager().AddEntity();
		startLogo->AddComponent<ECS::Position>(Vec2(500, 400));
		startLogo->AddComponent<ECS::ImageFontDraw>("font",Vec2(32,32),16).SetDrawData("TapToStart");
		startLogo->AddGroup(ENTITY_GROUP::GameUI);
	}
	Title::~Title()
	{
		//すべてのEntityを殺す処理があると便利
		auto entity = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		for (auto& e : entity)
		{
			e->Destroy();
		}
	}
	
	void Title::Update()
	{
		cloud.Run();
		stageCreator.Run(nullptr, nullptr, nullptr);
		ECS::EcsSystem::GetManager().Update();
		if(Input::Get().GetKeyFrame(KEY_INPUT_A) == 1)
		//if (TouchInput::GetInput().GetBtnPress(0) == 1)
		{
			Parameter param;
			ECS::PlayerArcheType()(Vec2(-150, 300), Vec2(64, 96));
			callBack->OnSceneChange(SceneName::Game, param,SceneStack::OneClear);
			return;
		}
	}

	void Title::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}