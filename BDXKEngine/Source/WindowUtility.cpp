#include "WindowUtility.h"
#include "Com.h"

Vector2Int WindowUtility::GetWindowSize(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);

    return size;
}

String WindowUtility::GetOpenFilePath()
{
	HRESULT result = CoInitialize(NULL);//初始化COM环境
	if (FAILED(result)) {
		throw std::domain_error("COM初始化失败。");
	}

	IFileOpenDialog* dialog;
	result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, reinterpret_cast<void**>(&dialog));//创建COM对象并获取接口
	if (FAILED(result)) {
		CoUninitialize();
		throw std::domain_error("对话框创建失败。");
	}

	//查询该对象的其他接口
	//IFileDialogCustomize* customize;
	//result = dialog->QueryInterface(IID_IFileDialogCustomize, reinterpret_cast<void**>(&customize));


	dialog->Show(NULL);
	IShellItem* shellItem;
	result = dialog->GetResult(&shellItem);
	if (FAILED(result)) {
		dialog->Release();
		CoUninitialize();
		throw std::domain_error("获取对话结果失败。");
	}

	PWSTR str;
	result = shellItem->GetDisplayName(SIGDN_FILESYSPATH, &str);//调用COM函数，在堆上分配内存
	if (FAILED(result)) {
		shellItem->Release();
		dialog->Release();
		CoUninitialize();
		throw std::domain_error("获取对话结果失败。");
	}

	String path(str);
	CoTaskMemFree(str);//释放堆内存

	shellItem->Release();
	dialog->Release();//释放COM对象
	CoUninitialize();//退出COM环境
	return path;
}
