#ifndef CHILLNCODE_H
#define CHILLNCODE_H

#include "ks/models.h"
#include "ks/commands.h"
#include "ai.h"
#include "square.h"
#include <vector>
#include <stack>
#include <iostream>

using namespace koala::chillin::client;
using namespace ks::models;
using namespace ks::commands;

class Chillncode
{
private:
    Agent myAgent;
    std::vector < std::vector <ECell> > boardVar;
    std::vector <Square> squares;
    std::vector < std::vector <int> > entryPositions;
    std::string teamName;
    std::vector < std::vector <int> > reachingPath;
    int reachingPathIndex;
    int mostWeightedSquareIndex;
    int currentSquareIndex;
public:
    void makeSquares();
    void setRequirements(std::vector < std::vector <ECell> > board, Agent &agent, std::string teamStr);
    void findNearestSquares();
    void mostWeightedNearestSquare();
    void findBestRoute();
    void updateCurrentSquareIndex();
    void sayWelcome();
    bool isNewSquare();
    bool isMostWeightedNearestSquareChanged();
    bool isWallbreakerNeeded();
    int findSquareNum(int posY, int posX);
    EDirection nextDirection();
    Chillncode();
private:
    void setEntryPositions();
    void findAllRoutes(std::vector < std::vector < std::vector <int> > > &allRoutes, std::vector <int> &destination);
    bool isReached(std::vector < std::vector <int> > &reachedVertices, int i, int j);
    bool isRouteReachable(std::vector < std::vector <int> > &route);
    int findMostWeightedNearestSquareIndex();
    int findRouteWeight(std::vector < std::vector <int> > &routePos);
public: //read-only functions
    std::vector < std::vector <ECell> > board() const;
};


//implementations
Chillncode::Chillncode()
{
}

inline std::vector < std::vector <ECell> > Chillncode::board() const
{
    return boardVar;
}

void Chillncode::setRequirements(std::vector < std::vector <ECell> > board, Agent &agent, std::string teamStr)
{
    boardVar = board;
    myAgent = agent;
    teamName = teamStr;
}

void Chillncode::makeSquares()
{
    int posY = 1, posX = 1;
    while(posY < board().size() - 1)
    {
        int i = posY, j = posX;
        bool isThreeSides = true, isTwoSides = true, isTwoMaked = false;
        std::vector < std::vector < std::vector <int> > > squarePositions;
        std::vector < std::vector <int> > allPositions;
        std::vector < std::vector <int> > twoSidesSquare = {{0, 0}, {0, 0}};
        std::vector <int> posYSteps = {0, 0, 1, 0};
        std::vector <int> posXSteps = {0, 1, -1, 1};

        for(int k = 0; k < 4; k++)
        {
            posY = posY + posYSteps[k];
            posX = posX + posXSteps[k];
            i = posY, j = posX;
            int count = 0;
            while(count < 4)
            {
                if(i > board().size() - 2 || j > board()[0].size() - 2)
                    break;

                if(board()[i][j] == ECell::AreaWall)
                    isThreeSides = false, isTwoSides = false;
                else
                {
                    bool isExist = false;
                    for(int l = 0; l < allPositions.size(); l++)
                    {
                        if(i == allPositions[l][0] && j == allPositions[l][1])
                        {
                            isExist = true;
                            break;
                        }
                    }
                    if(isExist == false)
                        allPositions.push_back({i, j});
                }

                count++;
                if((i == posY) && (posX <= j) && (j < posX + 1))
                    j++;
                else if((i < posY + 1) && (j == posX + 1))
                    i++;
                else if((i == posY + 1) && (posX < j) && (j <= posX + 1))
                    j--;
            }

            if(isTwoSides == true && allPositions.size() > 3)
            {
                if(isTwoMaked == false)
                {
                    twoSidesSquare[0][0] = posY, twoSidesSquare[0][1] = posX;
                    twoSidesSquare[1][0] = posY + 1, twoSidesSquare[1][1] = posX + 1;

                    isTwoMaked = true;
                }
            }
            if(isTwoMaked == false)
                isTwoSides = true;
        }
        posY--, posX--;

        if(isThreeSides == true && allPositions.size() > 8)
        {
            squarePositions.push_back({{posY, posX}, {posY + 2, posX + 2}});
            allPositions.clear();
            allPositions.shrink_to_fit();
            twoSidesSquare.clear();
            twoSidesSquare.shrink_to_fit();
        }
        else
        {
            if(isTwoMaked == true)
            {
                squarePositions.push_back({{twoSidesSquare[0][0], twoSidesSquare[0][1]}, {twoSidesSquare[1][0], twoSidesSquare[1][1]}});
            }
            for(int k = 0; k < allPositions.size(); k++)
            {
                while((twoSidesSquare[0][0] <= allPositions[k][0] && allPositions[k][0] <= twoSidesSquare[1][0]) &&
                      (twoSidesSquare[0][1] <= allPositions[k][1] && allPositions[k][1] <= twoSidesSquare[1][1]))
                {
                    k++;
                }
                squarePositions.push_back({{allPositions[k][0], allPositions[k][1]}, {allPositions[k][0], allPositions[k][1]}});
            }
        }

        for(int k = 0; k < squarePositions.size(); k++)
        {
            Square sqObject;
            squares.push_back(sqObject);
            int i1 = squarePositions[k][0][0], j1 = squarePositions[k][0][1];
            int i2 = squarePositions[k][1][0], j2 = squarePositions[k][1][1];

            std::shared_ptr< std::vector < std::vector<int> > > tempVector (new std::vector < std::vector<int> > {{i1, j1}, {i2, j2}} );
            squares.back().setPosition(tempVector);
        }

        posX += 3;
        if(posX > board()[0].size() - 2)
        {
            posX = 1;
            posY += 3;
        }
    }

    currentSquareIndex = findSquareNum(myAgent.position().y(), myAgent.position().x());
}

