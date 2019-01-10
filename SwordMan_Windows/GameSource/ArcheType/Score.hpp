//----------------------------------------------------
//!@file	Score.hpp
//!@brief	スコアの原型を作成します
//!@author	日比野　真聖
//!@date	2018/10/6
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../Components/TotalScoreDraw.hpp"
#include "../Utility/Converter.hpp"
#include "../Components/ImageFontDraw.hpp"

namespace ECS
{
	class AddScoreTag : public Component
	{
	public:
		void Initialize() override {}
		void Update() override {}
		void Draw2D() override {}
	private:
		void Draw3D() override {}
	};

	class AddScoreArcheType : public IArcheType<const std::string&, const Vec2&,const int>
	{
	public:
		ECS::Entity* operator()(const std::string& imageName, const Vec2& pos, const int score)
		{
			ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<Position>(pos);
			std::string str = "+";
#if _Android_
			str.append(Converter::ToString(score));
#else
			str.append(std::to_string(score));
#endif
			entity->AddComponent<ImageFontDraw>(imageName, Vec2(32, 32), 16).SetDrawData(str.c_str());
			entity->AddComponent<KillEntity>(30);
			entity->AddComponent<AddScoreTag>();
			entity->AddGroup(ENTITY_GROUP::GameUI);
			return entity;
		}
	};

	class TotalScoreArcheType : public IArcheType<const std::string&, const Vec2&>
	{
	public:
		ECS::Entity* operator()(const std::string& imageName, const Vec2& pos)
		{
			ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<Position>(pos);
			entity->AddComponent<ImageFontDraw>(imageName, Vec2(32, 32), 16);
			entity->AddComponent<TotalScoreDraw>();
			entity->AddGroup(ENTITY_GROUP::GameUI);
			return entity;
		}
	};
}