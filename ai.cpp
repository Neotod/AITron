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
std::vector < std::vector <int> > reachingPath;
std::vector <int> lastPosition;

Chillncode chillncode;

int currentSquareNum;

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

    chillncode.setRequirements(&board, &squares, agent, this->mySide);
    chillncode.makeSquares(squares);
    chillncode.findNearestSquares(nearestSquares);
    currentSquareNum = chillncode.findSquareNum(agent.position().y(), agent.position().x());
    chillncode.findBestRoute(reachingPath, lastPosition);
    lastPosition = {agent.position().y(), agent.position().x()};
}

void AI::decide()
{
    bool isNewSquare = chillncode.isNewSquare(currentSquareNum);
    if(isNewSquare == true)
    {
           chillncode.findNearestSquares(nearestSquares);
    }
    chillncode.mostWeightedNearestSquare(nearestSquares);
    chillncode.findBestRoute(reachingPath, lastPosition);

    Agent agent = this->world->agents()[this->mySide];
    lastPosition = {agent.position().y(), agent.position().x()};

    if(agent.wallBreakerCooldown() == 0)
    {
        vector < vector <ECell> > board = this->world->board();
        auto enemyWall = ECell::Empty;
        if(this->mySide == "Blue")
            enemyWall = ECell::YellowWall;
        else
            enemyWall = ECell::BlueWall;
        if(board[reachingPath[1][0]][reachingPath[1][1]] == enemyWall)
            this->activateWallBreaker();
    }

    EDirection nextDirection = chillncode.nextDirection(reachingPath[1]);
    changeDirection(nextDirection);
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
