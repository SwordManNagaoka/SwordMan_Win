#include "Result.h"

//アーキタイプ
#include "../../ArcheType/Button.hpp"
//コンポーネント
#include "../../Components/ImageFontDraw.hpp"
#include "../../Components/BlendMode.hpp"

namespace Scene
{
	Result::Result(IOnSceneChangeCallback* sceneTitleChange, const Parameter& parame)
		: AbstractScene(sceneTitleChange)
	{
		ECS::Entity* btn = ECS::ButtonArcheType()("pauseUI", Vec2(600, 400), Vec2(0, 0), Vec2(96, 144), 50);
		btn->GetComponent<ECS::CircleColiider>().SetOffset(48, 48);
		btn->AddComponent<ECS::BackTitleButtonTag>();
		btn->AddGroup(ENTITY_GROUP::GameUI);
	}

	Result::~Result()
	{
		ECS::EcsSystem::GetManager().AllKill();
	}
	
	void Result::Update()
	{
		const auto& button = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		for (auto& b : button)
		{
			b->Update();
			if (b->HasComponent<ECS::BackTitleButtonTag>())
			{
				b->GetComponent<ECS::PushButton>().SetSceneCallBack(callBack);
				auto changeFunc = [](Scene::IOnSceneChangeCallback* callBack)
				{
					Parameter param;
					callBack->OnSceneChange(SceneName::Title, param, SceneStack::OneClear);
					return;
				};
				b->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
		}
	}
	void Result::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}