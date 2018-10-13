/**
* @file Collider.hpp
* @brief コリジョンに必要なコンポーネント群です。
* @author tonarinohito
* @date 2018/8/29
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/BasicComponents.hpp"
#include <DxLib.h>
namespace ECS
{
	class IBoxColiider
	{
	public:
		IBoxColiider() = default;
		virtual ~IBoxColiider() = default;
		/** @brief コリジョンの色を指定します*/
		virtual void SetColor(const int r, const int g, const int b) = 0;
		/** @brief コリジョンの位置を変更します*/
		virtual void SetOffset(const float x, const float y) = 0;
		/** @brief コリジョンを塗りつぶします*/
		virtual void FillEnable() = 0;
		/** @brief コリジョンの塗りつぶしを解除します*/
		virtual void FillDisable() = 0;
		/** @brief コリジョンの描画を有効にします*/
		virtual void DrawEnable() = 0;
		/** @brief コリジョンの描画を無効にします*/
		virtual void DrawDisable() = 0;
		/** @brief 幅を返します*/
		virtual float w() const = 0;
		/** @brief 高さを返します*/
		virtual float h() const = 0;
		/** @brief x座標を返します、この値はオフセットされた値です*/
		virtual float x() const = 0;
		/** @brief y座標を返します、この値はオフセットされた値です*/
		virtual float y() const = 0;
	};
	class ICircleColiider
	{
	public:
		ICircleColiider() = default;
		virtual ~ICircleColiider() = default;
		/** @brief コリジョンの色を指定します*/
		virtual void SetColor(const int r, const int g, const int b) = 0;
		/** @brief コリジョンの位置を変更します*/
		virtual void SetOffset(const float x, const float y) = 0;
		/** @brief コリジョンを塗りつぶします*/
		virtual void FillEnable() = 0;
		/** @brief コリジョンの塗りつぶしを解除します*/
		virtual void FillDisable() = 0;
		/** @brief コリジョンの描画を有効にします*/
		virtual void DrawEnable() = 0;
		/** @brief コリジョンの描画を無効にします*/
		virtual void DrawDisable() = 0;
		/** @brief 半径を返します*/
		virtual float radius() const = 0;
		/** @brief x座標を返します、この値はオフセットされた値です*/
		virtual float x() const = 0;
		/** @brief y座標を返します、この値はオフセットされた値です*/
		virtual float y() const = 0;
	};

	//矩形の定義
	class HitBase final : public Component, public IBoxColiider
	{
	private:
		Position* pos = nullptr;
		Vec2 offSetPos;
		float W, H;
		unsigned int color = 4294967295;
		bool isFill = false;
		bool isDraw = true;
	public:
		explicit HitBase(const float ww, const float hh)
		{
			W = ww;
			H = hh;
		}
		~HitBase()
		{
			pos = nullptr;
		}
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
		}
		void Update() override {}
		void Draw2D() override
		{
			if (isDraw)
			{
				auto convert = pos->val.OffSetCopy(offSetPos.x, offSetPos.y);
				DrawBoxAA(
					convert.x,
					convert.y,
					convert.x + w(),
					convert.y + h(),
					color, isFill, 2);
			}
		}
		void SetColor(const int r, const int g, const int b) override
		{
			color = GetColor(r, g, b);
		}
		void SetOffset(const float x, const float y) override
		{
			offSetPos.x = x;
			offSetPos.y = y;
		}
		void FillEnable() override { isFill = true; }
		void FillDisable() override { isFill = false; }
		void DrawEnable() override { isDraw = true; }
		void DrawDisable() override { isDraw = false; }
		float w() const override { return W; }
		float h() const override { return H; }
		float x() const override { return pos->val.x + offSetPos.x; }
		float y() const override { return pos->val.y + offSetPos.y; }
	};
	
	//足元矩形の定義
	class FootBase final : public Component, public IBoxColiider
	{
	private:
		Position * pos = nullptr;
		Vec2 offSetPos;
		float W, H;
		unsigned int color = 4294967295;
		bool isFill = false;
		bool isDraw = true;
	public:
		explicit FootBase(const float ww, const float hh)
		{
			W = ww;
			H = hh;
		}
		~FootBase()
		{
			pos = nullptr;
		}
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
		}
		void Update() override {}
		void Draw2D() override
		{
			if (isDraw)
			{
				auto convert = pos->val.OffSetCopy(offSetPos.x, offSetPos.y);
				DrawBoxAA(
					convert.x,
					convert.y,
					convert.x + w(),
					convert.y + h(),
					color, isFill, 2);
			}
		}
		void SetColor(const int r, const int g, const int b) override
		{
			color = GetColor(r, g, b);
		}
		void SetOffset(const float x, const float y) override
		{
			offSetPos.x = x;
			offSetPos.y = y;
		}
		void FillEnable() override { isFill = true; }
		void FillDisable() override { isFill = false; }
		void DrawEnable() override { isDraw = true; }
		void DrawDisable() override { isDraw = false; }
		float w() const override { return W; }
		float h() const override { return H; }
		float x() const override { return pos->val.x + offSetPos.x; }
		float y() const override { return pos->val.y + offSetPos.y; }
	};

	//円の定義
	class CircleColiider final : public Component, public ICircleColiider
	{
	private:
		Position * pos = nullptr;
		Vec2 offSetPos;
		float r;
		unsigned int color = 4294967295;
		bool isFill = false;
		bool isDraw = true;
	public:
		explicit CircleColiider(const float r_)
		{
			r = r_;
		}
		~CircleColiider()
		{
			pos = nullptr;
		}
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
		}
		void Update() override {}
		void Draw2D() override
		{
			if (isDraw)
			{
				auto convert = pos->val.OffSetCopy(offSetPos.x, offSetPos.y);
				DrawCircleAA(
					convert.x,
					convert.y,
					r,
					24,
					color, isFill, 2);
			}
		}
		void SetColor(const int r, const int g, const int b) override
		{
			color = GetColor(r, g, b);
		}
		void SetOffset(const float x, const float y) override
		{
			offSetPos.x = x;
			offSetPos.y = y;
		}
		void FillEnable() override { isFill = true; }
		void FillDisable() override { isFill = false; }
		void DrawEnable() override { isDraw = true; }
		void DrawDisable() override { isDraw = false; }
		float radius() const override { return r; }
		float x() const override { return pos->val.x + offSetPos.x; }
		float y() const override { return pos->val.y + offSetPos.y; }
	};
}