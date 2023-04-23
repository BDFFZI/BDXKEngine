#include "Texture2DImporter.h"
#include <atlcomcli.h>
#include <wincodec.h>

#include "Core/Runtime/Base/Debug/Debug.h"
#include "Core/Runtime/Platform/GL/Texture/Texture2D.h"
#include "Core/Runtime/Platform/GL/Texture/TextureFormat.h"

namespace BDXKEditor
{
    ObjectPtr<Texture2D> Png(const std::filesystem::path& fileName)
    {
        ObjectPtr<Texture2D> texture2d;

        {
            HRESULT result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
            if (FAILED(result))throw std::exception("初始化COM失败");

            CComPtr<IWICImagingFactory> imagingFactory;
            result = CoCreateInstance(
                CLSID_WICImagingFactory,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&imagingFactory.p)
            );
            if (FAILED(result))throw std::exception("创建映像工厂失败");

            CComPtr<IWICBitmapDecoder> bitmapDecoder;
            result = imagingFactory->CreateDecoderFromFilename(
                fileName.wstring().c_str(),
                nullptr,
                GENERIC_READ,
                WICDecodeMetadataCacheOnDemand, &bitmapDecoder.p
            );
            if (FAILED(result))throw std::exception("创建位图解码器失败");

            CComPtr<IWICBitmapFrameDecode> bitmapFrameDecode;
            result = bitmapDecoder->GetFrame(0, &bitmapFrameDecode.p);
            if (FAILED(result))throw std::exception("创建位图帧解码器失败");

            WICPixelFormatGUID wicPixelFormatGuid;
            bitmapFrameDecode->GetPixelFormat(&wicPixelFormatGuid);

            TextureFormat textureFormat = TextureFormat::None;
            CComPtr<IWICBitmapSource> iwicBitmapSource = nullptr;
            if (wicPixelFormatGuid == GUID_WICPixelFormat24bppBGR)
            {
                textureFormat = TextureFormat::B8G8R8A8_UNORM;
                CComPtr<IWICFormatConverter> formatConverter;
                imagingFactory->CreateFormatConverter(&formatConverter.p);
                formatConverter->Initialize(
                    bitmapFrameDecode.p, GUID_WICPixelFormat32bppBGRA,
                    WICBitmapDitherTypeErrorDiffusion, nullptr, 0, WICBitmapPaletteTypeCustom
                );
                iwicBitmapSource = formatConverter;
            }
            else if (wicPixelFormatGuid == GUID_WICPixelFormat8bppIndexed)
            {
                textureFormat = TextureFormat::B8G8R8A8_UNORM;
                CComPtr<IWICFormatConverter> formatConverter;
                imagingFactory->CreateFormatConverter(&formatConverter.p);
                formatConverter->Initialize(
                    bitmapFrameDecode.p, GUID_WICPixelFormat32bppBGRA,
                    WICBitmapDitherTypeErrorDiffusion, nullptr, 0, WICBitmapPaletteTypeCustom
                );
                iwicBitmapSource = formatConverter;
            }
            else if (wicPixelFormatGuid == GUID_WICPixelFormat32bppBGRA)
            {
                textureFormat = TextureFormat::B8G8R8A8_UNORM;
                iwicBitmapSource = bitmapFrameDecode;
            }
            else if (wicPixelFormatGuid == GUID_WICPixelFormat128bppRGBAFloat)
            {
                textureFormat = TextureFormat::R32G32B32A32_FLOAT;
                iwicBitmapSource = bitmapFrameDecode;
            }

            else
            {
                Debug::LogWarning("发现不支持的图片格式");
            }

            if (textureFormat != TextureFormat::None)
            {
                unsigned int width, height;
                iwicBitmapSource->GetSize(&width, &height);
                const int pixelSize = GetPixelSize(textureFormat);
                const unsigned int bufferSize = width * height * pixelSize;

                const auto buffer = new unsigned char[bufferSize];
                iwicBitmapSource->CopyPixels(nullptr, width * pixelSize, bufferSize, buffer);

                texture2d = Texture2D::Create(
                    static_cast<int>(width), static_cast<int>(height),
                    textureFormat, reinterpret_cast<char*>(buffer), false
                );

                delete[] buffer;
            }
        }
        CoUninitialize();

        return texture2d;
    }

    ObjectPtrBase Texture2DImporter::Import()
    {
        ObjectPtrBase objectPtrBase = Png(GetTargetPath()).ToObject();
        BindObjectRef(objectPtrBase, 1000000002);
        return objectPtrBase;
    }
}
