#pragma once
#include "../ECS/ECS.hpp"
#define ENTITY_GROUP (ECS::Group)Game::GameGroup

class Game final
{
private:
	ECS::EntityManager* pManager;
	void EventUpDate();
	void ResourceLoad();
public:
	enum class GameGroup : ECS::Group
	{
		//OrderByDraw使用時、番号が大きいほど手前に描画される
		Back = 1u,
		Max,
	};
	Game();
	//Entityの初期化処理を明示的に呼び出します
	void ResetGame();
	//EntityおよびEventの更新処理を行います
	void Update();
	//Entityの描画を行います
	void Draw();
};
