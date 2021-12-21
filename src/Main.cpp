#include "../include/HeaderApp.hpp"
#include "../include/resource.hpp"
#include "../include/LifeGame.hpp"

namespace WindowSize {
int height = 1824U;
int width = 2736U;
} // namespace WindowSize

int main() {
    Deb::cout("CONSOLE LAUNCHED");
    return Deb::cout(WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOWDEFAULT));
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPInst, LPSTR pCmd, int cmdShow) {

    int cellLength = 0;
    while (cellLength <= 0) {
        std::cout << "セルの長さを決めてください." << std::endl;
        std::cin >> cellLength;
        std::cout << std::endl;
    }

    bool isNotRandom;
    std::cout << "初期条件を乱数で出力するなら0を入力してください．" << std::endl
              << "それ以外ならglider gunを横に並べたものを設定します" << std::endl;
    std::cin >> isNotRandom;
    std::cout << std::endl;

    int probability = 20;
    if (!isNotRandom) {
        std::cout << "初期条件でに各セルが生きている確率を入力してください" << std::endl
                  << "初期条件では20%になってます." << std::endl;
        std::cin >> probability;
        std::cout << std::endl;
    }

    // input
    InputManager* imanager = InputManager::getPtrImanager();
    imanager->addKeycode(KEY::Z, 0x5A, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);
    imanager->addKeycode(KEY::C, 0x43, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);

    std::cout << "起動します．" << std::endl
              << "Zキーで停止します." << std::endl
              << "続けるにはCキーを押してください." << std::endl;
    while (!imanager->getKey(KEY::C)) {
        imanager->inspect();
    }
    std::cout << std::endl;

    WindowSize::height = GetSystemMetrics(SM_CYSCREEN);
    WindowSize::width = GetSystemMetrics(SM_CXSCREEN);

    // glider gunを並べた盤面の用意
    std::vector<std::vector<int>> board(
        WindowSize::height / cellLength, std::vector<int>(WindowSize::width / cellLength, 0));
    int B = 2;
    for (int i = 0; i < (WindowSize::width / cellLength / 50); i++) {
        board[B + 25][B + 40 * i + 1] = 1;
        board[B + 23][B + 40 * i + 2] = 1;
        board[B + 25][B + 40 * i + 2] = 1;
        board[B + 13][B + 40 * i + 3] = 1;
        board[B + 14][B + 40 * i + 3] = 1;
        board[B + 21][B + 40 * i + 3] = 1;
        board[B + 22][B + 40 * i + 3] = 1;
        board[B + 35][B + 40 * i + 3] = 1;
        board[B + 36][B + 40 * i + 3] = 1;
        board[B + 12][B + 40 * i + 4] = 1;
        board[B + 16][B + 40 * i + 4] = 1;
        board[B + 21][B + 40 * i + 4] = 1;
        board[B + 22][B + 40 * i + 4] = 1;
        board[B + 35][B + 40 * i + 4] = 1;
        board[B + 36][B + 40 * i + 4] = 1;
        board[B + 1][B + 40 * i + 5] = 1;
        board[B + 2][B + 40 * i + 5] = 1;
        board[B + 11][B + 40 * i + 5] = 1;
        board[B + 17][B + 40 * i + 5] = 1;
        board[B + 21][B + 40 * i + 5] = 1;
        board[B + 22][B + 40 * i + 5] = 1;
        board[B + 1][B + 40 * i + 6] = 1;
        board[B + 2][B + 40 * i + 6] = 1;
        board[B + 11][B + 40 * i + 6] = 1;
        board[B + 15][B + 40 * i + 6] = 1;
        board[B + 17][B + 40 * i + 6] = 1;
        board[B + 18][B + 40 * i + 6] = 1;
        board[B + 23][B + 40 * i + 6] = 1;
        board[B + 25][B + 40 * i + 6] = 1;
        board[B + 11][B + 40 * i + 7] = 1;
        board[B + 17][B + 40 * i + 7] = 1;
        board[B + 25][B + 40 * i + 7] = 1;
        board[B + 12][B + 40 * i + 8] = 1;
        board[B + 16][B + 40 * i + 8] = 1;
        board[B + 13][B + 40 * i + 9] = 1;
        board[B + 14][B + 40 * i + 9] = 1;
    }
    std::vector<GOL::GameLife*> ptrLG = {};

    if (isNotRandom) {
        GOL::GameLife* ptr = new GOL::GameLife(board);
        ptrLG.push_back(ptr);
    } else {
        GOL::GameLife* ptr =
            new GOL::GameLife(WindowSize::height / cellLength, WindowSize::width / cellLength, probability);
        ptrLG.push_back(ptr);
    }

    LPCWSTR kNameWnd = L"LIFE_GAME";
    LPCWSTR kNameWndClass = L"DX11_CLASS";
    const unsigned int kWidth = WindowSize::width;
    const unsigned int kHeight = WindowSize::height;

    // D3DManager
    D3DManager* dmanager = D3DManager::getPtrD3DManager();
    dmanager->init(hInst, cmdShow, kNameWnd, kNameWndClass, kWidth, kHeight, false);

    MSG msg;

    // camera
    Camera camera = Camera();
    dmanager->createCamera(kWidth, kHeight, &camera);
    camera.posZ = -20.f;
    dmanager->applyCamera(&camera);

    // model
    ModelSquare model = ModelSquare();
    model.init(dmanager);
    model.colA = 1;
    model.colB = 1;
    model.colR = 1;
    model.colG = 1;
    model.sclX = cellLength;
    model.sclY = cellLength;
    // texture
    Texture tex = Texture();
    dmanager->createTexture(BOLD, &tex);
    dmanager->applyTexture(&tex);
    // main loop
    float Time = 0.;
    while (true) {
        if (PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                return 1;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        } else {
            dmanager->drawBegin();
            ptrLG.at(0)->update();
            std::vector<std::vector<int>> tile = ptrLG.at(0)->output();
            for (int i = 0; i < tile.size(); i++) {
                for (int j = 0; j < tile.at(0).size(); j++) {
                    if (tile.at(i).at(j)) {
                        model.posY = cellLength * i - WindowSize::height / 2;
                        model.posX = cellLength * j - WindowSize::width / 2;
                        dmanager->drawModel(&model);
                    }
                }
            }
            dmanager->drawEnd();
            imanager->inspect();
            if (imanager->getKey(KEY::Z)) {
                PostQuitMessage(NULL);
            }
        }
    }
    delete ptrLG.at(0);
    std::cout << "STOPPED";
    UnregisterClassW(kNameWndClass, hInst);
    return 0;
}