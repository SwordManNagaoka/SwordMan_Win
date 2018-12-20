/**
* @file BasicComponents.hpp
* @brief 座標や速度などの基本的なコンポーネント群です。
* @author tonarinohito
* @date 2018/8/29
* @par History
- 2018/08/20 tonarinohito
-# Physicsコンポーネントに機能を追加した
-# Physicsの重力定数をGravityに移動した
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include <DxLib.h>
#include <functional>
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
		float val = 0;
		Rotation() = default;
		Rotation(const float& r) : val(r) {}
		
	};
	/*!
	@struct Scale
	@brief  拡大率です,データの型はfloatです
	*/
	struct Scale final : public ComponentData
	{
		float val = 1;
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
		static constexpr float GRAVITY = 9.8f / 60 / 60 * 32 * 3;
		Gravity() :val(GRAVITY) {}
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
	/*
	@class Physics
	@brief Entityに重力を加えます。
	また衝突応答処理も含まれますが、これは明示的に呼び出してください
	@note Gravity, Velocity, Positionが必要です
	*/
	class Physics final : public Component
	{
	private:
		Gravity* gravity;
		Velocity* velocity;
		Position* pos;
		std::vector<Entity*> otherEntity;
		std::function<bool(const Entity&, const Entity&)> hitFunc;
		void CheckMove(Vec2& pos_, Vec2& velocity_)
		{
			Vec2 p = velocity_;
			////横軸に対する移動
			//while (p.x != 0.f)
			//{
			//	float  preX = pos_.x;

			//	if (p.x >= 1)
			//	{
			//		pos_.x += 1; p.x -= 1;
			//	}
			//	else if (p.x <= -1)
			//	{
			//		pos_.x -= 1; p.x += 1;
			//	}
			//	else
			//	{
			//		pos_.x += p.x;
			//		p.x = 0;
			//	}
			//	for (const auto& it : otherEntity)
			//	{
			//		if (hitFunc(*entity, *it))
			//		{
			//			velocity->val.x = 0;
			//			pos_.x = preX;		//移動をキャンセル
			//			break;
			//		}
			//	}

			//}
			//縦軸に対する移動
			while (p.y != 0.f)
			{
				float  preY = pos_.y;

				if (p.y >= 1)
				{
					pos_.y += 1; p.y -= 1;
				}
				else if (p.y <= -1)
				{
					pos_.y -= 1; p.y += 1;
				}
				else
				{
					pos_.y += p.y;
					p.y = 0;
				}
				for (const auto& it : otherEntity)
				{
					if (hitFunc(*entity, *it))
					{
						velocity->val.y = 0;
						pos_.y = preY;		//移動をキャンセル
						break;
					}
				}
			}
		}
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
		}
		void Update() override
		{
			velocity->val.y += gravity->val;
			CheckMove(pos->val, velocity->val);
		}
		void Draw2D() override {}
		void SetVelocity(const float& x, const float& y)
		{
			velocity->val.x = x;
			velocity->val.y = y;
		}
		void SetGravity(const float& g = Gravity::GRAVITY)
		{
			gravity->val = g;
		}
		//あたり判定の関数をセットする
		void SetCollisionFunction(std::function<bool(const Entity&, const Entity&)> func)
		{
			hitFunc = func;
		}
		//引数に指定したEntityにめり込まないようにする
		void PushOutEntity(std::vector<Entity*>&  e)
		{
			otherEntity = e;
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