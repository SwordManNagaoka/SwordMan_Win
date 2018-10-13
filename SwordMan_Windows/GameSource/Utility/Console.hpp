#pragma once
#include <cstdio>
#include <iostream>
#include <windows.h>
/*!
@class Console
@brief Win32アプリでcin、coutを許可するクラスです
*/

//デバッグビルドのみ有効にする
#ifdef _DEBUG
#define DOUT std::cout
#else 
#define DOUT 0 && std::cout
#endif
class Console final
{
	std::FILE* in_;
	std::FILE* out_;
public:
	Console()
	{
		AllocConsole();
		freopen_s(&out_, "CONOUT$", "w", stdout); //標準出力をコンソールにする
		freopen_s(&in_, "CONIN$", "r", stdin);  //標準入力をコンソールにする
	}
	~Console()
	{
		std::fclose(in_);
		std::fclose(out_);
		FreeConsole();
	}
};

/*!
@class coutDbgString
@brief coutでデバッグウインドウに出力できるようになるクラスです
*/
class coutDbgString final
{
private:
	class DbgStreambuf final : public std::streambuf
	{
	public:
		std::streamsize xsputn(const char* s, std::streamsize n)
		{
			OutputDebugString(s);
			return n;
		}
		int_type overflow(int_type c = EOF)
		{
			if (c != EOF)
			{
				char buf[2] = { (char)c, '\0' };
				OutputDebugString(buf);
			}
			return c;
		}
	};
	std::streambuf* default_stream;
	DbgStreambuf debug_stream;
public:
	coutDbgString()
	{
		default_stream = std::cout.rdbuf(&debug_stream);
	}
	~coutDbgString()
	{
		std::cout.rdbuf(default_stream);
	}


#ifdef _DEBUG
	//複数はサポートしない
# define ShowConsole() Console c
# define StartOutputDbgString() coutDbgString c
#else
# define ShowConsole() __noop
# define StartOutputDbgString() __noop
#endif

#define dout std::cout << __FUNCTION__ << ":"
#define FILENAME_AND_LINE __FILE__ << ":" << __LINE__
};