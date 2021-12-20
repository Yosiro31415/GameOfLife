#include "../include/HeaderDX11.hpp"

D3DManager* D3DManager::getPtrD3DManager() {
    static D3DManager dmanager = D3DManager();
    return &dmanager;
}

InputManager* InputManager::getPtrImanager() {
    static InputManager imanager = InputManager();
    return &imanager;
}

D3DManager::D3DManager() {
    hWnd = nullptr;
    ZeroMemory(&inf, sizeof(D3DInf));
}

void D3DManager::quit(HINSTANCE hInst) {
    UnregisterClassW(L"DX11_CLASS", hInst);
}

HWND* D3DManager::getWindowHandle() {
    return &hWnd;
}

D3DInf* D3DManager::getD3DInformation() {
    return &inf;
};