// hello.cpp
//#define _CRT_SECURE_NO_WARNINGS 
#include <node.h>
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>

using namespace std;

namespace rlairNodeAddon{

	using v8::Exception;
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Value;
	using v8::Number;
	
	const UINT uSupportFileAmmount = 10;

	//将UTF8宽字符数组转为多字节数组
	char * W2C(const TCHAR *wstr, int n)
	{
		int bufferSize = MAX_PATH * (uSupportFileAmmount + 1);
		static char sBuffer[MAX_PATH * (uSupportFileAmmount + 1)];
		memset(sBuffer, 0, sizeof(sBuffer));
		if (n > bufferSize) n = bufferSize;
		int i = WideCharToMultiByte(CP_UTF8, 0, wstr, n, sBuffer, bufferSize, NULL, NULL);
		return sBuffer;
	}

	// 这是 "GetClipboardContent" 方法的实现
	// 输入参数使用 const FunctionCallbackInfo<Value>& args 结构传入
	void GetClipboardContent(const FunctionCallbackInfo<Value>& args) 
	{
		Isolate* isolate = args.GetIsolate();

		UINT cFiles = 0;

		if (OpenClipboard(NULL))
		{
			HDROP hDrop = HDROP(GetClipboardData(CF_HDROP));
			if (hDrop)
			{
				cFiles = DragQueryFile(hDrop, (UINT)-1, NULL, 0);
				if (cFiles > 10)	//限制最多返回10个文件的路径
				{
					cFiles = 10;
				}

				TCHAR szBuffer[MAX_PATH * uSupportFileAmmount];
				memset(szBuffer, 0, sizeof(szBuffer));

				TCHAR szFile[MAX_PATH];
				for (UINT count = 0; count < cFiles; count++)
				{
					DragQueryFile(hDrop, count, szFile, sizeof(szFile));

					TCHAR szFileName[MAX_PATH + 10];
					UINT sizeTemp = sizeof(szFileName);
					memset(szFileName, 0, sizeof(szFileName));
					if (_tcslen(szBuffer) == 0)
					{
						_stprintf_s(szFileName, _T("\"%s\""), szFile);
					}
					else
					{
						_stprintf_s(szFileName, _T(",\"%s\""), szFile);
					}

					_tcscat_s(szBuffer, szFileName);
				}
				CloseClipboard();

				TCHAR szResultJson[MAX_PATH * (uSupportFileAmmount + 1)];
				memset(szResultJson, 0, sizeof(szResultJson));
				_stprintf_s(szResultJson, _T("[%s]"), szBuffer);

				char * resultJsonBytes = W2C(szResultJson, _tcslen(szResultJson));

				args.GetReturnValue().Set(String::NewFromUtf8(isolate, resultJsonBytes));
			}
			else
			{
				args.GetReturnValue().Set(String::NewFromUtf8(isolate, "[]"));
			}
		}
		else
		{
			args.GetReturnValue().Set(String::NewFromUtf8(isolate, "[]"));
		}
	}

	void Init(Local<Object> exports) 
	{
		NODE_SET_METHOD(exports, "getClipboardContent", GetClipboardContent);
	}

	NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}