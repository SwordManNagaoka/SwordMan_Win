//----------------------------------------------------
//!@file	ButtonTag.hpp
//!@brief	ボタンのタグを作成します
//!@author	日比野　真聖
//!@date	2018/10/8
//----------------------------------------------------
#pragma once
#include "../../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../../GameController/GameController.h"



namespace ECS
{
	struct PauseButtonTag : public ComponentData {};
	struct StartFromBeginButtonTag : public ComponentData {};
	struct RetryButtonTag : public ComponentData {};
	struct ContinueButtonTag : public ComponentData {};
	struct BackMenuButtonTag : public ComponentData {};
}