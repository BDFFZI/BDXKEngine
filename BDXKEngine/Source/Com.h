#pragma once
//__uuidof ��ȡUUID
//IID_PPV_ARGS �Զ����û�ȡID��ָ��ת��
//CComPtr ����ָ�� ��ǿ���Ƽ���

#include <shobjidl.h> //COM�ӿ�
#include<atlbase.h> //����COMָ��

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

