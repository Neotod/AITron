#include "ai.h"
#include "chillncode.h"

#include <iostream>
#include <vector>
#include <map>

#include "effolkronium/random.hpp"
#include "square.h"

using namespace std;
using Random = effolkronium::random_static;

using namespace koala::chillin::client;
using namespace ks::models;
using namespace ks::commands;

std::vector <Square> squares;
std::vector <int> nearestSquares;
int currentSquareNum;

Chillncode chillncode;

std::vector < std::vector <int> > reachingPath;

AI::AI(World *world): RealtimeAI<World*>(world)
{
}

AI::~AI()
{
}

void AI::initialize()
{
    vector < vector <ECell> > board = this->world->board();
    auto agent = this->world->agents()[this->mySide];

    chillncode.setRequirements(&board, &squares, &nearestSquares, agent, this->mySide);
    chillncode.makeSquares(squares);
    currentSquareNum = chillncode.findSquareNum(agent.position().y(), agent.position().x());
    chillncode.findNearestSquares(nearestSquares, currentSquareNum);
}

void AI::decide()
{
    bool newSquare = chillncode.isNewSquare(currentSquareNum);
    if(newSquare == true)
    {
           chillncode.findNearestSquares(nearestSquares, currentSquareNum);
    }
}


void AI::changeDirection(EDirection direction)
{
    ChangeDirection cmd;
    cmd.direction((ECommandDirection) direction);
    this->sendCommand(&cmd);
}

void AI::activateWallBreaker()
{
    ActivateWallBreaker cmd;
    this->sendCommand(&cmd);
}
