#include "../include/HeaderDX11.hpp"
#include "../include/resource.hpp"

#include <wincodec.h>
#pragma comment(lib, "Windowscodecs.lib")
#pragma comment(lib, "Ole32.lib")

bool D3DManager::createTexture(unsigned int id, Texture* pTexture) {
    try {
        HRSRC hImageRes = FindResourceA(nullptr, MAKEINTRESOURCEA(id), "IMAGE");
        if (!hImageRes)
            throw "failed to find resource";

        HGLOBAL hImageData = LoadResource(nullptr, hImageRes);
        if (!hImageData)
            throw "failed to load resource";

        void* pLock = LockResource(hImageData);
        if (!pLock)
            throw "failed to lock resource";

        DWORD sizeRes = SizeofResource(nullptr, hImageRes);
        if (!sizeRes)
            throw "failed to get size of resource";

        ComPtr<IWICImagingFactory> pFactory = nullptr;
        if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory,
                (LPVOID*)pFactory.GetAddressOf())))
            throw "failed to create wic factory";

        ComPtr<IWICStream> pStream = nullptr;
        if (FAILED(pFactory->CreateStream(pStream.GetAddressOf())))
            throw "failed to create stream";

        if (FAILED(pStream->InitializeFromMemory((byte*)pLock, sizeRes)))
            throw "failed to initialize stream";

        ComPtr<IWICBitmapDecoder> pDecoder = nullptr;
        if (FAILED(pFactory->CreateDecoderFromStream(
                pStream.Get(), nullptr, WICDecodeMetadataCacheOnLoad, pDecoder.GetAddressOf())))
            throw "failed to load file";

        ComPtr<IWICBitmapFrameDecode> pFrame = nullptr;
        if (FAILED(pDecoder->GetFrame(0U, pFrame.GetAddressOf())))
            throw "failed to get frame";

        ComPtr<IWICFormatConverter> pFormatConverter = nullptr;
        if (FAILED(pFactory->CreateFormatConverter(pFormatConverter.GetAddressOf())))
            throw "failed to craete format converter";

        if (FAILED(pFormatConverter->Initialize(pFrame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone,
                nullptr, 1.0f, WICBitmapPaletteTypeMedianCut)))
            throw "failed to initialize format";

        if (FAILED(pFormatConverter->GetSize(&pTexture->width, &pTexture->height)))
            throw "failed to get texture size";

        D3D11_TEXTURE2D_DESC descTex = {pTexture->width, pTexture->height, 1U, 1U, DXGI_FORMAT_R8G8B8A8_UNORM, {1U, 0U},
            D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0U};
        ComPtr<ID3D11Texture2D> pTex = nullptr;
        if (FAILED(inf.pDevice->CreateTexture2D(&descTex, nullptr, pTex.GetAddressOf())))
            throw "Failed to create texture resource";

        D3D11_MAPPED_SUBRESOURCE resMapped;
        ZeroMemory(&resMapped, sizeof(D3D11_MAPPED_SUBRESOURCE));
        if (FAILED(inf.pImContext->Map(pTex.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &resMapped)))
            throw "failed to map";
        if (FAILED(pFormatConverter->CopyPixels(
                nullptr, pTexture->width * 4U, pTexture->width * pTexture->height * 4U, (BYTE*)resMapped.pData)))
            throw "failed to copy pixcels";

        inf.pImContext->Unmap(pTex.Get(), 0U);

        D3D11_SHADER_RESOURCE_VIEW_DESC descShader;
        ZeroMemory(&descShader, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        descShader.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        descShader.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        descShader.Texture2D.MipLevels = 1U;

        if (FAILED(inf.pDevice->CreateShaderResourceView(pTex.Get(), &descShader, pTexture->pSRView.GetAddressOf())))
            throw "failed to create shader resource view";
    } catch (std::string error) {
        Deb::cout(error);
        return false;
    }
    return true;
}
/**
void D3DManager::applyTexture(Texture* pTexture) {
    inf.pImContext->PSSetShaderResources(0U, 1U, pTexture->pSRView.GetAddressOf());
}
**/
void D3DManager::applyTexture(Texture* pTexture) {
    if (pTexture == nullptr || pTexture->pSRView == nullptr) {
        inf.cbuffer.params.x = 0.0f;
    } else {
        inf.pImContext->PSSetShaderResources(0U, 1U, pTexture->pSRView.GetAddressOf());
        inf.cbuffer.params.x = 1.0f;
    }
}
