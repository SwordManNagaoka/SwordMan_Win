#include "Result.h"

//アーキタイプ
#include "../../ArcheType/Button.hpp"
//コンポーネント
#include "../../Components/ImageFontDraw.hpp"
#include "../../Components/BlendMode.hpp"

namespace Scene
{
	Result::Result(IOnSceneChangeCallback* sceneTitleChange, Parameter* parame)
		: AbstractScene(sceneTitleChange)
	{
		ECS::Entity* btn = ECS::ButtonArcheType()("pauseUI", Vec2(600, 400), Vec2(0, 0), Vec2(96, 144), 50);
		btn->GetComponent<ECS::CircleColiider>().SetOffset(48, 48);
		btn->AddComponent<ECS::BackTitleButtonTag>();
		btn->AddGroup(ENTITY_GROUP::GameUI);

		printfDx("%d\n", parame->Get<int>("score"));
	}

	Result::~Result()
	{
		ResourceManager::GetSound().Remove("BGM");
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
				b->GetComponent<ECS::PushButton>().SetSceneCallBack(&GetCallback());
				auto changeFunc = [](Scene::IOnSceneChangeCallback* callBack)
				{
					callBack->OnSceneChange(SceneName::Title, nullptr, SceneStack::AllClear);
					return;
				};
				b->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
		}
		if (Input::Get().GetKeyFrame(KEY_INPUT_X) == 1)
		{
			GetCallback().OnSceneChange(SceneName::Title, nullptr, SceneStack::AllClear);
			return;
		}
	}
	void Result::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}