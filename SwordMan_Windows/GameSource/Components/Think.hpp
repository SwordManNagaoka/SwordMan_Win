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
#include "../Components/Jump.hpp"
#include "../Utility/Input.hpp"

namespace ECS
{
	class Think : public Component
	{
	public:
		~Think()
		{
			jumpMove->SetJumpTrigger(false);
			jumpMove->SetFallTrigger(true);
			//data.state = PlayerData::State::Walk;
		}
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
				if (!jumpMove->IsLanding())
				{
					nowState = PlayerData::State::Airworthiness;
				}
				if (RightButtonTap(0))
				{
					nowState = PlayerData::State::Attack;
					if (LeftButtonTap(0) || LeftButtonTap(1))
					{
						nowState = PlayerData::State::Jump;
					}
				}
				else if (LeftButtonTap(0))
				{
					nowState = PlayerData::State::Jump;
					if (RightButtonTap(0) || RightButtonTap(1))
					{
						nowState = PlayerData::State::Attack;
					}
				}
				break;
			case PlayerData::State::Jump:
				nowState = PlayerData::State::Airworthiness;
				if (!jumpMove->IsLanding()) { break; }
				if (RightButtonTap(0) || RightButtonTap(1))
				{
					nowState = PlayerData::State::JumpAttack;
				}
				break;
			case PlayerData::State::Airworthiness:	
				if (jumpMove->IsLanding())
				{
					nowState = PlayerData::State::Walk;
				}
				if (RightButtonTap(0) || RightButtonTap(1))
				{
					nowState = PlayerData::State::JumpAttack;
				}
				break;
			case PlayerData::State::Attack:
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
				if (motionCnt.GetCurrentCount() >= 15)
				{
					motionEndFlag = true;
				}
				if (!jumpMove->IsLanding())
				{
					motionEndFlag = true;
				}
				else
				{
					if (LeftButtonTap(0) || LeftButtonTap(1))
					{
						motionEndFlag = true;
						nowState = PlayerData::State::Jump;
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
		bool	LeftButtonTap(const int btnNumber)
		{
			if (TouchInput::GetInput().GetBtnPress(btnNumber) == 1)
			{
				if (TouchInput::GetInput().GetTouchIDPos(btnNumber).x <= (System::SCREEN_WIDIH / 2.0f))
				{
					return true;
				}
			}
			if (Input::Get().GetKeyFrame(KEY_INPUT_LEFT)==1)
			{
				return true;
			}
			return false;
		}
		bool	RightButtonTap(const int btnNumber)
		{
			if (TouchInput::GetInput().GetBtnPress(btnNumber) == 1)
			{
				//Pauseボタンの矩形との当たり判定
				if (TouchInput::GetInput().GetTouchIDPos(btnNumber).x > (System::SCREEN_WIDIH / 2.0f))
				{
					if (TouchInput::GetInput().GetTouchIDPos(btnNumber).x >= (System::SCREEN_WIDIH - 98) && TouchInput::GetInput().GetTouchIDPos(btnNumber).x <= System::SCREEN_WIDIH)
					{
						if (TouchInput::GetInput().GetTouchIDPos(btnNumber).y <= 98 && TouchInput::GetInput().GetTouchIDPos(btnNumber).y >= 0)
						{
							return false;
						}
					}
					return true;
				}
			}
			if (Input::Get().GetKeyFrame(KEY_INPUT_RIGHT) == 1)
			{
				return true;
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