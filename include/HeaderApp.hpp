#include "../include/HeaderDX11.hpp"

#ifndef _HEADER_HPP_APP_
#define _HEADER_HPP_APP_

class Model : public ModelInf {
public:
    Model();
    virtual bool init(D3DManager* pManager) = 0;
};

class ModelSquare : public Model {
public:
    bool init(D3DManager* pManager);
};


#endif