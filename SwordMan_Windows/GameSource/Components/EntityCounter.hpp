//----------------------------------------------------
//!@file	EntityCounter.hpp
//!@brief	EntityにAddしてからの時間を計測するコンポーネントを作成します
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
	//-----------------------------------------------------
	//EntityにAddComponentしてからの時間を計測する
	//コンストラクタ
	//EntityCounter()
	//***
	//EntityCounter( 指定するカウント );
	//-----------------------------------------------------
	class EntityCounter : public Component
	{
	public:
		EntityCounter()
		{
			specifyCnt = 0;
		}
		EntityCounter(const int specifyCount)
		{
			specifyCnt = specifyCount;
		}
		void Initialize() override
		{
			count.Reset();
			isMaxTime = false;
		}
		void Update() override
		{
			count.Add();
			isMaxTime = false;
			if (count.GetCurrentCount() == specifyCnt)
			{
				isMaxTime = true;
				count.Reset();
			}
		}
		void Draw2D() override {}
		//指定したカウントになったか
		//true:指定したカウント false:指定したカウントでない
		bool IsSpecifyCnt()
		{
			return isMaxTime;
		}
		//時間を指定する
		void SetSpecifyCnt(const int specifyCount)
		{
			specifyCnt = specifyCount;
		}
		//現在の時間を取得する
		const int GetCurrentCount() const
		{
			return count.GetCurrentCount();
		}
	private:
		void Draw3D() override {}
	private:
		Counter count;
		int specifyCnt;
		bool isMaxTime;
	};

}