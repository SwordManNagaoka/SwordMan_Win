#include "GameController.h"
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
#include "../Utility/Input.hpp"

void GameController::ResourceLoad()
{

	ResourceManager::GetGraph().Load("Resource/image/a.png", "a");
	ResourceManager::GetGraph().Load("Resource/image/font_text.png", "font");
	ResourceManager::GetGraph().Load("Resource/image/ui/goalMessage.png", "goalMessage");
	ResourceManager::GetGraph().Load("Resource/image/ui/pauseButton.png", "pauseButton");
	ResourceManager::GetGraph().Load("Resource/image/ui/life.png", "health");
	ResourceManager::GetGraph().Load("Resource/image/ui/pauseUI.png", "pauseUI");
	ResourceManager::GetGraph().Load("Resource/image/ui/fade.png", "fade");
	ResourceManager::GetGraph().LoadDiv("Resource/image/sword.png", "sword", 5, 5, 1, 192, 192);
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
	Scene::SceneManager::Get().ChangeScene(Scene::SceneManager::State::Game);

	//イベント関数の登録
	//Event::EventManager().Get().Add(Scene::SceneManager::State::Game, Event::AddScoreEvent::Do);
	Event::EventManager().Get().Add(Scene::SceneManager::State::Game, Event::CollisionEvent::AttackCollisionToEnemy);
	Event::EventManager().Get().Add(Scene::SceneManager::State::Game, Event::CollisionEvent::PlayerToEnemy);
	//Event::EventManager().Get().Add(Scene::SceneManager::State::Game, Event::GoalEvent::HitPlayer);
	Event::EventManager().Get().Add(Scene::SceneManager::State::Game, Event::PouseButtonEvent::PouseButtonTap);
}

void GameController::ResetGame()
{

}

void GameController::Update()
{
	Input::Get().Update_Key();
	pManager->Refresh();
	Event::EventManager::Get().Update();
	Scene::SceneManager::Get().Update();
}

void GameController::Draw()
{
	Scene::SceneManager::Get().Draw();
}
