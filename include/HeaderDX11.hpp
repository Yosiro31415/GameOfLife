#pragma once
#ifndef _HEADER_HPP_
#define _HEADER_HPP_

#include <DirectXMath.h>
#include <d3d11.h>
#include <windows.h>
#include <wrl/client.h>
#include <cmath>
#include <map>
#include <xinput.h>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <sstream>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "xinput.lib")

using Microsoft::WRL::ComPtr;

struct ConstantBuffer {
    DirectX::XMFLOAT4X4 matScl;
    DirectX::XMFLOAT4X4 matRot;
    DirectX::XMFLOAT4X4 matTrs;
    DirectX::XMFLOAT4X4 matView;
    DirectX::XMFLOAT4X4 matProj;
    DirectX::XMFLOAT4 vecColor;
    DirectX::XMFLOAT4 vecLight;
    DirectX::XMFLOAT4 params;
};

struct D3DInf {
    //ジェネラル
    ComPtr<ID3D11Device> pDevice;
    ComPtr<ID3D11DeviceContext> pImContext;
    ComPtr<IDXGISwapChain> pSwapChain;
    ComPtr<ID3D11RenderTargetView> pRTView;
    ComPtr<ID3D11DepthStencilView> pDSView;
    D3D11_VIEWPORT viewport;

    // shader
    ComPtr<ID3D11VertexShader> pVShader;
    ComPtr<ID3D11PixelShader> pPShader;

    // polygon
    ComPtr<ID3D11Buffer> pCBuffer;
    ConstantBuffer cbuffer;
    // maybe vertex call
    ComPtr<ID3D11InputLayout> pILayout;
};

struct Vertex {
    float pcnu[12];
};

struct ModelInf {
    unsigned int numIdx;
    float posX, posY, posZ;
    float degX, degY, degZ;
    float sclX, sclY, sclZ;
    float colR, colG, colB, colA;
    ComPtr<ID3D11Buffer> pVBuffer;
    ComPtr<ID3D11Buffer> pIBuffer;
};

struct Camera {
    float width, height;
    float angle, nearZ, farZ;
    float posX, posY, posZ;
    float dirX, dirY, dirZ;
    float uppX, uppY, uppZ;
};

struct Texture {
    ~Texture() {
        // std::cout << pSRView.Reset() << std::endl;
    }
    unsigned int width, height;
    ComPtr<ID3D11ShaderResourceView> pSRView;
};

class D3DManager {
private:
    HWND hWnd;
    D3DInf inf;
    D3DManager();

public:
    static D3DManager* getPtrD3DManager();
    void quit(HINSTANCE hInst);
    // general
    void drawBegin();
    void drawEnd();
    bool init(HINSTANCE hInst, int cmdShow, LPCWSTR nameWnd, LPCWSTR nameWndClass, unsigned int width,
        unsigned int height, bool windowed);
    HWND* getWindowHandle();

    // model
    bool createModelBuffers(unsigned int numVtx, Vertex* dataPCU, unsigned int* dataIdxm, ModelInf* minf);
    void drawModel(ModelInf* minf);

    // debug
    void DebugBox(int i);
    void DebugBox(const char* str);
    D3DInf* getD3DInformation();

    // camera
    void createCamera(float width, float height, Camera* camera);
    void applyCamera(Camera* camera);

    bool createTexture(unsigned int id, Texture* pTexture);
    void applyTexture(Texture* pTexture);

    void setMatrixScale(float sclX, float sclY, float sclZ);
    void setMatrixRotate(float degX, float degY, float degZ);
    void setMatrixTranslate(float posX, float posY, float posZ);
    void setMatrixView(
        float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float uppX, float uppY, float uppZ);
    void setMatrixProject(float width, float height, float angle, float nearZ, float farZ, bool parse);
    void setVectorColor(float colR, float colG, float colB, float colA);
    void setVectorLight(float x, float y, float z, float w);
    void setVectorParams(float x, float y, float z, float w);
};

// input
enum struct GAMEPAD_KEYTYPE : int {
    Buttons,
    LTrigger,
    RTrigger,
    ThumbLL,
    ThumbLR,
    ThumbLU,
    ThumbLD,
    ThumbRL,
    ThumbRR,
    ThumbRU,
    ThumbRD,
};

struct KeyInf {
    char state;
    char codeKey;
    char codeKeyboard;
    GAMEPAD_KEYTYPE typeGamepadKey;
    short codeGamepad;
};

class InputManager {
private:
    int numRegistered;
    char statePrev[64];
    KeyInf inf[64];
    InputManager();

public:
    static InputManager* getPtrImanager();
    bool addKeycode(char codeKey, char codeKeyboard, GAMEPAD_KEYTYPE typeGamepadKey, short codeGamepad);
    void inspect();
    char getKey(char codeKey);
};

enum KEY : int {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    Z,
    C,
    ENTER,
};



namespace NUM_C {
const float PI = 3.141592;
} // namespace NUM_C

class Random {
public:
    static float Num(float up, float low) {
        std::mt19937 mt{std::random_device{}()};
        std::uniform_real_distribution<> dist(up, low);
        return dist(mt);
    }
};

class Deb {
public:
    static std::string cout(std::string str) {
        std::cout << str << std::endl;
        return str;
    }
    static int cout(int num) {
        std::cout << num << std::endl;
        return num;
    }
    static float cout(float num) {
        std::cout << num << std::endl;
        return num;
    }
};

struct isCollision {
    bool HEAD = false;
    bool FOOT = false;
    bool RIGHT = false;
    bool LEFT = false;
    bool H_R = false;
    bool H_L = false;
    bool F_R = false;
    bool F_L = false;
};
#endif
