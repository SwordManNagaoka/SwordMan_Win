//----------------------------------------------------
//!@file	ImageFontDraw.hpp
//!@brief	フォント描画機能を作成します
//!@author	日比野　真聖
//!@date	2018/10/6
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Utility/Counter.hpp"
#include "../Components/Renderer.hpp"


namespace ECS
{
	//------------------------------------------------------------------
	//画像文字の描画を行う
	//コンストラクタ
	//ImageFontDraw( 画像1つのチップサイズ, src側の横の文字数 );
	//***型***
	//ImageFontDraw( Vec2 , const int )
	//------------------------------------------------------------------
	class ImageFontDraw : public Component
	{
	public:
		ImageFontDraw(const std::string& imageName, const Vec2& srcChipSize, const int widthCharNum)
		{
			imageChipSize = srcChipSize;
			charNum = widthCharNum;
			offsetPos = Vec2(imageChipSize.x, 0);
			imageNameStr = imageName;
		}
		void Initialize() override
		{
			isNumberFont = false;
			pos = &entity->GetComponent<Position>();
			if (entity->HasComponent<Scale>())
			{
				scale = &entity->GetComponent<Scale>();
			}
			else
			{
				scale = &entity->AddComponent<Scale>();
			}
			if (entity->HasComponent<RectGraphDraw>())
			{
				rectDraw = &entity->GetComponent<RectGraphDraw>();
			}
			else
			{
				rectDraw = &entity->AddComponent<RectGraphDraw>(imageNameStr.c_str(), 0, 0, imageChipSize.x, imageChipSize.y);
			}
		}
		void Update() override {}
		void Draw2D() override
		{
			if (isNumberFont)
			{
				NumberFontDraw(dataStr.c_str());
			}
			else
			{
				CharFontDraw(dataStr.c_str());
			}
		}
	public:
		//描画する文字列をセット
		void SetDrawData(const char* drawData)
		{
			dataStr = drawData;
		}
		//表示する文字のオフセット
		void SetOffset(const Vec2& offset)
		{
			offsetPos = offset;
		}
		//フォント画像の種類をセット
		//数字だけのフォント画像なら true
		//文字+数字のフォント画像なら false
		void SetFontImageKind(bool kindFlag)
		{
			isNumberFont = kindFlag;
		}
	private:
		void Draw3D() override {}
		//文字画像フォント描画用(数字含む)
		void	CharFontDraw(const char* numberStr)
		{
			//描画の位置
			Vec2	distance = pos->val;
			for (int i = 0; i < static_cast<int>(strlen(numberStr)); ++i)
			{
				int code = static_cast<unsigned char>((numberStr[i]));
				int fx = code % charNum * imageChipSize.x;
				int fy = code / charNum * imageChipSize.y;
				rectDraw->SetRect(fx, fy, imageChipSize.x, imageChipSize.y);
				rectDraw->SetOffset(offsetPos.x * scale->val * i, offsetPos.y * scale->val);
				rectDraw->Draw2D();
			}
		}
		//数字だけの画像フォント描画用
		void NumberFontDraw(const char* dataStr)
		{
			Vec2 distance = pos->val;
			for (int i = 0; i < static_cast<int>(strlen(dataStr)); ++i)
			{
				int code = static_cast<unsigned char>((dataStr[i]));
				int fx = (code - '0') * imageChipSize.x;
				int fy = 0 * imageChipSize.y;
				rectDraw->SetRect(fx, fy, imageChipSize.x, imageChipSize.y);
				rectDraw->SetOffset(offsetPos.x * scale->val * i, offsetPos.y * scale->val);
				rectDraw->Draw2D();
			}
		}
	private:
		std::string dataStr;
		std::string imageNameStr;
		Vec2 imageChipSize;
		Vec2 offsetPos;
		int charNum;
		bool isNumberFont;
		Position* pos = nullptr;
		Scale* scale = nullptr;
		RectGraphDraw* rectDraw = nullptr;
	};
}
