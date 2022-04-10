#include "DialogWindow.h"

String DialogWindow::FileOpenDialog()
{
	HRESULT result = CoInitialize(NULL);//��ʼ��COM����
	if (FAILED(result)) {
		throw std::domain_error("COM��ʼ��ʧ�ܡ�");
	}

	IFileOpenDialog* dialog;
	result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, reinterpret_cast<void**>(&dialog));//����COM���󲢻�ȡ�ӿ�
	if (FAILED(result)) {
		CoUninitialize();
		throw std::domain_error("�Ի��򴴽�ʧ�ܡ�");
	}

	//��ѯ�ö���������ӿ�
	//IFileDialogCustomize* customize;
	//result = dialog->QueryInterface(IID_IFileDialogCustomize, reinterpret_cast<void**>(&customize));


	dialog->Show(NULL);
	IShellItem* shellItem;
	result = dialog->GetResult(&shellItem);
	if (FAILED(result)) {
		dialog->Release();
		CoUninitialize();
		throw std::domain_error("��ȡ�Ի����ʧ�ܡ�");
	}

	PWSTR str;
	result = shellItem->GetDisplayName(SIGDN_FILESYSPATH, &str);//����COM�������ڶ��Ϸ����ڴ�
	if (FAILED(result)) {
		shellItem->Release();
		dialog->Release();
		CoUninitialize();
		throw std::domain_error("��ȡ�Ի����ʧ�ܡ�");
	}

	String path(str);
	CoTaskMemFree(str);//�ͷŶ��ڴ�

	shellItem->Release();
	dialog->Release();//�ͷ�COM����
	CoUninitialize();//�˳�COM����
	return path;
}

String DialogWindow::FileSaveDialog()
{
	throw std::exception("δ�������");
}

