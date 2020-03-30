#include <vector>

#include "ai.h"
#include "effolkronium/random.hpp"
#include "square.h"
#include "chillncode.h"

using namespace std;
using Random = effolkronium::random_static;

using namespace koala::chillin::client;
using namespace ks::models;
using namespace ks::commands;

Chillncode chillncode;

std::vector <int> agentLastPos;

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

    chillncode.setRequirements(board, agent, this->mySide);
    chillncode.makeSquares();
    chillncode.findNearestSquares();
    agentLastPos = {agent.position().y(), agent.position().x()};
    chillncode.mostWeightedNearestSquare();
    chillncode.findBestRoute(agentLastPos);
}

void AI::decide()
{
    vector < vector <ECell> > board = this->world->board();
    auto agent = this->world->agents()[this->mySide];

    chillncode.setRequirements(board, agent, this->mySide);
    agentLastPos = {agent.position().y(), agent.position().x()};

    bool isNewSquare = chillncode.isNewSquare();
    if(isNewSquare == true)
    {
        chillncode.updateCurrentSquareIndex();
        chillncode.mostWeightedNearestSquare();
        chillncode.findBestRoute(agentLastPos);
    }

    bool isChanged = chillncode.isMostWeightedNearestSquareChanged();
    if(isChanged == true)
    {
        chillncode.mostWeightedNearestSquare();
        chillncode.findBestRoute(agentLastPos);
    }

    if(agent.wallBreakerCooldown() == 0)
    {
        bool isWallbreakerNeeded = chillncode.isWallbreakerNeeded();
        if(isWallbreakerNeeded == true)
        {
            activateWallBreaker();
        }
    }

    auto nextDir = chillncode.nextDirection();
    changeDirection(nextDir);
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
