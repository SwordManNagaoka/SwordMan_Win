#include "Title.h"
#include "../../GameController/GameController.h"
#include "../../ECS/ECS.hpp"

//クラス
#include "../../Class/TouchInput.hpp"
#include "../../Class/Scene/SceneManager.hpp"
//アーキタイプ
#include "../../ArcheType/Button.hpp"
//コンポーネント



namespace Scene
{
	Title::Title()
	{
		ECS::ButtonArcheType()("pauseButton", Vec2(300, 300), Vec2(0, 0), Vec2(98, 98), 50)->AddGroup(ENTITY_GROUP::GameUI);
	}
	void Title::Update()
	{
		if (TouchInput::GetInput().Push(0))
		{
			//Scene::SceneManager::Get().ChangeScene(Scene::SceneManager::State::Game);
		}
		ECS::EcsSystem::GetManager().Update();
	}

	void Title::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}

	void Title::Release()
	{
		//すべてのEntityを殺す処理があると便利
		auto entity = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		for (auto& e : entity)
		{
			e->Destroy();
		}
	}
}