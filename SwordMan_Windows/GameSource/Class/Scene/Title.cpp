#include "Title.h"
#include "../../GameController/GameController.h"
#include "../../ECS/ECS.hpp"

//クラス
#include "../../Class/TouchInput.hpp"
#include "../../Class/Scene/SceneManager.hpp"
#include "../../Utility/Input.hpp"
#include "../../ResourceManager/ResourceManager.hpp"
//アーキタイプ
#include "../../ArcheType/Button.hpp"
#include "../../ArcheType/Player.hpp"
//コンポーネント


namespace Scene
{
	Title::Title(IOnSceneChangeCallback* sceneTitleChange, const Parameter& parame)
		: AbstractScene(sceneTitleChange)
	{
		ECS::ButtonArcheType()("pauseButton", Vec2(300, 300), Vec2(0, 0), Vec2(98, 98), 50)->AddGroup(ENTITY_GROUP::GameUI);
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
		if (TouchInput::GetInput().Push(0))
		{
			Parameter param;
			callBack->OnSceneChange(SceneName::Game, param, false);
		}
		ECS::EcsSystem::GetManager().Update();
		if (Input::Get().GetKeyFrame(KEY_INPUT_S) == 1)
		{
			Parameter param;
			ECS::PlayerArcheType()(Vec2(250, 300), Vec2(64, 96));
			callBack->OnSceneChange(SceneName::Game, param, true);
			return;
		}
	}

	void Title::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}