/**
* @file  Renderer.hpp
* @brief 描画関連のコンポーネントです。
* @note  特に理由がなければSpriteDrawやSpriteAnimationDrawが最も多機能なのでそれらの使用を推奨します
* @author tonarinohito
* @date 2018/10/06
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Collision/Collision.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../System/System.hpp"
#include <DxLib.h>
const float PI = 3.141592654f;
inline float ToRadian(float degrees) {return degrees * (PI / 180.0f);}
inline float ToDegree(float radians) {return radians * (180.0f / PI);}

namespace ECS
{
	/*!
	@brief 色のデータです
	*RGBを扱います。データの型はintです
	*/
	struct Color final : public ComponentData
	{
		int red, green, blue;

		Color() :
			red(255),
			green(255),
			blue(255)
		{}
		Color(const int r, const int g, const int b) :
			red(r),
			green(g),
			blue(b)
		{}
	};

	/*!
	@brief アルファブレンドに関するデータです
	*ブレンドモードとアルファ値を扱います。データの型はenum BlendStateとintです
	*/
	struct AlphaBlend final : public ComponentData
	{
		//!DXライブラリのブレンドモードを簡潔にまとめたものです
		enum BlendMode
		{
			NOBLEND = DX_BLENDMODE_NOBLEND,		//ノーブレンド（デフォルト）
			ALPHA = DX_BLENDMODE_ALPHA,		//ブレンド
			ADD = DX_BLENDMODE_ADD,			//加算ブレンド
			SUB = DX_BLENDMODE_SUB,			//減算ブレンド
			MULA = DX_BLENDMODE_MULA,			//乗算ブレンド
			INVSRC = DX_BLENDMODE_INVSRC,		//反転ブレンド
			PMA_ALPHA = DX_BLENDMODE_PMA_ALPHA,	//乗算済みα用のαブレンド
			PMA_ADD = DX_BLENDMODE_PMA_ADD,		//乗算済みα用の加算ブレンド
			PMA_SUB = DX_BLENDMODE_PMA_SUB,		//乗算済みα用の減算ブレンド
			PMA_INVSRC = DX_BLENDMODE_PMA_INVSRC,   //乗算済みα用の反転ブレンド
		};
		BlendMode blendMode;
		int alpha;
		AlphaBlend() :
			blendMode(ALPHA),
			alpha(255)
		{}
		AlphaBlend(const BlendMode& brend, const int alpha) :
			blendMode(brend),
			alpha(alpha)
		{}
	};

	//!描画系の処理に共通する処理をまとめたものです
	struct RenderUtility final
	{
		//!色を設定します
		static void SetColor(const Color* color)
		{
			if (color != nullptr)
			{
				//RGBの設定
				SetDrawBright(color->red, color->green, color->blue);
			}
		}
		//!アルファブレンドを設定します
		static void SetBlend(const AlphaBlend* blend)
		{
			if (blend != nullptr)
			{
				//ブレンドの設定
				SetDrawBlendMode(blend->blendMode, blend->alpha);
			}
		}
		//!描画の状態をもとに戻します。必ず描画終了時に呼び出してください
		static void ResetRenderState()
		{
			//変更した色の情報を元に戻す
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			SetDrawBright(255, 255, 255);
		}
		//!エンティティにColorとAlphaBlendを安全に参照させます。
		static void SatRenderDetail(const Entity* entity, Color** color, AlphaBlend** blend)
		{
			//色データがあれば反映する
			if (entity->HasComponent<Color>())
			{
				*color = &entity->GetComponent<Color>();
			}
			//ブレンド用データがあれば反映する
			if (entity->HasComponent<AlphaBlend>())
			{
				*blend = &entity->GetComponent<AlphaBlend>();
			}
		}
	};

	/*!
	@brief 簡易画像描画機能です。左上基準です
	* - Positionが必要です。
	* - 色を変えたい場合はColorが必要です
	* - アルファブレンドをしたい場合はAlphaBlendが必要です
	*/
	class SimpleDraw final : public Component
	{
	private:
		Position* pos_ = nullptr;
		Color* color_ = nullptr;
		AlphaBlend* blend_ = nullptr;
		std::string name_;
		bool isDraw_ = true;
		bool isCenter_ = false;
	public:
		//!登録した画像名を指定して初期化します
		SimpleDraw(const char* name)
		{
			assert(ResourceManager::GetGraph().IsExistenceHandle(name));
			name_ = name;
		}
		void Initialize() override
		{
			pos_ = &entity->GetComponent<Position>();
			RenderUtility::SatRenderDetail(entity, &color_, &blend_);
		}
		void Update() override {}
		void Draw2D() override
		{
			if (ResourceManager::GetGraph().IsExistenceHandle(name_) &&
				isDraw_)
			{
				RenderUtility::SetColor(color_);
				RenderUtility::SetBlend(blend_);
				if (!isCenter_)
				{
					DrawGraphF(
						pos_->val.x,
						pos_->val.y,
						ResourceManager::GetGraph().GetHandle(name_), true);
				}
				else
				{
					int w(0), h(0);
					GetGraphSize(ResourceManager::GetGraph().GetHandle(name_), &w, &h);
					DrawGraphF(
						pos_->val.x - (static_cast<float>(w) / 2),
						pos_->val.y - (static_cast<float>(h) / 2),
						ResourceManager::GetGraph().GetHandle(name_), true);
				}
				RenderUtility::ResetRenderState();
			}

		}
		//! @brief 描画を有効にします
		void DrawEnable()
		{
			isDraw_ = true;
		}
		//! @brief 描画を無効にします
		void DrawDisable()
		{
			isDraw_ = false;
		}
		//!描画する基準座標を中心にするか引数で指定します
		void DoCenter(const bool isCenter)
		{
			isCenter_ = isCenter;
		}
	};

	/*!
	@brief 指定した範囲を描画する機能です。左上基準です
	* - Positionが必要です。
	* - 色を変えたい場合はColorが必要です
	* - アルファブレンドをしたい場合はAlphaBlendが必要です
	*/
	class RectDraw final : public Component
	{
	private:
		Position* pos_ = nullptr;
		Color* color_ = nullptr;
		AlphaBlend* blend_ = nullptr;
		RECT rect;
		std::string name_;
		bool isDraw_ = true;
		bool isCenter_ = false;
		Vec2 offset;
	public:
		//!登録した画像名を指定して初期化します
		RectDraw(const char* name, const int srcX, const int srcY, const int w, const int h)
		{
			assert(ResourceManager::GetGraph().IsExistenceHandle(name));
			rect.left = srcX;
			rect.right = srcY;
			rect.bottom = w;
			rect.top = h;
			name_ = name;
		}
		void Initialize() override
		{
			pos_ = &entity->GetComponent<Position>();
			RenderUtility::SatRenderDetail(entity, &color_, &blend_);
			offset = { 0,0 };
		}
		void Update() override {}
		void Draw2D() override
		{
			if (ResourceManager::GetGraph().IsExistenceHandle(name_) &&
				isDraw_)
			{
				RenderUtility::SetColor(color_);
				RenderUtility::SetBlend(blend_);
				if (!isCenter_)
				{
					DrawRectGraphF(
						pos_->val.x + offset.x, pos_->val.y + offset.y,
						rect.left, rect.right,
						rect.bottom,
						rect.top,
						ResourceManager::GetGraph().GetHandle(name_),
						true);
				}
				else
				{
					DrawRectGraphF(
						pos_->val.x - (static_cast<float>(rect.bottom) / 2),
						pos_->val.y - (static_cast<float>(rect.top) / 2),
						rect.left, rect.right,
						rect.bottom,
						rect.top,
						ResourceManager::GetGraph().GetHandle(name_),
						true);
				}

				RenderUtility::ResetRenderState();
			}
		}
		//! @brief 描画を有効にします
		void DrawEnable()
		{
			isDraw_ = true;
		}
		//! @brief 描画を無効にします
		void DrawDisable()
		{
			isDraw_ = false;
		}
		//!描画する基準座標を中心にするか引数で指定します
		void DoCenter(const bool isCenter)
		{
			isCenter_ = isCenter;
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

	/*!
	@brief 分割画像を描画する機能です。また向きによって反転します。左上基準です
	* - Position必要です。
	* - Directionが必要です。
	* - 色を変えたい場合はColorが必要です
	* - アルファブレンドをしたい場合はAlphaBlendが必要です
	*/
	class AnimationDraw final : public Component
	{
	private:
		Position* pos_ = nullptr;
		Direction* dir = nullptr;
		AlphaBlend* blend_ = nullptr;
		Color* color_ = nullptr;
		std::string name_;
		int index_ = 0;
		bool isTurn = false;
		bool isDraw_ = true;
		Vec2 offsetAmount;
	public:
		//!登録した画像名を指定して初期化します
		AnimationDraw(const char* name)
		{
			assert(ResourceManager::GetGraph().IsExistenceDivHandle(name));
			name_ = name;
		}
		void Initialize() override
		{
			if (entity->HasComponent<Direction>())
			{
				dir = &entity->GetComponent<Direction>();
			}
			pos_ = &entity->GetComponent<Position>();
			RenderUtility::SatRenderDetail(entity, &color_, &blend_);
			offsetAmount = { 0,0 };
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
			if (ResourceManager::GetGraph().IsExistenceDivHandle(name_) &&
				isDraw_)
			{
				RenderUtility::SetColor(color_);
				RenderUtility::SetBlend(blend_);
				if (!isTurn)
				{
					DrawGraphF(
						pos_->val.x + offsetAmount.x,
						pos_->val.y + offsetAmount.y,
						ResourceManager::GetGraph().GetDivHandle(name_, index_),
						true);
				}
				else
				{
					DrawTurnGraphF(
						pos_->val.x + offsetAmount.x,
						pos_->val.y + offsetAmount.y,
						ResourceManager::GetGraph().GetDivHandle(name_, index_),
						true);
				}
				RenderUtility::ResetRenderState();
			}
		}
		//! @brief 描画したい分割画像の要素番号を指定します
		void SetIndex(const int index)
		{
			index_ = index;
		}
		//! @brief 描画を有効にします
		void DrawEnable()
		{
			isDraw_ = true;
		}
		//! @brief 描画を無効にします
		void DrawDisable()
		{
			isDraw_ = false;
		}
		//! @brief 画像位置のオフセット
		void	Offset(const Vec2& offset)
		{
			offsetAmount = offset;
		}
	};

	///*!
	//@brief 多機能な描画機能です。画像の中心が基準です
	//* - Transfromが必要です。
	//* - 色を変えたい場合はColorが必要です
	//* - アルファブレンドをしたい場合はAlphaBlendが必要です
	//*/
	//class SpriteDraw final : public Component
	//{
	//private:
	//	Position* pos_ = nullptr;
	//	Scale* scale_ = nullptr;
	//	Rotation* rota_ = nullptr;
	//	Color* color_ = nullptr;
	//	AlphaBlend* blend_ = nullptr;
	//	std::string name_;
	//	Vec2_i size_;
	//	bool isDraw_ = true;
	//	bool isCenter_ = true;
	//public:
	//	//!登録した画像名を指定して初期化します
	//	SpriteDraw(const char* name)
	//	{
	//		assert(ResourceManager::GetGraph().IsExistenceHandle(name));

	//		name_ = name;
	//	}
	//	void Initialize() override
	//	{
	//		pos_ = &entity->GetComponent<Position>();
	//		rota_ = &entity->GetComponent<Rotation>();
	//		scale_ = &entity->GetComponent<Scale>();
	//		GetGraphSize(ResourceManager::GetGraph().GetHandle(name_), &size_.x, &size_.y);
	//		RenderUtility::SatRenderDetail(entity, &color_, &blend_);
	//	}
	//	void Update() override {}
	//	void Draw2D() override
	//	{
	//		if (ResourceManager::GetGraph().IsExistenceHandle(name_) &&
	//			isDraw_)
	//		{
	//			RenderUtility::SetColor(color_);
	//			RenderUtility::SetBlend(blend_);
	//			if (!isCenter_)
	//			{
	//				DrawRotaGraph3F(
	//					pos_->val.x,
	//					pos_->val.y,
	//					0,
	//					0,
	//					scale_->val.x,
	//					scale_->val.y,
	//					ToRadian(rota_->val),
	//					ResourceManager::GetGraph().GetHandle(name_), true);
	//			}
	//			else
	//			{
	//				DrawRotaGraph3F(
	//					pos_->val.x,
	//					pos_->val.y,
	//					static_cast<float>(size_.x) / 2,
	//					static_cast<float>(size_.y) / 2,
	//					scale_->val.x,
	//					scale_->val.y,
	//					ToRadian(rota_->val),
	//					ResourceManager::GetGraph().GetHandle(name_), true);
	//			}
	//			RenderUtility::ResetRenderState();
	//		}

	//	}
	//	//! @brief 描画を有効にします
	//	void DrawEnable()
	//	{
	//		isDraw_ = true;
	//	}
	//	//! @brief 描画を無効にします
	//	void DrawDisable()
	//	{
	//		isDraw_ = false;
	//	}
	//	//!描画する基準座標を中心にするか引数で指定します
	//	void DoCenter(const bool isCenter)
	//	{
	//		isCenter_ = isCenter;
	//	}
	//};

	///*!
	//@brief 多機能な分割画像描画機能です。画像の左上が基準です
	//* - Transfromが必要です。
	//* - 色を変えたい場合はColorが必要です
	//* - アルファブレンドをしたい場合はAlphaBlendが必要です
	//* - setPivotで基準座標を変更できます
	//*/
	//class SpriteAnimationDraw final : public Component
	//{
	//private:
	//	Position* pos_ = nullptr;
	//	Scale* scale_ = nullptr;
	//	Rotation* rota_ = nullptr;
	//	Color* color_ = nullptr;
	//	AlphaBlend* blend_ = nullptr;
	//	std::string name_;
	//	Vec2 pivot_;
	//	bool isDraw_ = true;
	//	int index_ = 0;
	//public:
	//	//!登録した画像名を指定して初期化します
	//	SpriteAnimationDraw(const char* name)
	//	{
	//		assert(ResourceManager::GetGraph().IsExistenceDivHandle(name));
	//		name_ = name;
	//	}
	//	void Initialize() override
	//	{
	//		pos_ = &entity->GetComponent<Position>();
	//		rota_ = &entity->GetComponent<Rotation>();
	//		scale_ = &entity->GetComponent<Scale>();
	//		RenderUtility::SatRenderDetail(entity, &color_, &blend_);
	//	}
	//	void Update() override {}
	//	void Draw2D() override
	//	{
	//		if (ResourceManager::GetGraph().IsExistenceDivHandle(name_) &&
	//			isDraw_)
	//		{
	//			RenderUtility::SetColor(color_);
	//			RenderUtility::SetBlend(blend_);

	//			DrawRotaGraph3F(
	//				pos_->val.x,
	//				pos_->val.y,
	//				pivot_.x,
	//				pivot_.y,
	//				scale_->val.x,
	//				scale_->val.y,
	//				ToRadian(rota_->val),
	//				ResourceManager::GetGraph().GetDivHandle(name_, index_), true);
	//			RenderUtility::ResetRenderState();
	//		}

	//	}
	//	//! @brief 描画したい分割画像の要素番号を指定します
	//	void SetIndex(const int index)
	//	{
	//		index_ = index;
	//	}
	//	//! @brief 描画を有効にします
	//	void DrawEnable()
	//	{
	//		isDraw_ = true;
	//	}
	//	//! @brief 描画を無効にします
	//	void DrawDisable()
	//	{
	//		isDraw_ = false;
	//	}
	//	//!描画する基準座標を引数で指定します
	//	void SetPivot(const Vec2& pivot)
	//	{
	//		pivot_ = pivot;
	//	}
	//};
}