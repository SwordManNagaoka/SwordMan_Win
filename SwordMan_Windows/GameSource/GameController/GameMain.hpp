#pragma once
#include "../System/System.hpp"
#include "../GameController/GameController.h"
#include "../Utility/FPS.hpp"
#include <memory>
class GameMain final
{
private:
	std::unique_ptr<System> system;
	std::unique_ptr<GameController> game;
	void Update()
	{
		game->Update();
	}
	void Draw()
	{
		game->Draw();
	}

public:
	GameMain()
	{
		system = std::make_unique<System>();
		game = std::make_unique<GameController>();

	}
	void Run()
	{
		while (system->IsOk())
		{
			Timer::Update();
			Update();
			Draw();
		}
	}
};