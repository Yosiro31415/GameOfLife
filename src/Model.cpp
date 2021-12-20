#include "../include/HeaderApp.hpp"

Model::Model() {
    numIdx = 0U;
    posX = 0.0f;
    posY = 0.0f;
    posZ = 0.0f;
    degX = 0.0f;
    degY = 0.0f;
    degZ = 0.0f;
    sclX = 1.0f;
    sclY = 1.0f;
    sclZ = 1.0f;
    colR = 1.0f;
    colG = 1.0f;
    colB = 1.0f;
    colA = 1.0f;
    pVBuffer = nullptr;
    pIBuffer = nullptr;
}