#pragma once
//__uuidof 获取UUID
//IID_PPV_ARGS 自动配置获取ID和指针转换
//CComPtr 智能指针 【强烈推荐】

#include <shobjidl.h> //COM接口
#include<atlbase.h> //智能COM指针

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

