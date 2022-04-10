#pragma once
//__uuidof 获取UUID
//IID_PPV_ARGS 自动安全配置接口指针
//CComPtr 智能指针 【强烈推荐】

#include <shobjidl.h> 
#include<atlbase.h>
#include"Assert.h"
class Com
{
public:
	/// <summary>
	/// 安全释放指针
	/// </summary>
	template <class T>
	static void SafeRelease(T** ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}
};

