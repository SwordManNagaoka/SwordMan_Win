#include "Game.h"
#include "../ResourceManager/ResourceManager.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Components/Renderer.hpp"
#include "../Components/Collider.hpp"
#include "../Class/MapLoader.hpp"
#include "../ArcheType/Map.hpp"
void Game::EventUpDate()
{

}

void Game::ResourceLoad()
{
	ResourceManager::GetGraph().Load("Resource/image/a.png","a");
	ResourceManager::GetGraph().Load("Resource/image/ground01.png", "map");
}

Game::Game()
{
	ResourceLoad();
	pManager = &ECS::EcsSystem::GetManager();
	auto& e = pManager->AddEntity();
	e.AddComponent<ECS::Position>(100.f,100.f);
	e.AddComponent<ECS::CircleColiider>(20.f);
	e.AddComponent<ECS::SimpleDraw>("a");
	MapLoader("Resource/stage/test_nagaoka.csv", "map", Vec2(5,5),13, 8, 96, ECS::MapArcheType());
}

void Game::ResetGame()
{

}

void Game::Update()
{
	pManager->Refresh();
	pManager->Update();

}

void Game::Draw()
{
	pManager->Draw2D();
}
