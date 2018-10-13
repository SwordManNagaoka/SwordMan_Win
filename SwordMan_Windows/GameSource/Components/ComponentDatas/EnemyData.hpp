//----------------------------------------------------
//!@file	EnemyData.hpp
//!@brief	敵のデータを作成します
//!@author	日比野　真聖
//!@date	2018/9/30
//----------------------------------------------------
#pragma once
#include "../../ECS/ECS.hpp"
#include "../../Utility/Vec.hpp"
#include "../../Components/ComponentDatas/CollisionData.hpp"


namespace ECS
{
	struct EnemyCommonData
	{
		Vec2 pos;
		Vec2 size;
		std::string imageName;
		int changeAnimFrameTime;
		int animNum;
		float moveSpeed;
		float jumpPower;
	};
}