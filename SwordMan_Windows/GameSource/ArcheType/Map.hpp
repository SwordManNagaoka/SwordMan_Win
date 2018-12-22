/**
* @file Map.hpp
* @brief Mapの原型を作成します。
* @author tonarinohito
* @date 2018/9/14
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/MapMove.hpp"
namespace ECS
{
	//マップチップ一つあたりの大きさは96x96
	class MapArcheType : public IArcheType<const char*, const Vec2&, const Vec2&, const int, const int, const int>
	{
	public:
		ECS::Entity* operator()(const char* name, const Vec2& pos, const Vec2& velocity, const int w, const int h, const int id) override
		{
			ECS::Entity*entity = &ECS::EcsSystem::GetManager().AddEntity();
			//このスコープでしか必要ない処理なためラムダ式で記述
			auto Tmp = [=](ECS::Entity* e)
			{
				e->AddComponent<KillEntity>(1);
				e->AddGroup(ENTITY_GROUP::Default);
			};
			auto Create = [=](ECS::Entity* e, const int srcX, const int srcY, bool isCollision = true)
			{
				
				e->AddComponent<Position>(pos);
				e->AddComponent<Velocity>(velocity);
				e->AddComponent<MapMove>();
				if (isCollision)
				{
					e->AddComponent<HitBase>(static_cast<float>(w), static_cast<float>(h)).SetColor(255, 0, 0);
				}
				e->AddComponent<RectDraw>(name, srcX, srcY, w, h);
				e->AddGroup(ENTITY_GROUP::Ground);
				
				
			};
			
			switch (id)
			{
			case -1:Tmp(entity);					break;
			case 0: Create(entity, 0, 0);			break;  //地面
			case 1: Create(entity, 96, 0, false);   break;  //断面
			case 2: Create(entity, 0, 96);			break;  //地面左端
			case 3: Create(entity, 96, 96);			break;  //断面左端
			case 4: Create(entity, 0, 192);			break;  //地面右端
			case 5: Create(entity, 96, 192);		break;	//断面右端
			}
			return entity;
		}
	};
}