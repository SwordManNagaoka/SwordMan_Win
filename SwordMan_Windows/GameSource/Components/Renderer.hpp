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
			if (ResourceManager::GetGraph().IsExistenceHandle(name))
			{
				DrawGraphF(pos->val.x, pos->val.y, ResourceManager::GetGraph().GetHandle(name), true);
			}
		}
	};
	//指定した範囲を切り取って描画
	class RectDraw final : public Component
	{
	private:
		Position * pos = nullptr;
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
		}
		void Update() override {}
		void Draw2D() override
		{
			if (ResourceManager::GetGraph().IsExistenceHandle(name))
			{
				//スクリーン外は描画しない
				if (Collision::BoxAndBox(
					Vec2(0, 0),
					Vec2(System::SCREEN_WIDIH, System::SCREEN_HEIGHT),
					pos->val,
					Vec2(float(rect.bottom), float(rect.top))))
				{
					DrawRectGraphF(
						pos->val.x, pos->val.y,
						rect.left, rect.right, 
						rect.bottom, 
						rect.top, 
						ResourceManager::GetGraph().GetHandle(name), 
						true);
				}
			}
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
	public:
		AnimationDraw(const char* name_)
		{
			if (!ResourceManager::GetGraph().IsExistenceDivHandle(name_))
			{
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
					DrawGraphF(pos->val.x, pos->val.y, ResourceManager::GetGraph().GetDivHandle(name, index), true);
				}
				else
				{
					DrawTurnGraphF(pos->val.x, pos->val.y, ResourceManager::GetGraph().GetDivHandle(name, index), true);
				}
			}
		}
		//分割画像の要素番号を指定する
		void SetIndex(const int index_)
		{
			index = index_;
		}
	};
}