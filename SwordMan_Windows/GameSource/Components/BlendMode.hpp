//----------------------------------------------------
//!@file	BlendMode.hpp
//!@brief	画像のブレンド扱うコンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/10/11
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../../Components/Collider.hpp"
#include "../../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Utility/Counter.hpp"

namespace ECS
{
	class BlendMode : public Component
	{
	public:
		BlendMode()
		{
			kind = Kind::In;
			cnt.Reset();
		}
		void Initialize() override
		{
			if (entity->HasComponent<SimpleDraw>())
			{
				draw = &entity->GetComponent<SimpleDraw>();
			}
		}
		void Update() override
		{
			if (isCnt)
			{
				if (kind == Kind::In)
				{
					cnt.Add();
				}
				else if (kind == Kind::Out)
				{
					cnt.Sub();
				}
			}
		}
		void Draw2D() override
		{
			if (draw == nullptr) { return; }
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt.GetCurrentCount());
			draw->IsDraw(true);
			draw->Draw2D();
			draw->IsDraw(false);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		//α値を設定する( 0～255 )
		void SetAlpha(const int alpha)
		{
			cnt.SetCounter(alpha, 1, 0, 255);
			isCnt = false;
		}
		//フェードアウト (徐々に暗くしていきます)
		void FadeOut(const int startAlpha, const int endAlpha, const int fadeSpeed = 1)
		{
			cnt.SetCounter(startAlpha, fadeSpeed, 0, endAlpha);
			isCnt = true;
			kind = Kind::Out;
		}
		//フェードイン (徐々に明るくしていきます)
		void FadeIn(const int startAlpha, const int endAlpha, const int fadeSpeed = 1)
		{
			cnt.SetCounter(startAlpha, fadeSpeed, 0, endAlpha);
			isCnt = true;
			kind = Kind::In;
		}
	private:
		void Draw3D() override {}
	private:
		enum class Kind
		{
			In, Out
		};
	private:
		Counter cnt;
		SimpleDraw* draw = nullptr;
		bool isCnt = false;
		Kind kind;
	};
}
