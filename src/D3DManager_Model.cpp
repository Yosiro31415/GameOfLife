#include "../include/HeaderDX11.hpp"

bool D3DManager::createModelBuffers(unsigned int numVtx, Vertex* dataPCU, unsigned int* dataIdx, ModelInf* minf) {

    // vertex buffer
    D3D11_BUFFER_DESC descVBuffer = {
        sizeof(Vertex) * numVtx, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0U, 0U, 0U};
    D3D11_SUBRESOURCE_DATA dataVBuffer = {dataPCU, 0U, 0U};
    if (FAILED(inf.pDevice->CreateBuffer(&descVBuffer, &dataVBuffer, minf->pVBuffer.GetAddressOf()))) {
        return false;
    }

    // index buffer
    D3D11_BUFFER_DESC descIndex = {
        (unsigned int)sizeof(unsigned int) * minf->numIdx, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0U, 0U, 0U};
    D3D11_SUBRESOURCE_DATA dataIndex = {dataIdx, 0U, 0U};
    if (FAILED(inf.pDevice->CreateBuffer(&descIndex, &dataIndex, minf->pIBuffer.GetAddressOf()))) {
        return false;
    }
    return true;
}

void D3DManager::drawModel(ModelInf* minf) {
    unsigned int strides = sizeof(Vertex);
    unsigned int offsets = 0U;
    inf.pImContext->IASetVertexBuffers(0U, 1U, minf->pVBuffer.GetAddressOf(), &strides, &offsets);
    inf.pImContext->IASetIndexBuffer(minf->pIBuffer.Get(), DXGI_FORMAT_R32_UINT, 0U);

    // put infModel into constantBuffer
    DirectX::XMStoreFloat4x4(
        &inf.cbuffer.matScl, DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(minf->sclX, minf->sclY, minf->sclZ)));
    DirectX::XMStoreFloat4x4(&inf.cbuffer.matRot,
        DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(minf->degX),
            DirectX::XMConvertToRadians(minf->degY), DirectX::XMConvertToRadians(minf->degZ))));
    DirectX::XMStoreFloat4x4(&inf.cbuffer.matTrs,

        DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(minf->posX, minf->posY, minf->posZ)));
    DirectX::XMStoreFloat4(&inf.cbuffer.vecColor, DirectX::XMVectorSet(minf->colR, minf->colG, minf->colB, minf->colA));

    // put constantBuffer into sheder
    inf.pImContext->UpdateSubresource(inf.pCBuffer.Get(), 0U, nullptr, &inf.cbuffer, 0U, 0U);
    inf.pImContext->VSSetConstantBuffers(0U, 1U, inf.pCBuffer.GetAddressOf());

    inf.pImContext->DrawIndexed(minf->numIdx, 0U, 0U);
}
