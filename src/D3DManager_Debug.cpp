#include "../include/HeaderDX11.hpp"
#include <stdio.h>

void D3DManager::DebugBox(int i) {
    char buff[64] = "";
    sprintf(buff, "%d", i);
    MessageBoxA(nullptr, buff, "int", MB_OK);
}

void D3DManager::DebugBox(const char* str) {
    MessageBoxA(nullptr, str, "const char*", MB_OK);
}