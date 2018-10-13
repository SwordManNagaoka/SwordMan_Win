#pragma once
#include "DxLib.h"
#include "../Utility/Vec.hpp"
#include <vector>
#include <unordered_map>


//--------------------------------------------------------------------
//TouchInput
//製作者: 日比野　真聖
//日付: 2018/9/14
//--------------------------------------------------------------------
class TouchInput
{
private:
	class InputSystem
	{
	public:
		InputSystem()
		{
			btnNumState.resize(2);
			for (int i = 0; i < 2; ++i)
			{
				buttonState[i] = BtnState::ReleaseBtn;
			}
		}
		InputSystem(const int touchNum)
		{
			btnNumState.resize(touchNum);
			for (int i = 0; i < touchNum; ++i)
			{
				buttonState[i] = BtnState::ReleaseBtn;
			}
		}
	private:
		//!@brief	ボタンの状態
		enum class BtnState
		{
			PushBtn,
			PressBtn,
			ReleaseBtn,
			FreeBtn,
		};
	public:
		//!@brief	タッチされている箇所の座標(x,y)を取得します
		const Vec2	GetTouchNum(const int touchNum) const
		{
			int x, y;
			GetTouchInput(touchNum, &x, &y, nullptr, nullptr);
			return { static_cast<float>(x),static_cast<float>(y) };
		}
		//!@brief	指定したタッチIDの座標(x,y)を取得します
		const Vec2	GetTouchIDPos(const int touchID) const
		{
			return GetTouchNum(touchID);
		}
		//!@brief	タッチされたボタンが押されたかチェック
		bool	Push(const int touchID)
		{
			return buttonState[touchID] == BtnState::PushBtn;
		}
		//!@brief	タッチされたボタンが押されているかチェック
		bool	Press(const int touchID)
		{
			return buttonState[touchID] == BtnState::PressBtn;
		}
		//!@brief	タッチされたボタンが離されているかチェック
		bool	Release(const int touchID)
		{
			return buttonState[touchID] == BtnState::ReleaseBtn;
		}
		//!@brief	タッチされたボタンが離されたかチェック
		bool	Free(const int touchID)
		{
			return buttonState[touchID] == BtnState::FreeBtn;
		}

		//!@brief	更新
		void	Update()
		{
			if (GetTouchInputNum() > 0)
			{
				//入力がありました
				for (size_t i = 0; i < buttonState.size(); ++i)
				{
					if (btnNumState[i] <= -1)
					{
						btnNumState[i] = 0;
					}
					++btnNumState[i];
					if (btnNumState[i] >= 2)
					{
						btnNumState[i] = 2;
					}
				}
			}
			else
			{
				for (size_t i = 0; i < buttonState.size(); ++i)
				{
					if (btnNumState[i] > 0)
					{
						btnNumState[i] = -1;
					}
					else if (btnNumState[i] <= -1)
					{
						btnNumState[i] = -2;
					}
				}
			}
			for (size_t i = 0; i < buttonState.size(); ++i)
			{
				if (btnNumState[i] == 1)
				{
					buttonState[i] = BtnState::PushBtn;
				}
				else if (btnNumState[i] == -1)
				{
					buttonState[i] = BtnState::FreeBtn;
				}
				else if (btnNumState[i] == 2)
				{
					buttonState[i] = BtnState::PressBtn;
				}
				else if (btnNumState[i] == -2)
				{
					buttonState[i] = BtnState::ReleaseBtn;
				}
			}
		}
	private:
		std::unordered_map<int, BtnState>	buttonState;
		std::vector<int>					btnNumState;
	};
public:
	//!@brief	入力システムにアクセスします
	static InputSystem&	GetInput()
	{
		static std::unique_ptr<InputSystem>	input =
			std::make_unique<InputSystem>();
		return *input;
	}
};
