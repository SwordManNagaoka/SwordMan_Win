//----------------------------------------------------
//!@file	CollisionData.hpp
//!@brief	コリジョンのデータを作成します
//!@author	日比野　真聖
//!@date	2018/9/30
//----------------------------------------------------
#pragma once
#include "../../ECS/ECS.hpp"
#include "../../Utility/Vec.hpp"


namespace ECS
{
	struct CollisionCommonData
	{
		bool isCollision;
	};

	struct BoxCollisionData
	{
		CollisionCommonData commonData;
		Vec2 boxSize;
	};

	struct CircleCollisionData
	{
		CollisionCommonData commonData;
		float radius;
	};
}