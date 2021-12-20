#include "../include/HeaderDX11.hpp"

void D3DManager::createCamera(float width, float height, Camera* camera) {
    *camera = {width, height, DirectX::XM_PIDIV4, 1.0f, 2000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
}

void D3DManager::applyCamera(Camera* camera) {
    DirectX::XMStoreFloat4x4(&inf.cbuffer.matView,
        DirectX::XMMatrixTranspose(
            DirectX::XMMatrixLookToLH(DirectX::XMVectorSet(camera->posX, camera->posY, camera->posZ, 0.0f),
                DirectX::XMVectorSet(camera->dirX, camera->dirY, camera->dirZ, 0.0f),
                DirectX::XMVectorSet(camera->uppX, camera->uppY, camera->uppZ, 0.0f))));
    DirectX::XMStoreFloat4x4(&inf.cbuffer.matProj, DirectX::XMMatrixTranspose(DirectX::XMMatrixOrthographicLH(
                                                       camera->width, camera->height, camera->nearZ, camera->farZ)));
}