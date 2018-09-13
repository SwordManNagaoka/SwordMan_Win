/**
* @file BasicComponents.hpp
* @brief 座標や速度などの基本的なコンポーネント群です。
* @author tonarinohito
* @date 2018/8/29
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include <DxLib.h>
namespace ECS
{
	/*! 
	@struct Position
	@brief  座標です,データの型はVec2です
	*/
	struct Position final : public ComponentData
	{
		Vec2 val;
		Position() = default;
		Position(const Vec2& v) :val(v) {}
		Position(const float& x, const float& y) :val(x, y) {}

	};
	/*!
	@struct Rotation
	@brief  回転値です,データの型はfloatです
	*/
	struct Rotation final : public ComponentData
	{
		float val;
		Rotation() = default;
		Rotation(const float& r) : val(r) {}
		
	};
	/*!
	@struct Scale
	@brief  拡大率です,データの型はfloatです
	*/
	struct Scale final : public ComponentData
	{
		float val;
		Scale() = default;
		Scale(const float& scale) : val(scale) {}
		
	};
	/*!
	@struct Velocity
	@brief  速度です,データの型はVec2です
	*/
	struct Velocity final : public ComponentData
	{
		Vec2 val;
		Velocity() = default;
		Velocity(const Vec2& v) :val(v) {}
		Velocity(const float& x, const float& y) : val(x, y) {}
		
	};
	/*!
	@struct Direction
	@brief  向きです,データの型はenum class Dirです
	*/
	struct Direction final : public ComponentData
	{
		enum class Dir : short
		{
			R,
			L,
			U,
			D
		};
		Dir val;
		Direction() : val(Dir::R) {};
		
	};
	/*!
	@struct Gravity
	@brief 重力です,データの型はfloatです
	*/
	struct Gravity final : public ComponentData
	{
		float val;
		Gravity() = default;
		Gravity(const float g) :val(g) {}
		
	};
	/*!
	@struct AnimationState
	@brief  アニメーション用の状態です,データの型はenum class Stateです
	*/
	struct AnimationState final : public ComponentData
	{
		enum class State : short
		{
			Idol,
			Jump,
			Fall,
			Run,
			Attack,
		};
		State val;
		AnimationState() = default;
		AnimationState(const State& state) : val(state) {}
		
	};
	/*! 
	@class Physics
	@brief GravityとVelocityをまとめます
	*/
	class Physics final : public Component
	{
	private:
		Gravity* gravity;
		Velocity* velocity;
		Position* pos;
		static constexpr float GRAVITY = 9.8f / 60 / 60 * 32 * 3;
	public:
		void Initialize() override
		{
			if (!entity->HasComponent<Gravity>())
			{
				entity->AddComponent<Gravity>();
			}
			if (!entity->HasComponent<Velocity>())
			{
				entity->AddComponent<Velocity>();
			}
			velocity = &entity->GetComponent<Velocity>();
			gravity = &entity->GetComponent<Gravity>();
			pos = &entity->GetComponent<Position>();
			gravity->val = GRAVITY;
		}
		void Update() override {}
		void Draw2D() override {}
		void SetVelocity(const float& x, const float& y)
		{
			velocity->val.x = x;
			velocity->val.y = y;
		}
		void SetGravity(const float& g = GRAVITY)
		{
			gravity->val = g;
		}
	};

	/*!
	@class Transform
	@brief PositionとRotationとScaleをまとめます
	*/
	class Transform final : public Component
	{
	private:
		Position* pos;
		Rotation* rota;
		Scale* scale;

	public:
		Transform() :pos(nullptr), rota(nullptr) {}
		void Initialize() override
		{
			if (!entity->HasComponent<Position>())
			{
				entity->AddComponent<Position>(0.f, 0.f);
			}
			if (!entity->HasComponent<Rotation>())
			{
				entity->AddComponent<Rotation>(0.f);
			}
			if (!entity->HasComponent<Scale>())
			{
				entity->AddComponent<Scale>(1.f);
			}
			pos = &entity->GetComponent<Position>();
			rota = &entity->GetComponent<Rotation>();
			scale = &entity->GetComponent<Scale>();
		}
		void Update() override {}
		void Draw2D() override {}
		void SetPosition(const float& x, const float& y)
		{
			pos->val.x = x;
			pos->val.y = y;
		}
		void SetRotation(const float& r)
		{
			rota->val = r;
		}
		void SetScale(const float& scale_)
		{
			scale->val = scale_;
		}
	};

	/*!
	@class KillEntity
	@brief コンストラクタで指定したフレーム後にEntityを殺します
	*/
	class KillEntity final : public Component
	{
	private:
		int cnt;
	public:
		KillEntity(const int span) :cnt(span) {}

		void Initialize() override {}
		void Update() override
		{
			--cnt;
			if (cnt <= 0)
			{
				entity->Destroy();
			}
		}
		void Draw2D() override {}
		void Kill()
		{
			cnt = 0;
		}
		void SetKillLimit(const int limit)
		{
			cnt = limit;
		}
	};
}