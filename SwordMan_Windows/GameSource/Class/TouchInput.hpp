#pragma once
#include "DxLib.h"
#include "../Utility/Vec.hpp"
#include <vector>
#include <unordered_map>


//--------------------------------------------------------------------
//TouchInput
//製作者: 日比野　真聖
//日付: 2018/9/14
//更新:tonarinohito フレームを返すメソッド追加
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
			btnCounter.resize(2);
			for (int i = 0;i < 2;++i)
			{
				btnCounter[i] = 0;
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
			if (GetTouchInputNum() > 0)
			{
				for (int i = 0; i < GetTouchInputNum(); ++i)
				{
					if (btnCounter.size() < GetTouchInputNum()) { break; }
					++btnCounter[i];
				}
			}
			else
			{
				for (int i = 0; i < btnCounter.size(); ++i)
				{
					btnCounter[i] = 0;
				}
			}
		}
		//ボタンを押しているフレーム数を返す
		int GetBtnPress(const int btnNumber)
		{
			return btnCounter[btnNumber];
		}
		//ボタンを押したかをリセットする
		void ResetBtnPush(const int btnNumber)
		{
			btnCounter[btnNumber] = 0;
		}
	private:
		std::unordered_map<int, BtnState>	buttonState;
		std::vector<int>					btnNumState;
		std::vector<int>					btnCounter;
	};
	//!@class 正常なタップを扱います
	class TapInput
	{
	private:
		struct TapData
		{
			TapData()
				: x_(0), y_(0), frame_(0)
			{}
			TapData(const int x, const int y, const int frame) noexcept
				: x_(x), y_(y), frame_(frame)
			{}
			TapData(const TapData&) = default;
		public:
			int x_;
			int y_;
			int frame_;
		};
	public:
		void Update() noexcept
		{
			//if (!tap.empty()) { tap.clear(); }
			std::unordered_map<int, TapData> tempTap;
			for (int i = 0; i < GetTouchInputNum(); ++i)
			{
				int posX, posY, id;
				//タッチされている箇所の座標取得
				GetTouchInput(i, &posX, &posY, &id, nullptr);
				//全フレームのタッチデータの中からタッチIDを探す
				if (tap.find(id) == tap.end())
				{
					//見つからない
					if (id <= maxTapNum)
					{
						std::pair<const int, TapData> temp(id, TapData(posX, posY, 1));
						tap.insert(temp);
						tempTap[id] = TapData(posX, posY, 1);
					}
				}
				else
				{
					//見つかる
					tempTap[id] = TapData(posX, posY, tap[id].frame_ + 1);
				}
			}
			//データを更新
			tap = std::move(tempTap);
		}
	public:
		//!@brief 指定したIDがタップされているか判定
		//!@return true:1フレーム以上タップ , false:IDなしorフレーム0
		const bool IsTap(const int id) const noexcept
		{
			if (tap.count(id) != 0)
			{
				return tap.at(id).frame_ >= 1;
			}
			return false;
		}
		//!@brief 指定したIDのフレーム時間を取得
		//!@return 0:フレームなし
		const int GetTapFrame(const int id) const noexcept
		{
			if (tap.count(id) != 0)
			{
				return tap.at(id).frame_;
			}
			return 0;
		}
		//!@brief 指定したIDのタップした座標を取得
		//!@return タップ位置
		const Vec2 GetTapPos(const int id) const noexcept
		{
			return Vec2(tap.at(id).x_, tap.at(id).y_);
		}
	private:
		std::unordered_map<int, TapData> tap;
		int maxTapNum = 2;
	};
public:
	//!@brief	入力システムにアクセスします
	static InputSystem&	GetInput() 
	{
		static std::unique_ptr<InputSystem>	input =
			std::make_unique<InputSystem>();
		return *input;
	}
	//!@brief タップの入力システムにアクセスします
	static TapInput& GetTap() noexcept
	{
		static std::unique_ptr<TapInput> input =
			std::make_unique<TapInput>();
		return *input;
	}
};
