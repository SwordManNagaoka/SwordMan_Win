//----------------------------------------------------
//!@file	SideHitBase.hpp
//!@brief	左右側の当たり判定のコンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/9/28
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Components/ComponentDatas/PlayerData.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"


namespace ECS
{
	//---------------------------------------------------------------------
	//左右側の当たり判定とマップの速度で移動させる
	//SideHitBase( 横の幅 , 縦の幅 );
	//-
	//SideHitBase( 横の幅 , 縦の幅 , マップチップの半分の高さの個数);
	//---------------------------------------------------------------------
	class SideHitBase final : public Component, public IBoxColiider
	{
	private:
		struct MapData
		{
			Vec2 pos;
			float height;
			float speed;
			float blockNum;
		};
	public:
		explicit SideHitBase(const float width, const float height)
		{
			size.x = width;
			size.y = height;
			mapData.blockNum = 1;
		}
		explicit SideHitBase(const float width, const float height, const int heightBlockNum)
		{
			size.x = width;
			size.y = height;
			mapData.blockNum = heightBlockNum;
		}
		~SideHitBase()
		{
			pos = nullptr;
		}
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
			initPos = pos->val;
		}
		void Update() override
		{
			//最初に横とマップが当たっているかチェックする
			//1: 当たったときに、すぐ左に移動させる
			//2: 当たらなくなったかどうかの判断は、プレイヤーの高さ情報をみる
			//マップの高さを調べてそれより上なら解除
			auto map = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground);
			for (auto& m : map)
			{
				if (m->GetComponent<Position>().val.x >= (System::SCREEN_WIDIH / DivNum)) { continue; }
				if (Collision::BoxAndBox<SideHitBase, HitBase>(*entity, *m))
				{
					//当たっとき左に移動
					leftMoveFlag = true;
					//マップデータの取得
					mapData.pos = m->GetComponent<Position>().val;
					mapData.speed = m->GetComponent<Velocity>().val.x;
					mapData.height = m->GetComponent<HitBase>().h();
					break;
				}
			}
			LeftMove();
		}
		void Draw2D() override
		{
			if (isDraw)
			{
				auto convert = pos->val.OffSetCopy(offset.x, offset.y);
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
			offset.x = x;
			offset.y = y;
		}
		bool CheckMapHit() const
		{
			return leftMoveFlag;
		}
		void FillEnable() override { isFill = true; }
		void FillDisable() override { isFill = false; }
		void DrawEnable() override { isDraw = true; }
		void DrawDisable() override { isDraw = false; }
		float w() const override { return size.x; }
		float h() const override { return size.y; }
		float x() const override { return pos->val.x + offset.x; }
		float y() const override { return pos->val.y + offset.y; }
	private:
		void Draw3D() override {}
		void LeftMove()
		{
			if (leftMoveFlag)
			{
				pos->val.x -= mapData.speed + Permission;
				float playerHeight = entity->GetComponent<HitBase>().h();
				if (pos->val.y < mapData.pos.y - (mapData.height / DivNum * mapData.blockNum) - (playerHeight / DivNum))
				{
					leftMoveFlag = false;
				}
			}
		}
	private:
		Position* pos = nullptr;
		Vec2 offset;
		Vec2 size;
		Vec2 initPos;
		MapData mapData;
		unsigned int color = 4294967295;
		bool isFill = false;
#if DEBUG
		bool isDraw = true;
#else
		bool isDraw = false;
#endif
		bool leftMoveFlag = false;
		const float Permission = 0.2f;
		const float DivNum = 2.0f;
	};
}