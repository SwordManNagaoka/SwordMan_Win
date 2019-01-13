/**
* @file System.hpp
* @brief Dxlibの初期化とシステムのチェックを行います
* @author tonarinohito
* @date 2018/8/29
*/
#pragma once
#include <DxLib.h>
#include <assert.h>
#ifndef __ANDROID__
#include <Windows.h>
static bool IsFullScreen()
{
	int flag;
	flag = MessageBox(
		NULL,
		TEXT("フルスクリーンモードで起動しますか？"),
		TEXT("スクリーン設定"),
		MB_YESNO | MB_ICONQUESTION);
	if (flag == IDNO)
	{
		return true;
	}
	return false;
}
#endif
class System final
{
private:
	void SystemInit()
	{
		//ウィンドウがノンアクティブ時は実行しない
		SetAlwaysRunFlag(false);
		//ログ消し
		SetOutApplicationLogValidFlag(false);
		//縦横比維持
		SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);
#ifndef __ANDROID__
		// Windows版のコンパイルだったら ChangeWindowMode を実行する
		ChangeWindowMode(IsFullScreen());
		//XAudio2を使用する
		SetEnableXAudioFlag(true);
		//ウインドウタイトルを変更
		SetMainWindowText("SwordManNagaoka");
		//アイコン
		SetWindowIconID(222);
#endif
		//画面サイズ変更
		SetGraphMode(1280,720, 32);
		SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
		//初期化
		DxLib_Init();
		assert(DxLib_IsInit());
		//裏画面設定
		SetDrawScreen(DX_SCREEN_BACK);
	}
	const bool Processloop() const
	{
		if (ScreenFlip() != 0) return false;
		if (ProcessMessage() != 0) return false;
		if (ClearDrawScreen() != 0) return false;
		return true;
	}
public:
	static constexpr int
	SCREEN_WIDIH = 1280,
	SCREEN_HEIGHT = 720;
	System()
	{
		SystemInit();


	}

	~System()
	{
		DxLib_End();
	}

	const bool IsOk() const
	{
		return Processloop();
	}
};