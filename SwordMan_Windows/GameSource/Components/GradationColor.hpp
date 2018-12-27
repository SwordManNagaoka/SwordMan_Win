//----------------------------------------------------
//!@file	GradationColor.hpp
//!@brief	色をグラデーション変化させるコンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/12/23
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Utility/Counter.hpp"
#include "../Components/Renderer.hpp"


namespace ECS
{
	class GradationColor final : public ECS::Component
	{
	public:
		void Initialize() override
		{
			color = &entity->GetComponent<Color>();
			gradationDelta = Vec3(1, 1, 1);
		}
		void Update() override
		{
			color->red += gradationDelta.x;
			color->green += gradationDelta.y;
			color->blue += gradationDelta.z;
			if (color->red <= 2 || color->red >= 254) { gradationDelta.x *= -1; }
			if (color->green <= 2 || color->green >= 254) { gradationDelta.y *= -1; }
			if (color->blue <= 2 || color->blue >= 254) { gradationDelta.z *= -1; }
		}
		void Draw2D() override {}
		void Draw3D() override {}
	public:
		void SetGradationPower(const int gradationPower) noexcept
		{
			gradationDelta = gradationPower;
		}
		void SetGradationPower(const Vec3& gradationPower) noexcept
		{
			gradationDelta = gradationPower;
		}
	private:
		Color* color;
		Vec3 gradationDelta;
	};
}