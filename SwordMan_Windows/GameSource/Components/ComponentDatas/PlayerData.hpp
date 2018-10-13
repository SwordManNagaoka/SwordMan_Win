#pragma once
#include "../../ECS/ECS.hpp"
#include "../../Utility/Vec.hpp"
#include "../../ResourceManager/ResourceManager.hpp"
#include "../../Utility/Counter.hpp"


struct PlayerData
{
	enum class State
	{
		Walk,			//歩く
		Jump,			//ジャンプ
		Airworthiness,	//耐空
		Attack,			//地面上の攻撃
		JumpAttack,		//耐空の攻撃
	};
	State	state;
};


struct Health : public ECS::ComponentData
{
	Counter value;
};