//----------------------------------------------------
//!@file	BlendMode.hpp
//!@brief	画像のブレンド扱うコンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/10/11
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Utility/Counter.hpp"

namespace ECS
{
	class BlendMode : public Component
	{
	public:
		BlendMode()
		{
			kind = Kind::Non;
			cnt.Reset();
		}
		void Initialize() noexcept override
		{
			if (entity->HasComponent<SimpleDraw>())
			{
				draw = &entity->GetComponent<SimpleDraw>();
			}
			else if (entity->HasComponent<RectDraw>())
			{
				rectDraw = &entity->GetComponent<RectDraw>();
			}
		}
		void Update() noexcept override {}
		void Draw2D() noexcept override
		{
			//※本来はUpdate()で行うべきだが、なぜかUpdate()に入らないのでDraw()で呼ぶ
			switch (kind)
			{
			case Kind::Non:
				break;
			case Kind::In:
				cnt.Add();
				break;
			case Kind::Out:
				cnt.Sub();
				break;
			}

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt.GetCurrentCount());
			if (draw != nullptr)
			{
				draw->DrawEnable();
				draw->Draw2D();
				draw->DrawDisable();
			}
			else if (rectDraw != nullptr)
			{
				rectDraw->DrawEnable();
				rectDraw->Draw2D();
				rectDraw->DrawDisable();
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		//α値を設定する( 0～255 )
		void SetAlpha(const int alpha)
		{
			cnt.SetCounter(alpha, 1, 0, 255);
			kind = Kind::Non;
		}
		//α値を取得( 0～255 )
		const int GetAlpha() const noexcept
		{
			return cnt.GetCurrentCount();
		}
		const bool isEnd()  noexcept
		{
			if (kind == Kind::Out)
			{
				return cnt.IsMin();
			}
			else if (kind == Kind::In)
			{
				return cnt.IsMax();
			}
			return cnt.IsMax();
		}
		//フェードアウト (徐々に暗くしていきます)
		void FadeOut(const int startAlpha, const int endAlpha, const int fadeSpeed = 1)
		{
			cnt.SetCounter(startAlpha, fadeSpeed, endAlpha, startAlpha);
			kind = Kind::Out;
		}
		//フェードイン (徐々に明るくしていきます)
		void FadeIn(const int startAlpha, const int endAlpha, const int fadeSpeed = 1)
		{
			cnt.SetCounter(startAlpha, fadeSpeed, startAlpha, endAlpha);
			kind = Kind::In;
		}
		void setRect(const int srcx, const int srcy, const int width, const int height) noexcept
		{
			rectDraw->SetRect(srcx, srcy, width, height);
		}
	private:
		enum class Kind
		{
			In, Out,Non
		};
	private:
		Counter cnt;
		SimpleDraw* draw = nullptr;
		RectDraw* rectDraw = nullptr;
		Kind kind;
	};
}
