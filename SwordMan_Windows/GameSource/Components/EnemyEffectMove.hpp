//----------------------------------------------------
//!@file	EnemyEffectMove.hpp
//!@brief	敵のエフェクト時の動作のコンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/10/8
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Components/Renderer.hpp"


namespace ECS
{
	//敵のエフェクトの動作
	class EnemyEffectMove : public Component
	{
	public:
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
			if (entity->HasComponent<HitBase>())
			{
				size = Vec2(entity->GetComponent<HitBase>().w() / 2.0f, entity->GetComponent<HitBase>().h() / 2.0f);
			}
			else
			{
				size = Vec2(96, 96);
			}
		}
		void Update() override
		{
			pos->val += direction * moveSpeed;
			if (pos->val.x <= 0 || System::SCREEN_WIDIH <= pos->val.x + size.x)
			{
				direction.x = -direction.x;
			}
			if (pos->val.y < 0 || System::SCREEN_HEIGHT <= pos->val.y + size.y)
			{
				direction.y = -direction.y;
			}
		}
		void Draw2D() override {}
		//方向と動作スピードを設定する
		void SetDirMove(const Vec2& dir, const float speed)
		{
			direction = dir;
			moveSpeed = speed;
		}
	private:
		void Draw3D() override {}
	private:
		Position* pos = nullptr;
		Vec2 size;
		Vec2 direction;
		float moveSpeed;
	};
}