#include "Game.h"
#include "../ResourceManager/ResourceManager.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Components/Renderer.hpp"
#include "../Components/Collider.hpp"
void Game::EventUpDate()
{

}

void Game::ResourceLoad()
{
	ResourceManager::GetGraph().Load("Resource/image/a.png","a");
}

Game::Game()
{
	ResourceLoad();
	pManager = &ECS::EcsSystem::GetManager();
	auto& e = pManager->AddEntity();
	e.AddComponent<ECS::Position>(100.f,100.f);
	e.AddComponent<ECS::CircleColiider>(20.f);
	e.AddComponent<ECS::SimpleDraw>("a");
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