bool Chillncode::isReached(std::vector < std::vector <int> > &reachedVertices, int i, int j)
{
    bool isReached = false;
    for(int k = 0; k < reachedVertices.size(); k++)
    {
        if(reachedVertices[k][0] == i && reachedVertices[k][1] == j)
        {
            isReached = true;
            break;
        }
    }
    if(isReached == true)
        return true;
    else
        return false;
}

void Chillncode::findAllRoutes(std::vector < std::vector < std::vector <int> > > &allRoutes, std::vector <int> &destination)
{
    std::vector <int> agentPos = { myAgent.position().y(), myAgent.position().x() };
    std::map < std::vector <int>, std::vector < std::vector <int> > > vertices;
    std::vector < std::vector <int> > currentSquarePos = squares[currentSquareIndex].position();

    for (int i = currentSquarePos[0][0]; i <= currentSquarePos[1][0]; i++)
    {
        for (int j = currentSquarePos[0][1]; j <= currentSquarePos[1][1]; j++)
        {
            vertices.insert(std::pair < std::vector <int>, std::vector < std::vector <int> > > ({i,j}, {} ));
        }
    }

    std::vector < std::vector <int> > mainRoute;
    std::stack < std::vector <int> > stack;
    int i, j;
    i = agentPos[0], j = agentPos[1];
    stack.push({ i, j });
    mainRoute.push_back({ i, j });
    do
    {
        std::vector <int> rowSteps = { -1, 1, 0, 0 };
        std::vector <int> columnSteps = { 0, 0, 1, -1 };
        std::vector <int> currentVertex = stack.top();
        stack.pop();
        i = currentVertex[0], j = currentVertex[1];
        int k = 0;
        while(k < 4)
        {
            int ii = i + rowSteps[k];
            int jj = j + columnSteps[k];
            k++;
            if (ii > currentSquarePos[1][0] || jj > currentSquarePos[1][1] || ii < currentSquarePos[0][0] || jj < currentSquarePos[0][1])
            {
                if (ii != destination[0] || jj != destination[1])
                    continue;
            }

            bool reached = isReached(mainRoute, ii, jj);
            if (reached == false)
            {
                std::map < std::vector <int>, std::vector < std::vector <int> > >::iterator itr = vertices.find({ i, j });
                std::vector < std::vector <int> > reachedNeighbors = itr->second;
                reached = isReached(reachedNeighbors, ii, jj);
                if (reached == false)
                {
                    if (ii == destination[0] && jj == destination[1])
                    {
                        auto routeCopy = mainRoute;
                        routeCopy.push_back({ ii, jj });
                        allRoutes.push_back(routeCopy);
                    }
                    else
                    {
                        stack.push({ i, j });
                        mainRoute.push_back({ ii, jj });
                        i = ii, j = jj;
                        k = 0;
                    }
                    itr->second.push_back({ ii, jj });
                    continue;
                }
            }
        }
        std::map < std::vector <int>, std::vector < std::vector <int> > >::iterator itr = vertices.find({ i, j });
        itr->second.clear();
        mainRoute.pop_back();
    } while (!stack.empty());
}

