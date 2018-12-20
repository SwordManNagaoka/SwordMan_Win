///**
//* @file MapLoader.hpp
//* @brief アンドロイドプログラミングでアクセスできるパスを取得します。
//* @author tonarinohito
//* @date 2018/9/18
//*/
//#pragma once
//#include <DxLib.h>
//#include <string>
//
//class DXFilieRead final
//{
//private:
//	char pathBuffer[1024];
//public:
//	const char* GetPath(const std::string& path, const char* tempFileName)
//	{
//		//ファイルを開く
//		int handle = FileRead_open(path.c_str());
//		if (handle == 0)
//		{
//			printfDx("HandleError!!!\n");
//		}
//		//ファイルのサイズを取得する
//		int64_t fileSize = FileRead_size(path.c_str());
//		if (fileSize == -1)
//		{
//			printfDx("SizeError!!!\n");
//		}
//		//ファイルからデータを読み込む
//		void *buffer = malloc(fileSize);
//		int isRead = FileRead_read(buffer, fileSize, handle);
//		if (isRead == -1)
//		{
//			printfDx("ReadError!!!\n");
//		}
//		//ファイルを閉じる
//		FileRead_close(handle);
//		//アプリのデータ保存用のディレクトリパスを取得する
//		GetInternalDataPath(pathBuffer, sizeof(pathBuffer));
//		//ディレクトリパスにディレクトリ区切りの『/』とテンポラリファイルの名前を追加する
//		strcat(pathBuffer, tempFileName);
//		//テンポラリファイルを書き込み用に開く
//		FILE* fp = fopen(pathBuffer, "wb");
//		if (fp != NULL)
//		{
//			//テンポラリファイルにcsvファイルの内容を書き出す
//			fwrite(buffer, 1, fileSize, fp);
//			//ファイルを閉じる
//			fclose(fp);
//		}
//		//確保したメモリを解放
//		free(buffer);
//		return pathBuffer;
//	}
//};
