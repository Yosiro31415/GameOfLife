/* This is a header for using game of life.
 * You can include this header. And using some class for game of life.
 * If you want to use, you can decleare by using namespace GOL
 * 
 * The following is a bullet list of how to use.
 * 1.Create instance of GameLife like "" GOL::GameLife GL= GameLife() "".
 * 2.Use update() method for update like "" GL.update() "".
 * 3.Use output() method, you can get updated 2Dvector configurated by int.
 * 3.or If you want to use this in Command line you can use dispOutput() method. 
 * over.
 * 
 * If you want to set initial conditions, you can use constructa by using prepared 2Dvector contains int.
 * exp ""GOL::GameLife GL= GameLife(preparedVector)  ""
 * 
 */

#ifndef _GAMEOFLIFE_
#define _GAMEOFLIFE_

#include <vector>
#include <iostream>
#include <random>

namespace GOL {
struct CellGoL {
    bool isAlive = false;
};

class GameLife {
    const int height;
    const int width;

    std::vector<std::vector<CellGoL>> tile = {}; //(y,x)coodinate
    int checkCell(int in_y, int in_x, std::vector<std::vector<CellGoL>>* oldTile);

public:
    GameLife();
    GameLife(int in_height, int in_width);
    GameLife(int probability);
    GameLife(int in_height, int in_width, int probability);
    GameLife(std::vector<std::vector<CellGoL>> in_tile);
    GameLife(std::vector<std::vector<int>> in_tile);
    ~GameLife();
    int update();
    std::vector<std::vector<int>> output();

    int dispOutput(GameLife* LG);
};

GameLife::GameLife() : width(500), height(500) {
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, 99);

    for (int i = 0; i < height; i++) {
        std::vector<CellGoL> line = {};
        for (int j = 0; j < width; j++) {
            CellGoL cell;
            if (dist(mt) < 20) {
                cell.isAlive = true;
            }
            line.push_back(cell);
        }
        tile.push_back(line);
    }
}

GameLife::GameLife(int probability) : width(500), height(500) {
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, 99);

    for (int i = 0; i < height; i++) {
        std::vector<CellGoL> line = {};
        for (int j = 0; j < width; j++) {
            CellGoL cell;
            if (dist(mt) < probability) {
                cell.isAlive = true;
            }
            line.push_back(cell);
        }
        tile.push_back(line);
    }
}

GameLife::GameLife(int in_height, int in_width) : width(in_width), height(in_height) {
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, 99);

    for (int i = 0; i < height; i++) {
        std::vector<CellGoL> line = {};
        for (int j = 0; j < width; j++) {
            CellGoL cell;
            if (dist(mt) < 20) {
                cell.isAlive = true;
            }
            line.push_back(cell);
        }
        tile.push_back(line);
    }
}

GameLife::GameLife(int in_height, int in_width, int probability) : width(in_width), height(in_height) {
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, 99);

    for (int i = 0; i < height; i++) {
        std::vector<CellGoL> line = {};
        for (int j = 0; j < width; j++) {
            CellGoL cell;
            if (dist(mt) < probability) {
                cell.isAlive = true;
            }
            line.push_back(cell);
        }
        tile.push_back(line);
    }
}

GameLife::GameLife(std::vector<std::vector<CellGoL>> in_tile) : height(in_tile.size()), width(in_tile.at(0).size()) {

    tile = in_tile;
};

GameLife::GameLife(std::vector<std::vector<int>> in_tile) : height(in_tile.size()), width(in_tile.at(0).size()) {

    for (int i = 0; i < height; i++) {
        std::vector<CellGoL> line = {};
        for (int j = 0; j < width; j++) {
            CellGoL cell;
            if (in_tile.at(i).at(j) > 0)
                cell.isAlive = true;
            line.push_back(cell);
        }
        tile.push_back(line);
    }
};

GameLife::~GameLife(){

};

int GameLife::update() {

    std::vector<std::vector<CellGoL>> oldTile;
    oldTile = tile;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            checkCell(i, j, &oldTile);
        }
    }
    return 0;
};

int GameLife::checkCell(int in_y, int in_x, std::vector<std::vector<CellGoL>>* oldTile) {

    int head = in_y + 1;
    int foot = in_y - 1;
    int right = in_x + 1;
    int left = in_x - 1;

    if (in_x > width || in_x < 0) {
        return 1;
    }
    if (in_y > height || in_y < 0) {
        return 1;
    }

    bool isRightSafe = right < width;
    bool isLeftSafe = left > 0;
    bool isHeadSafe = head < height;
    bool isFootSafe = foot > 0;

    int numLivCel = 0;
    if (isHeadSafe && isRightSafe && oldTile->at(head).at(right).isAlive)
        numLivCel++;
    if (isHeadSafe && oldTile->at(head).at(in_x).isAlive)
        numLivCel++;
    if (isHeadSafe && isLeftSafe && oldTile->at(head).at(left).isAlive)
        numLivCel++;
    if (isLeftSafe && oldTile->at(in_y).at(left).isAlive)
        numLivCel++;
    if (isRightSafe && oldTile->at(in_y).at(right).isAlive)
        numLivCel++;
    if (isRightSafe && isFootSafe && oldTile->at(foot).at(right).isAlive)
        numLivCel++;
    if (isFootSafe && oldTile->at(foot).at(in_x).isAlive)
        numLivCel++;
    if (isFootSafe && isLeftSafe && oldTile->at(foot).at(left).isAlive)
        numLivCel++;

    if (oldTile->at(in_y).at(in_x).isAlive) {
        if (numLivCel == 2 || numLivCel == 3) {
        } else
            tile.at(in_y).at(in_x).isAlive = false;
    } else {
        if (numLivCel == 3)
            tile.at(in_y).at(in_x).isAlive = true;
    }
    return 0;
};

int GameLife::dispOutput(GameLife* LG) {
    system("cls");
    std::vector<std::vector<int>> OP = LG->output();
    for (int i = 0; i < OP.size(); i++) {
        for (int j = 0; j < OP.at(0).size(); j++) {
            if (!OP.at(i).at(j)) {
                std::cout << "□ ";
            } else {
                std::cout << "■ ";
            }
        }
        std::cout << std::endl;
    }
    return 0;
};

std::vector<std::vector<int>> GameLife::output() {
    std::vector<std::vector<int>> OP;
    for (int i = 0; i < height; i++) {
        std::vector<int> line = {};
        for (int j = 0; j < width; j++) {
            CellGoL cell;
            if (tile.at(i).at(j).isAlive)
                cell.isAlive = true;
            line.push_back(cell.isAlive);
        }
        OP.push_back(line);
    }
    return OP;
};
} // namespace GOL
#endif
