/**
* @file MapMove.hpp
* @brief マップの挙動です。
* @author tonarinohito
* @date 2018/9/15
*/
#pragma once
#include "../Components/BasicComponents.hpp"
#include "../System/System.hpp"
namespace ECS
{
	/*!
	@class MapMove
	@brief 移動と自身の削除のみ行う。生成は考慮しない
	@note  VelocityとPositionが必要です
	*/
	class MapMove : public Component
	{
	private:
		int mapWidth = 96;
		Position* pos;
		Velocity* vel;
	public:
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
			vel = &entity->GetComponent<Velocity>();
		}
		void Update() override
		{
			pos->val.x -= vel->val.x;
			//画面に外に来たら死ぬ
			if (pos->val.x < 0 - mapWidth)
			{
				entity->Destroy();
			}
		}

		void Draw2D() override {}

		void SetWidth(const int w)
		{
			mapWidth = w;
		}
	};
}