void Chillncode::setEntryPositions()
{
    entryPositions.clear();
    std::vector < std::vector <int> > currentSquarePos = squares[currentSquareIndex].position();
    std::vector < std::vector <int> > mostWeightedSquarePos = squares[mostWeightedSquareIndex].position();
    int sides = currentSquarePos[1][0] - currentSquarePos[0][0] + 1;
    int i = currentSquarePos[0][0], j = currentSquarePos[0][1];
    bool breakFor = false;

    int n = 0;
    std::vector <int> iSteps = {-1, n, sides, n};
    std::vector <int> jSteps = {n, sides, n, -1};

    for(int l = 0; l < sides; l++)
    {
        for(int k = 0; k < 4; k++)
        {
            i = currentSquarePos[0][0], j = currentSquarePos[0][1];
            i += iSteps[k], j += jSteps[k];
            if(board()[i][j] == ECell::AreaWall)
                continue;
            while((mostWeightedSquarePos[0][0] <= i && i <= mostWeightedSquarePos[1][0]) &&
                  (mostWeightedSquarePos[0][1] <= j && j <= mostWeightedSquarePos[1][1]))
            {
                entryPositions.push_back({i, j});
                if(iSteps[k] == n)
                    i++;
                if(jSteps[k] == n)
                    j++;
                breakFor = true;
            }
            if(breakFor == true)
                break;
        }
        if(breakFor == true)
            break;
        n++;
        iSteps = {-1, n, sides, n};
        jSteps = {n, sides, n, -1};
    }
}

void Chillncode::findBestRoute()
{
    int maxWeight = 0;
    std::vector < std::vector < std::vector <int> > > mostWeightedRoutes;
    for(int i = 0; i < entryPositions.size(); i++)
    {
        std::vector < std::vector < std::vector <int> > > allRoutes;
        findAllRoutes(allRoutes, entryPositions[i]);
        std::vector < std::vector <int> > maxRoute;
        for(int j = 0; j < allRoutes.size(); j++)
        {
            int weight = findRouteWeight(allRoutes[j]);
            if(weight > maxWeight)
            {
                bool isReachable = isRouteReachable(allRoutes[j]);
                if(isReachable == true)
                {
                    maxWeight = weight;
                    maxRoute = allRoutes[j];
                }
            }
        }
        if(allRoutes.size() != 0)
            mostWeightedRoutes.push_back(maxRoute);
    }

    maxWeight = 0;
    for(std::vector < std::vector <int> > route : mostWeightedRoutes)
    {
        int weight = findRouteWeight(route);
        if(weight > maxWeight)
        {
            maxWeight = weight;
            reachingPath = route;
        }
    }

    reachingPathIndex = 1;
}

bool Chillncode::isRouteReachable(std::vector < std::vector <int> > &route)
{
    int reachedWalls = 0;
    for(std::vector <int> pos : route)
        if(board()[pos[0]][pos[1]] == ECell::BlueWall || board()[pos[0]][pos[1]] == ECell::YellowWall)
            reachedWalls++;


    if(myAgent.wallBreakerCooldown() == 0)
    {
        if(reachedWalls <= myAgent.wallBreakerRemTime())
            return true;
        else
            return false;
    }
    else
    {
        int health = myAgent.health();
        if(reachedWalls < health)
            return true;
        else
            return false;
    }

}

int Chillncode::findRouteWeight(std::vector < std::vector <int> > &routePos)
{
    auto myWall = ECell::Empty;
    if(myAgent.name() == "Blue")
        myWall = ECell::BlueWall;
    else
        myWall = ECell::YellowWall;
    int emptyWalls = 0, myWalls = 0, enemyWalls = 0;
    for(int i = 0; i < routePos.size(); i++)
    {
        int posI = routePos[i][0], posJ = routePos[i][1];
        if(board()[posI][posJ] == ECell::Empty)
            emptyWalls++;
        else if(board()[posI][posJ] == myWall)
            myWalls++;
        else
            enemyWalls++;
    }

    int weight = (emptyWalls * 1) + (myWalls * -1) + (enemyWalls * 0.6);
    return weight;
}

