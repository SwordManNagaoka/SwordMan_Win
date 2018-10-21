//----------------------------------------------------
//!@file	Dash.hpp
//!@brief	横に移動(走らせる)コンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/10/11
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/AnimationController.hpp"



namespace ECS
{
	class PlayerDash : public Component
	{
	public:
		void Initialize() override
		{
			if (entity->HasComponent<Position>())
			{
				pos = &entity->GetComponent<Position>();
			}
			moveSpeed = 1.0f;
			addMoveSpeed = 0.0f;
			tolerance = 3.0f;
		}
		void Update() override
		{
			if (!moveFlag) { return; }
			if (pos == nullptr) { return; }
			if (IsTargetPos()) { return; }
			pos->val.x += moveSpeed;
			moveSpeed += addMoveSpeed;
		}
		void Draw2D() override
		{

		}
		void SetSpeed(const float speed)
		{
			moveSpeed = speed;
		}
		void SetAddSpeed(const float addSpeed)
		{
			addMoveSpeed = addSpeed;
		}
		void SetTargetPos(const Vec2& targetPos)
		{
			targetPoint = targetPos;
		}
		bool IsTargetPos() const
		{
			if (targetPoint.x - tolerance <= pos->val.x && pos->val.x <= targetPoint.x + tolerance)
			{
				return true;
			}
			return false;
		}
		void SetIsMove(bool isMove)
		{
			moveFlag = isMove;
		}
	private:
		void Draw3D() override {}
	private:
		Position* pos = nullptr;
		Vec2 targetPoint;
		float tolerance;
		float moveSpeed;
		float addMoveSpeed;
		bool moveFlag = true;
	};
}