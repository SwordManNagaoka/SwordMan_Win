//----------------------------------------------------
//!@file	TotalScoreDraw.hpp
//!@brief	合計スコア表示を作成します
//!@author	日比野　真聖
//!@date	2018/10/6
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Utility/Counter.hpp"
#include "../Components/ImageFontDraw.hpp"
#include "../Utility/Converter.hpp"


namespace ECS
{
	class TotalScoreDraw : public Component
	{
	public:
		TotalScoreDraw() 
			: score(0)
		{}
		TotalScoreDraw(const int totalScore)
			: score(totalScore)
		{}
	public:
		void	Initialize() override
		{
			scoreCount = 0;
			raito = 0.05f;
			if (entity->HasComponent<ImageFontDraw>())
			{
				fontDraw = &entity->GetComponent<ImageFontDraw>();
			}
		}
		void	Update() override
		{
			if (scoreCount < score)
			{
				int distance = abs(score - scoreCount);
				if (distance >= MaxScoreDistance)
				{
					int plusScore = static_cast<int>(static_cast<float>(distance) * raito);
					scoreCount += plusScore;
				}
				else
				{
					++scoreCount;
				}
				if (scoreCount >= score) { scoreCount = score; }
			}
		}
		void	Draw2D() override
		{
			if (fontDraw == nullptr) { return; }
			fontDraw->SetDrawData(std::to_string(scoreCount).c_str());
		}
	public:
		//合計のスコアに加える
		void AddScore(const int addScore)
		{
			score += addScore;
		}
	private:
		void	Draw3D() override {}
	private:
		int	score;
		int scoreCount;
		float raito;
		const int MaxScoreDistance = 50;
		ImageFontDraw* fontDraw = nullptr;
	};
}