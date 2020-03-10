#include "ai.h"
#include "chillncode.h"

#include <iostream>
#include <vector>
#include <map>

#include "effolkronium/random.hpp"

using namespace std;
using Random = effolkronium::random_static;

using namespace koala::chillin::client;
using namespace ks::models;
using namespace ks::commands;

vector < vector < vector<int> > > squarePositions;
int squareWeights[squarePositions.size()];

AI::AI(World *world): RealtimeAI<World*>(world)
{
}

AI::~AI()
{
}

void AI::initialize()
{
    auto board = this->world->board();
    chillncode obj;
    obj.makeSquares(board, squarePositions);
}

void AI::decide()
{

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
