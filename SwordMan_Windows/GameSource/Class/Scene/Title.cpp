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
	Title::Title(IOnSceneChangeCallback* sceneTitleChange, Parameter* parame)
		: AbstractScene(sceneTitleChange)
	{
		stageLoader.LoadStage("Resource/stage/stageparam01.csv");
		stageLoader.LoadStageConstitution();
		ResourceManager::GetGraph().RemoveGraph(stageLoader.GetStageParam().mapImage);
		ResourceManager::GetGraph().Load("Resource/image/ground01.png", "stage1");
		ResourceManager::GetGraph().Load("Resource/image/ground03.png", "stage3");
		const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage1";
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
		ECS::EcsSystem::GetManager().AllKill();
	}
	
	void Title::Update()
	{
		cloud.Run();
		stageCreator.Run(nullptr, nullptr, nullptr);
		ECS::EcsSystem::GetManager().Update();
		if (TouchInput::GetInput().GetBtnPress(0) == 1 ||
			Input::Get().GetKeyFrame(KEY_INPUT_Z)==1)
		{
			GetCallback().OnSceneChange(SceneName::Menu, nullptr, SceneStack::OneClear);
			return;
		}
	}

	void Title::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}