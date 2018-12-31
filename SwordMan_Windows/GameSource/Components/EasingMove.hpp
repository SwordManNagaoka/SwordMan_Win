//----------------------------------------------------
//!@file	EasingMove.hpp
//!@brief	イージングを用いた移動コンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/10/11
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Utility/Easing.hpp"


namespace ECS
{
	//------------------------------------------------------------
	//コンストラクタ
	//EasingMove( 使用するEasing関数ポインタ, 適用させる方向 )
	//*************************
	//その他の設定
	//1: SetEasing( easing関数 );
	//2: SetBeginToEndPoint( 始点 , 終点 );
	//3: SetTimeToDuration( 時間 , 区間 );
	//------------------------------------------------------------
	class EasingMove : public Component
	{
	public:
		enum class DirectionState
		{
			LeftAndRight,
			UpAndDown,
			All,
		};
	public:
		EasingMove(const Ease easing, const DirectionState& directionState)
		{
			ease = easing;
			moveDirection = directionState;
			begin = 0.0f;
			end = 0.0f;
			time = 0.0f;
			duration = 0.0f;
		}
		void Initialize() override
		{
			if (entity->HasComponent<ECS::Position>())
			{
				pos = &entity->GetComponent<Position>();
			}
		}
		void Update() override
		{
			if (pos == nullptr) { return; }
			switch (moveDirection)
			{
			case DirectionState::LeftAndRight:
				pos->val.x = easing.GetVolume(begin, end - begin);
				break;
			case DirectionState::UpAndDown:
				pos->val.y = easing.GetVolume(begin, end - begin);
				break;
			case DirectionState::All:
				pos->val.x = easing.GetVolume(begin, end - begin);
				pos->val.y = easing.GetVolume(begin, end - begin);
				break;
			}
			ease(time, duration);
			easing.Run(ease, duration);
		}
		bool IsEnd() { return easing.IsEaseEnd(); }
		void Draw2D() override
		{

		}
		//[ 始点ポイント ]と[ 終点ポイント ]を設定
		void SetBeginToEndPoint(const float beginPoint, const float endPoint)
		{
			SetBeginPoint(beginPoint);
			SetEndPoint(endPoint);
		}
		//[ 移動時間 ]と[ 移動区間 ]を設定
		void SetTimeToDuration(const float moveTime, const float moveDuration)
		{
			SetMoveTime(moveTime);
			SetMoveDuration(moveDuration);
		}
		//[ easing関数 ]を設定
		void SetEasing(const Ease easing)
		{
			ease = easing;
		}
	private:
		//動作の開始時間を設定
		void SetBeginPoint(const float beginPoint)
		{
			begin = beginPoint;
		}
		//動作の終了地点を設定
		void SetEndPoint(const float endPoint)
		{
			end = endPoint;
		}
		//動作させるフレーム時間を設定
		void SetMoveTime(const float moveTime)
		{
			time = moveTime;
		}
		//動作させる区間を設定
		void SetMoveDuration(const float moveDuration)
		{
			duration = moveDuration;
		}
	private:
		void Draw3D() override {}
	private:
		Easing easing;
		Position* pos = nullptr;
		float begin;
		float end;
		float time;
		float duration;
		Ease ease;
		DirectionState moveDirection;
	};
}