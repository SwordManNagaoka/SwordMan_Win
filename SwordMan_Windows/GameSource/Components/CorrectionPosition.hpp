//----------------------------------------------------
//!@file	CorrectionPosition.hpp
//!@brief	設定した位置からの補正コンポーネントを作成します
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
#include "../Components/SideHitBase.hpp"


namespace ECS
{
	//----------------------------------------------------------
	//初期位置からずれていた場合、補正する
	//
	//CorrectionPosition( 初期の位置 );
	//----------------------------------------------------------
	class CorrectionPosition final : public Component
	{
	public:
		explicit CorrectionPosition(const Vec2& pos)
		{
			initPos = pos;
		}
		~CorrectionPosition()
		{
			pos = nullptr;
		}
		void Initialize() override
		{

		}
		void Update() override
		{
			pos = &entity->GetComponent<Position>();
			if (entity->HasComponent<SideHitBase>())
			{
				bool mapHit = entity->GetComponent<SideHitBase>().CheckMapHit();
				if (mapHit) { return; }
				if (initPos.x != pos->val.x)
				{
					float distanceX = initPos.x - pos->val.x;
					pos->val.x += distanceX * per;
				}
			}
		}
		void Draw2D() override {}
		void SetInitPos(const Vec2& pos)
		{
			initPos = pos;
		}
	private:
		void Draw3D() override {}
	private:
		Position* pos = nullptr;
		Vec2 initPos;
		const float per = 0.01f;
	};
}