void Chillncode::updateCurrentSquareIndex()
{
    auto nearestSquaresNums = squares[currentSquareIndex].nearestSquares();
    std::vector <int> agentPos = {myAgent.position().y(), myAgent.position().x()};
    for(int i = 0; i < nearestSquaresNums.size(); i++)
    {
        std::vector < std::vector <int> > nearestSquarePos = squares[nearestSquaresNums[i]].position();
        if((nearestSquarePos[0][0] <= agentPos[0] && agentPos[0] <= nearestSquarePos[1][0]) &&
                (nearestSquarePos[0][1] <= agentPos[1] && agentPos[1] <= nearestSquarePos[1][1]))
        {
            currentSquareIndex = nearestSquaresNums[i];
            break;
        }
    }
}

bool Chillncode::isNewSquare()
{
    std::vector < std::vector <int> > currentSquarePos = squares[currentSquareIndex].position();
    std::vector <int> agentPos = {myAgent.position().y(), myAgent.position().x()};
    if((currentSquarePos[0][0] > agentPos[0] || agentPos[0] > currentSquarePos[1][0]) ||
            (currentSquarePos[0][1] > agentPos[1] || agentPos[1] > currentSquarePos[1][1]))
        return true;
    else
        return false;
}

int Chillncode::findMostWeightedNearestSquareIndex()
{
    auto myWall = ECell::Empty;

    if(teamName == "Blue")
        myWall = ECell::BlueWall;
    else
        myWall = ECell::YellowWall;

    std::vector <int> nearestSquares = squares[currentSquareIndex].nearestSquares();
    std::vector <int> weights;
    for(int k = 0; k < nearestSquares.size(); k++)
    {
        int myWallsNum = 0, enemyWallsNum = 0, emptyWallsNum = 0;
        int squareNum = nearestSquares[k];
        std::vector < std::vector <int> > squarePos = squares[squareNum].position();
        for(int i = squarePos[0][0]; i <= squarePos[1][0]; i++)
        {
            for(int j = squarePos[0][1]; j <= squarePos[1][1]; j++)
            {
                if(board()[i][j] == ECell::Empty)
                    emptyWallsNum++;
                else if(board()[i][j] == myWall)
                    myWallsNum++;
                else
                    enemyWallsNum++;
            }
        }
        int weight = (emptyWallsNum * 1) + (myWallsNum * (-1)) + (enemyWallsNum * 0.6);
        weights.push_back(weight);
    }

    auto currentSquarePos = squares[currentSquareIndex].position();
    int verticalWeight = -1000, verticalIndex;
    for(int i = 0; i < nearestSquares.size(); i++)
    {
        auto nearSquarePos = squares[nearestSquares[i]].position();
        int newRow;
        for(int j = 0; j < 2; j++)
        {
            if(j == 0)
                newRow = currentSquarePos[0][0] - 1;
            else
                newRow = currentSquarePos[1][0] + 1;

            if(nearSquarePos[0][0] <= newRow && newRow <= nearSquarePos[1][0])
            {
                if(weights[i] > verticalWeight)
                {
                    verticalWeight = weights[i];
                    verticalIndex = nearestSquares[i];
                }
                break;
            }
        }
    }

    int horizontalWeight = -1000, horizontalIndex;
    for(int i = 0; i < nearestSquares.size(); i++)
    {
        auto nearSquarePos = squares[nearestSquares[i]].position();
        int newColumn;
        if(teamName == "Blue")
            newColumn = currentSquarePos[1][1] + 1;
        else
            newColumn = currentSquarePos[0][1] - 1;
        if(nearSquarePos[0][1] <= newColumn && newColumn <= nearSquarePos[1][1])
        {
            horizontalWeight = weights[i];
            horizontalIndex = nearestSquares[i];
            break;
        }
    }

    if(verticalWeight < horizontalWeight)
    {
        return horizontalIndex;
    }
    else
    {
        return verticalIndex;
    }
}

int Chillncode::findSquareNum(int posY, int posX)
{
    int squareNum = 0;
    int size = squares.size();
    for(int i = 0; i < size; i++)
    {
        auto squarePos = squares[i].position();
        if((squarePos[0][0] <= posY && posY <= squarePos[1][0])
                && (squarePos[0][1] <= posX && posX <= squarePos[1][1]))
        {
            break;
        }
        squareNum++;
    }
    return squareNum;
}

