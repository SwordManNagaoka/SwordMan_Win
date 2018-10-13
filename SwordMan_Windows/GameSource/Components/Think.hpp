//----------------------------------------------------
//!@file	Think.hpp
//!@brief	思考(状態に応じた遷移)部分を作成します
//!@author	日比野　真聖
//!@date	2018/9/14
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Components/ComponentDatas/PlayerData.hpp"
#include "../Class/TouchInput.hpp"
#include "../Components/AnimationController.hpp"
#include "../Utility/Input.hpp"
namespace ECS
{
	class Think : public Component
	{
	public:
		void	Initialize() override
		{
			data.state = PlayerData::State::Walk;
			motionEndFlag = false;
			if (entity->HasComponent<TriggerJumpMove>())
			{
				jumpMove = &entity->GetComponent<TriggerJumpMove>();
			}
		}
		void	Update() override
		{
			PlayerData::State	nowState = data.state;
			//個別の状態遷移
			switch (nowState)
			{
			case PlayerData::State::Walk:
				if (LeftButtonTap())
				{
					nowState = PlayerData::State::Jump;
				}
				if (RightButtonTap())
				{
					nowState = PlayerData::State::Attack;
				}
				if (!jumpMove->IsLanding())
				{
					nowState = PlayerData::State::Airworthiness;
				}
				break;
			case PlayerData::State::Jump:
				if (jumpMove->IsLanding())
				{
					nowState = PlayerData::State::Walk;	
				}
				else
				{
					nowState = PlayerData::State::Airworthiness;
				}
				break;
			case PlayerData::State::Airworthiness:	
				if (RightButtonTap())
				{
					nowState = PlayerData::State::JumpAttack;
				}
				if (jumpMove->IsLanding())
				{
					nowState = PlayerData::State::Walk;
				}
				break;
			case PlayerData::State::Attack:
				if (motionCnt.GetCurrentCount() >= 20)
				{
					if (jumpMove->IsLanding())
					{
						nowState = PlayerData::State::Walk;
					}
					else
					{
						nowState = PlayerData::State::Airworthiness;
					}
				}
				break;
			case PlayerData::State::JumpAttack:
				if (motionEndFlag)
				{
					if (jumpMove->IsLanding())
					{
						nowState = PlayerData::State::Walk;
					}
					else
					{
						nowState = PlayerData::State::Airworthiness;
					}
				}
				if (motionCnt.GetCurrentCount() >= 8)
				{
					motionEndFlag = true;
				}
				if (jumpMove->IsLanding())
				{
					motionEndFlag = true;
				}
				break;
			}
			motionCnt.Add();
			UpdateState(nowState);
		}
		void	Draw2D() override {}
	public:
		//現在の状態を取得
		const PlayerData::State&	GetNowState() const
		{
			return data.state;
		}
		//現在のモーションカウントを取得
		const int	GetNowMotionCnt() const
		{
			return motionCnt.GetCurrentCount();
		}
		//モーションを変化させます
		void	ChangeMotion(const PlayerData::State& motionState)
		{
			UpdateState(motionState);
		}
		//モーションの終了かのどうかのチェック
		//true:終了 false:終了していない
		bool	CheckMotionCancel()
		{
			return motionEndFlag;
		}
	private:
		void	Draw3D() override {}
		bool	UpdateState(const PlayerData::State& nowState)
		{
			if (data.state == nowState) { return false; }
			//更新
			motionCnt.Reset();
			data.state = nowState;
			motionEndFlag = false;
			return true;
		}
		bool	LeftButtonTap()
		{
			if (Input::Get().GetKeyFrame(KEY_INPUT_LEFT) == 1)
			{
				//if (TouchInput::GetInput().GetTouchIDPos(0).x <= (System::SCREEN_WIDIH / 2.0f))
				{
					return true;
				}
			}
			return false;
		}
		bool	RightButtonTap()
		{
			if (Input::Get().GetKeyFrame(KEY_INPUT_RIGHT) == 1)
			{
				//if (TouchInput::GetInput().GetTouchIDPos(0).x > (System::SCREEN_WIDIH / 2.0f))
				{
					return true;
				}
			}
			return false;
		}
	private:
		PlayerData	data;
		Counter		motionCnt;
		TriggerJumpMove* jumpMove;
		bool motionEndFlag;
	};
}