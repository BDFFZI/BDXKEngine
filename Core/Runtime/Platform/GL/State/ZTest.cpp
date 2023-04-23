#include "ZTest.h"

namespace BDXKEngine
{
    ZTest ZTest::Default = {};
    ZTest ZTest::Background = {Operation::Always, 0, 0, false, true};

    void ZTest::Transfer(Transferer& transferer)
    {
        TransferFieldInfoOf(operation, int);
        TransferFieldInfo(factor);
        TransferFieldInfo(units);
        TransferFieldInfo(write);
        TransferFieldInfo(clip);
    }
    ZTest::ZTest() = default;
    ZTest::ZTest(Operation operation, float factor, float units, bool write, bool clip): operation(operation),
                                                                                         factor(factor),
                                                                                         units(units),
                                                                                         write(write),
                                                                                         clip(clip)
    {
    }
    void ZTest::UpdateGL()
    {
        depthStencilState = nullptr;
        D3D11_DEPTH_STENCIL_DESC description{};
        description.DepthEnable = true;
        description.DepthWriteMask = write ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        description.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(operation);
        const HRESULT result = GL::GetDevice()->CreateDepthStencilState(&description, &depthStencilState.p);
        if (FAILED(result))throw std::exception("创建深度测试参数失败");
    }
    void ZTest::UploadRP() const
    {
        GL::GetDeviceContext()->OMSetDepthStencilState(depthStencilState, 0);
    }
}
