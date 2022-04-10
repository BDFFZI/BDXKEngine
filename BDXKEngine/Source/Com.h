#pragma once
//__uuidof ��ȡUUID
//IID_PPV_ARGS �Զ���ȫ���ýӿ�ָ��
//CComPtr ����ָ�� ��ǿ���Ƽ���

#include <shobjidl.h> 
#include<atlbase.h>
#include"Assert.h"
class Com
{
public:
	/// <summary>
	/// ��ȫ�ͷ�ָ��
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

