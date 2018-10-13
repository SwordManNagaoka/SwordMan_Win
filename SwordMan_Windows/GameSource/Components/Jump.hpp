#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../Components/BasicComponents.hpp"


namespace ECS
{
	class JumpMove : public Component
	{
	public:
		JumpMove()
			: MaxFallSpeed(15.0f)
		{}
		void	Initialize() override
		{
			jumpPow = -12.0f;
			flag = false;
			gravity = 9.8f / 60.0f / 60.0f;
			fallSpeed = 0.0f;
		}
		void	Update() override
		{
			//ジャンプフラグが立つと、ジャンプする
			if (flag)
			{
				fallSpeed = jumpPow;
			}
			fallSpeed += gravity;
			
			//位置に落下速度を加算
			if (entity->HasComponent<Position>())
			{
				Position& pos = entity->GetComponent<Position>();
				pos.val.y += fallSpeed;
			}

			//落下速度の制御
			if (fallSpeed >= MaxFallSpeed)
			{
				fallSpeed = MaxFallSpeed;
			}
		}
		void	Draw2D() override
		{

		}
		//!@brief	ジャンプフラグを設定
		void	SetJumpFlag(bool jumpFlag)
		{
			flag = jumpFlag;
		}
	private:
		void	Draw3D() override {}
	private:
		bool		flag;
		float		gravity;
		float		fallSpeed;
		float		jumpPow;
	private:
		const float	MaxFallSpeed;
	};

	/*!
	@class TriggerJumpMove
	@brief Entityがジャンプできるようになります
	@note Physicsが必要です
	*/
	class TriggerJumpMove : public Component
	{
	private:
		bool jumpTrigger = false;
		bool fallTrigger = false;
		Physics* physics;
		Velocity* velocity;
		float jumpPow;
		bool isFall;
		bool isJump;
		bool isLanding;
	private:
		void Draw2D() override {}
		void Draw3D() override {}
	public:
		explicit TriggerJumpMove(const float pow) : jumpPow(pow) {}
		void Initialize() override
		{
			physics = &entity->GetComponent<Physics>();
			velocity = &entity->GetComponent<Velocity>();
		}
		void Update() override
		{
			//ジャンプフラグが立つと、ジャンプする
			if (jumpTrigger && isLanding)
			{
				velocity->val.y = jumpPow;
			}
			//落下フラグが立つと落下
			else if (isJump && fallTrigger)
			{
				velocity->val.y *= 0.9f;
			}
			//落下速度が負の値ならジャンプ中
			if (velocity->val.y < 0)
			{
				isJump = true;
				isFall = false;
			}
			else
			{
				isJump = false;
				isFall = true;
			}
			//着地判定
			if (velocity->val.y == 0)
			{
				isLanding = true;
			}
			else
			{
				isLanding = false;
			}
		}
		//!@brief ジャンプフラグを設定。trueでジャンプ力分上昇します
		void SetJumpTrigger(bool jumpFlag)
		{
			jumpTrigger = jumpFlag;
		}
		//!@brief 落下フラグを設定。trueで落下します
		void SetFallTrigger(bool fallFlag)
		{
			fallTrigger = fallFlag;
		}
		//!@briefジャンプ中か
		const bool IsJumping() const
		{
			return isJump;
		}
		//!@brief落下中か
		const bool IsFalling() const
		{
			return isFall;
		}
		//!@brief着地中か
		const bool IsLanding() const
		{
			return isLanding;
		}
	};
}