#pragma once
#include "../ECS/ECS.hpp"
#define ENTITY_GROUP (ECS::Group)GameController::GameGroup
class GameController final
{
private:
	ECS::EntityManager* pManager;
	void ResourceLoad();
public:
	enum class GameGroup : ECS::Group
	{
		//OrderByDraw使用時、番号が大きいほど手前に描画される
		Back1,		//背景(空など)
		Back2,		//背景(遠景など)
		Back3,		//背景(雲など)
		Ground,		//地形・地面
		Gimmick,	//ギミック
		Wepon,		//武器(剣など)
		Player,		//プレイヤー
		Enemy,		//敵
		Effect,		//エフェクト
		GameUI,		//ゲーム本編用UI
		Fade1,		//フェード画像その1
		PauseUI,	//ポーズ画面用UI
		Fade2,		//フェード画像その2
		Max,		//Group最大数
	};
	GameController();
	//Entityの初期化処理を明示的に呼び出します
	void ResetGame();
	//EntityおよびEventの更新処理を行います
	void Update();
	//Entityの描画を行います
	void Draw();
};
