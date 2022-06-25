#include "Graphics.h"

CComPtr<ID3D11Buffer> Graphics::renderingMatrixBuffer = 0;
RenderingMatrix Graphics::renderingMatrix = {};