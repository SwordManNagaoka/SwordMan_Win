#pragma once
#include "../System/System.hpp"
#include "../GameController/Game.h"
#include "../Utility/FPS.hpp"
#include <memory>
class GameMain final
{
private:
	std::unique_ptr<System> system;
	std::unique_ptr<Game> game;
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
		game = std::make_unique<Game>();

	}
	void Run()
	{
		while (system->IsOk())
		{
			FPS::Get().Update();
			Update();
			Draw();
			FPS::Get().Wait();
		}
	}
};