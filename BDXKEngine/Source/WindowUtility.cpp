#include "WindowUtility.h"

Vector2Int WindowUtility::GetSize(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);

    return size;
}
