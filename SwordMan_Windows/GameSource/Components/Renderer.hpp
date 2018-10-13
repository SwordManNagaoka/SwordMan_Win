/**
* @file Renderer.hpp
* @brief 描画関連のコンポーネントです。
* @author tonarinohito
* @date 2018/8/29
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Collision/Collision.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../System/System.hpp"
#include <DxLib.h>

namespace ECS
{
	//簡易画像描画
	class SimpleDraw final : public Component
	{
	private:
		Position * pos = nullptr;
		std::string name;
		bool isDraw = true;
	public:
		SimpleDraw(const char* name_)
		{
			if (!ResourceManager::GetGraph().IsExistenceHandle(name_))
			{
				assert(false);
			}
			name = name_;
		}
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
		}
		void Update() override {}
		void Draw2D() override
		{
			if (!isDraw) { return; }
			if (ResourceManager::GetGraph().IsExistenceHandle(name))
			{
				DrawGraphF(pos->val.x, pos->val.y, ResourceManager::GetGraph().GetHandle(name), true);
			}
		}
		//描画を行うか
		//true: 描画する false:描画しない
		void IsDraw(bool drawFlag)
		{
			isDraw = drawFlag;
		}
	};
	//指定した範囲を切り取って描画
	class RectDraw final : public Component
	{
	private:
		Position * pos = nullptr;
		Vec2 offset;
		RECT rect;
		std::string name;
	public:
		RectDraw(const char* name_, const int srcX, const int srcY, const int w, const int h)
		{
			if (!ResourceManager::GetGraph().IsExistenceHandle(name_))
			{
				assert(false);
			}
			rect.left = srcX;
			rect.right = srcY;
			rect.bottom = w;
			rect.top = h;
			name = name_;
		}
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
			offset = { 0, 0 };
		}
		void Update() override {}
		void Draw2D() override
		{
			if (ResourceManager::GetGraph().IsExistenceHandle(name))
			{
				DrawRectGraphF(
					pos->val.x + offset.x, pos->val.y + offset.y,
					rect.left, rect.right,
					rect.bottom,
					rect.top,
					ResourceManager::GetGraph().GetHandle(name),
					true);
			}
		}
		//描画矩形の設定・変更
		void SetRect(const int srcX, const int srcY, const int width, const int height)
		{
			rect.left = srcX;
			rect.right = srcY;
			rect.bottom = width;
			rect.top = height;
		}
		//描画位置をずらす
		void SetOffset(const int drawX, const int drawY)
		{
			offset.x = drawX;
			offset.y = drawY;
		}
	};
	//分割画像描画用
	class AnimationDraw final : public Component
	{
	private:
		Position * pos = nullptr;
		Direction* dir = nullptr;
		std::string name;
		int index = 0;
		bool isTurn = false;
		Vec2 offsetAmount;
	public:
		AnimationDraw(const char* name_)
		{
			if (!ResourceManager::GetGraph().IsExistenceDivHandle(name_))
			{
				DOUT << name << std::endl;
				assert(false);
			}
			name = name_;
		}
		void Initialize() override
		{
			if (entity->HasComponent<Direction>())
			{
				dir = &entity->GetComponent<Direction>();
			}
			pos = &entity->GetComponent<Position>();
			offsetAmount = Vec2(0.0f, 0.0f);
		}
		void Update() override
		{
			if (dir->val == Direction::Dir::R)
			{
				isTurn = false;
			}
			if (dir->val == Direction::Dir::L)
			{
				isTurn = true;
			}

		}
		void Draw2D() override
		{
			if (ResourceManager::GetGraph().IsExistenceDivHandle(name))
			{
				if (!isTurn)
				{
					DrawGraphF(pos->val.x + offsetAmount.x, pos->val.y + offsetAmount.y, ResourceManager::GetGraph().GetDivHandle(name, index), true);
				}
				else
				{
					DrawTurnGraphF(pos->val.x + offsetAmount.x, pos->val.y + offsetAmount.y, ResourceManager::GetGraph().GetDivHandle(name, index), true);
				}
			}
		}
		//分割画像の要素番号を指定する
		void SetIndex(const int index_)
		{
			index = index_;
		}
		//画像位置のオフセット
		void	Offset(const Vec2& offset)
		{
			offsetAmount = offset;
		}
	};
}