void Chillncode::mostWeightedNearestSquare()
{
    mostWeightedSquareIndex = findMostWeightedNearestSquareIndex();
    setEntryPositions();
}

void Chillncode::findNearestSquares()
{
    for(int l = 0; l < squares.size(); l++)
    {
        auto squarePos = squares[l].position();
        std::vector <int> nearestSquares;
        int sides = squarePos[1][0] - squarePos[0][0] + 1;
        int i = squarePos[0][0], j = squarePos[0][1];

        int n = 0;
        std::vector <int> iSteps = {-1, n, sides, n};
        std::vector <int> jSteps = {n, sides, n, -1};

        for(int m = 0; m < sides; m++)
        {
            for(int k = 0; k < 4; k++)
            {
                i = squarePos[0][0], j = squarePos[0][1];
                i += iSteps[k], j += jSteps[k];

                if(board()[i][j] == ECell::AreaWall)
                    continue;

                bool isExist = false;
                for(int p = 0; p < nearestSquares.size(); p++)
                {
                    std::vector < std::vector <int> > nearestSquarePos = squares[nearestSquares[p]].position();
                    if((nearestSquarePos[0][0] <= i && i <= nearestSquarePos[1][0]) &&
                       (nearestSquarePos[0][1] <= j && j <= nearestSquarePos[1][1]))
                    {
                        isExist = true;
                        break;
                    }
                }
                if(isExist == true)
                    continue;

                int number = findSquareNum(i, j);
                nearestSquares.push_back(number);
            }
            n++;
            iSteps = {-1, n, sides, n};
            jSteps = {n, sides, n, -1};
        }
        std::shared_ptr < std::vector <int> > nearestSquaresPtr(new std::vector <int> (nearestSquares));
        squares[l].setNearestSquares(nearestSquaresPtr);
    }
}

bool Chillncode::isMostWeightedNearestSquareChanged()
{
    int squareIndex = findMostWeightedNearestSquareIndex();
    if(squareIndex != mostWeightedSquareIndex)
        return true;
    else
        return false;
}

bool Chillncode::isWallbreakerNeeded()
{
    int i = reachingPath[reachingPathIndex][0], j = reachingPath[reachingPathIndex][1];

    if(board()[i][j] != ECell::Empty)
    {
        return true;
    }
    else
        return false;
}

void Chillncode::sayWelcome()
{
    std::vector <std::string> chillncode = {
        {'\n'},
        {"   ==============================================================================="},
        {"   ==============================================================================="},
        {"   =====     ===  =========  ==  ==  =========  ====     =============  =========="},
        {"   ====  ===  ==  =========  ==  === ========== ===  ===  ============  =========="},
        {"   ===  ========  =========  ==  === ========== ==  ==================  =========="},
        {"   ===  ========  =====  ==  ==  ======  = =======  =========   ======  ===   ===="},
        {"   ===  ========    =======  ==  ======     ======  ========     ===    ==  =  ==="},
        {"   ===  ========  =  ==  ==  ==  ======  =  ======  ========  =  ==  =  ==     ==="},
        {"   ===  ========  =  ==  ==  ==  ======  =  ======  ========  =  ==  =  ==  ======"},
        {"   ====  ===  ==  =  ==  ==  ==  ======  =  =======  ===  ==  =  ==  =  ==  =  ==="},
        {"   =====     ===  =  ==  ==  ==  ======  =  ========     ====   ====    ===   ===="},
        {"   ==============================================================================="},
        {"   ==============================================================================="},
        {'\n'}
    };
    for(int i = 0; i < chillncode.size(); i++)
        std::cout << chillncode[i] << "\n";
}

EDirection Chillncode::nextDirection()
{
    std::vector <int> agentPos = {myAgent.position().y(), myAgent.position().x()};
    auto nextDir = EDirection::Right;
    if(agentPos[0] == reachingPath[reachingPathIndex][0])
    {
        if(reachingPath[reachingPathIndex][1] - agentPos[1] == 1)
            nextDir = EDirection::Right;
        else
            nextDir = EDirection::Left;
    }
    else if(agentPos[1] == reachingPath[reachingPathIndex][1])
    {
        if(agentPos[0] - reachingPath[reachingPathIndex][0] == 1)
            nextDir = EDirection::Up;
        else
            nextDir = EDirection::Down;
    }
    reachingPathIndex++;
    return nextDir;
}


#endif //CHILLNCODE_H
