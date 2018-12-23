#include "GameController.h"
#include "../Class/Sound.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Components/Renderer.hpp"
#include "../Components/Collider.hpp"
#include "../ArcheType/Map.hpp"
#include "../ArcheType/Player.hpp"
#include "../Class/TouchInput.hpp"
#include "../Class/Scene/SceneManager.hpp"
#include "../Events/EventManager.hpp"
#include "../Events/AtackEvent.hpp"
#include "../ArcheType/Enemy.hpp"
#include "../Events/AddScoreEvent.hpp"
#include "../Events/GoalEvent.hpp"
#include "../Events/PauseTap.hpp"

#include "../Class/Scene/Title.h"
#include "../Class/Scene/Game.h"
#include "../Class/Scene/Pause.h"
#include "../Class/Scene/Result.h"
#include "../Class/Scene/Menu.h"
#include "../Utility/Input.hpp"
int CommonData::StageNum::val = 0;
void GameController::ResourceLoad()
{
	ResourceManager::GetSound().Load("Resource/sounds/rolling.wav", "rolling", SoundType::SE);
	ResourceManager::GetSound().Load("Resource/sounds/smash.wav", "smash", SoundType::SE);
	ResourceManager::GetSound().Load("Resource/sounds/hit.wav", "hit", SoundType::SE);
	ResourceManager::GetSound().Load("Resource/sounds/bomb.wav", "bomb", SoundType::SE);
	ResourceManager::GetSound().Load("Resource/sounds/jump.wav", "jump", SoundType::SE);
	ResourceManager::GetGraph().Load("Resource/image/cloud.png", "cloud");
	ResourceManager::GetGraph().Load("Resource/image/a.png", "a");
	ResourceManager::GetGraph().Load("Resource/image/font_text.png", "font");
	ResourceManager::GetGraph().Load("Resource/image/ui/goalMessage.png", "goalMessage");
	ResourceManager::GetGraph().Load("Resource/image/ui/pauseButton.png", "pauseButton");
	ResourceManager::GetGraph().Load("Resource/image/ui/life.png", "health");
	ResourceManager::GetGraph().Load("Resource/image/ui/pauseUI.png", "pauseUI");
	ResourceManager::GetGraph().Load("Resource/image/ui/fade.png", "fade");
	ResourceManager::GetGraph().Load("Resource/image/ui/pauseMessage.png", "pauseMessage");
	ResourceManager::GetGraph().LoadDiv("Resource/image/sword.png", "sword", 5, 5, 1, 193, 193);
	ResourceManager::GetGraph().LoadDiv("Resource/image/rolling.png", "rolling", 12, 4, 3, 288, 288);	
	ResourceManager::GetGraph().LoadDiv("Resource/image/enemy01.png", "enemy1", 2, 2, 1, 96, 96);
	ResourceManager::GetGraph().LoadDiv("Resource/image/enemy02.png", "enemy2", 4, 4, 1, 96, 96);
	ResourceManager::GetGraph().LoadDiv("Resource/image/enemy03.png", "enemy3", 6, 6, 1, 96, 96);
	ResourceManager::GetGraph().LoadDiv("Resource/image/enemy04.png", "enemy4", 4, 4, 1, 96, 96);
	ResourceManager::GetGraph().LoadDiv("Resource/image/goal.png", "goal", 1, 1, 1, 144, 192);
	ResourceManager::GetGraph().LoadDiv("Resource/image/player.png", "player", 6, 2, 3, 96, 96);
	ResourceManager::GetGraph().LoadDiv("Resource/image/effect/hit_weak.png", "hitWeak", 4, 4, 1, 192, 192);
	ResourceManager::GetGraph().LoadDiv("Resource/image/effect/bomb.png", "bomb", 4, 4, 1, 192, 192);
	ResourceManager::GetGraph().LoadDiv("Resource/image/effect/hit_strong.png", "hitStrong", 5, 5, 1, 192, 192);
}

GameController::GameController()
{
	//最初に必要なリソースやEntityの生成、ロードを行う
	ResourceLoad();
	pManager = &ECS::EcsSystem::GetManager();	
	//初期シーン
	sceneStack.push(std::make_unique< Scene::Title >(this, nullptr));	//タイトルシーンを作成し、プッシュ
	MasterSound::Get().SetAllBGMGain(0.8f);
}

void GameController::ResetGame()
{

}

void GameController::Update()
{
	TouchInput::GetInput().Update();
	Input::Get().Update_Key();
	pManager->Refresh();
	sceneStack.top()->Update();
	MasterSound::Get().Update();
}

void GameController::Draw()
{
	sceneStack.top()->Draw();
}


void GameController::OnSceneChange(const Scene::SceneName& scene, Parameter* parame, const Scene::SceneStack& stackClear)
{
	switch (stackClear)
	{
	case Scene::SceneStack::Non:
		break;
	case Scene::SceneStack::OneClear:
		sceneStack.top()->Finalize();
		sceneStack.pop();
		break;
	case Scene::SceneStack::AllClear:
		StackAllClear();
		break;
	}
	switch (scene)
	{
	case Scene::SceneName::Title:
		sceneStack.push(std::make_unique<Scene::Title>(this, parame));
		break;
	case Scene::SceneName::Menu:
		sceneStack.push(std::make_unique<Scene::Menu>(this, parame));
		break;
	case Scene::SceneName::Game:
		sceneStack.push(std::make_unique<Scene::Game>(this, parame));
		break;
	case Scene::SceneName::Pause:
		sceneStack.push(std::make_unique<Scene::Pause>(this, parame));
		break;
	case Scene::SceneName::Result:
		sceneStack.push(std::make_unique<Scene::Result>(this, parame));
		break;
	default:
		break;
	}
}

void GameController::StackAllClear()
{
	while (!sceneStack.empty())
	{
		sceneStack.top()->Finalize();
		sceneStack.pop();
	}
}
