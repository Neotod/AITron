#ifndef CHILLNCODE_H
#define CHILLNCODE_H

#include "ks/models.h"
#include "ks/commands.h"
#include "ai.h"
#include "square.h"
#include <vector>
#include <memory>
#include <queue>
#include <complex>
#include <algorithm>

using namespace koala::chillin::client;
using namespace ks::models;
using namespace ks::commands;

class Chillncode
{
private:
    std::shared_ptr < std::vector < std::vector <ECell> > > boardPtr;
    Agent myAgent;
    std::shared_ptr < std::vector <Square> > squaresPtr;
    std::vector < std::vector <int> > entryPositions;
    std::string teamName;
    int mostWeightedSquareNum;
    int currentSquareNum;
public:
    void makeSquares(std::vector <Square> &squares);
    void setRequirements(std::vector < std::vector <ECell> > *board, std::vector <Square> *squares,
                         Agent agent, std::string teamStr);
    void findNearestSquares(std::vector <int> &nearestSquares);
    void mostWeightedNearestSquare(std::vector <int> &nearestSquares);
    int findSquareNum(int posY, int posX);
    bool isNewSquare(int current);
    void findBestRoute(std::vector < std::vector <int> > &finalRoute, std::vector <int> &lastPos);
    EDirection nextDirection(std::vector <int> &nextPos);
    Chillncode();
private:
    bool isReached(std::vector < std::vector <int> > &reachedVertices, int i, int j);
    void setEntryPositions();
    void findAllRoutes(std::vector < std::vector < std::vector <int> > > &allRoutes, std::vector <int> &destination, std::vector <int> &lastPos);
    bool isRouteReachable(std::vector < std::vector <int> > &route);
    void findMostWeightedNearestSquare(std::vector <int> &nearestSquares);
    int findRouteWeight(std::vector < std::vector <int> > &routePos);
public: //read-only functions
    std::vector <Square> squares() const;
    std::vector < std::vector <ECell> > board() const;
};


Chillncode::Chillncode()
{
}

inline std::vector <Square> Chillncode::squares() const
{
    return *squaresPtr;
}

inline std::vector < std::vector <ECell> > Chillncode::board() const
{
    return *boardPtr;
}

void Chillncode::setRequirements(std::vector < std::vector <ECell> > *board, std::vector <Square> *squares,
                                 Agent agent, std::string teamStr)
{
    std::shared_ptr < std::vector < std::vector <ECell> > > boardSharedPtr(board);
    boardPtr = boardSharedPtr;
    std::shared_ptr < std::vector <Square> > squaresSharedPtr(squares);
    squaresPtr = squaresSharedPtr;
    myAgent = agent;
    teamName = teamStr;
    currentSquareNum = findSquareNum(myAgent.position().y(), myAgent.position().x());
}

void Chillncode::makeSquares(std::vector <Square> &squares)
{
    int posY = 1, posX = 1;
    while(posY < board().size() - 1)
    {
        int i = posY, j = posX, itr = 0;
        bool isThreeSides = true, isTwoSides = true, isTwoMaked = false;
        std::vector < std::vector < std::vector <int> > > squarePositions;
        std::vector < std::vector <int> > allPositions;
        std::vector < std::vector <int> > twoSidesSquare = {{0, 0}, {0, 0}};
        std::vector <int> posYSteps = {0, 0, 1, 1};
        std::vector <int> posXSteps = {0, 1, -1, 1};

        for(int k = 0; k < 4; k++)
        {
            posY = posY + posYSteps[itr];
            posX = posX + posXSteps[itr];
            itr++;
            int count = 1;
            while(count < 4)
            {
                count++;
                if((i == posY) && (posX <= j) && (j < posX + 1))
                    j++;
                else if((i < posY + 1) && (j == posX + 1))
                    i++;
                else if((i == posY + 1) && (posX < j) && (j <= posX + 1))
                    j--;

                if(i < 1 || j < 1 || i > board().size() - 2 || j > board()[0].size() - 2)
                    continue;
                if(board()[i][j] == ECell::AreaWall)
                {
                    isThreeSides = false, isTwoSides = false;
                    continue;
                }

                bool isExist = false;
                for(int l = 0; l < allPositions.size(); l++)
                {
                    if(i == allPositions[l][0] && j == allPositions[l][1])
                        isExist = true;
                }
                if(isExist == false)
                    allPositions.push_back({i, j});
            }

            if(isTwoSides == true)
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

        if(isThreeSides == true)
        {
            squarePositions.push_back({{posY, posX}, {posY + 2, posX + 2}});
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

        posY += 3, posX += 3;
        if(posX > board()[0].size() - 2)
            posX = 1;
    }
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

void Chillncode::findAllRoutes(std::vector < std::vector < std::vector <int> > > &allRoutes, std::vector <int> &destination, std::vector <int> &lastPos)
{
    std::vector <int> agentPos = {myAgent.position().y(), myAgent.position().x()};
    std::vector < std::vector <int> > reachedVertices;
    std::queue < std::vector <int> > queue;
    std::vector < std::vector <int> > currentSquarePos = squares()[currentSquareNum].position();
    int i, j;
    queue.push(agentPos);
    allRoutes.push_back({{agentPos[0], agentPos[1]}});
    while(queue.size() != 0)
    {
        std::vector <int> currentPos = queue.front();
        queue.pop();
        i = currentPos[0], j = currentPos[1];
        reachedVertices.push_back({i, j});
        std::vector <int> rowSteps = {1, -1, 0, 0};
        std::vector <int> columnSteps = {0, 0, 1, -1};
        std::vector < std::vector <int> > neighbors;
        for(int k = 0; k < 4; k++)
        {
            int ii = i + rowSteps[k];
            int jj = j + columnSteps[k];
            if((ii > currentSquarePos[1][0] || jj > currentSquarePos[1][1] || ii < currentSquarePos[0][0] || jj < currentSquarePos[0][1])
                    || (ii != destination[0] && jj != destination[1]))
                continue;
            if((i == currentPos[0] && j == currentPos[1]) && (ii == lastPos[0] && jj == lastPos[1]))
                continue;
            neighbors.push_back({ii, jj});
            bool reached = isReached(reachedVertices, ii, jj);
            if(reached == false)
            {
                queue.push({ii, jj});
            }
        }
        for(int m = 0; m < allRoutes.size(); m++)
        {
           if(allRoutes[m].back()[0] == i, allRoutes[m].back()[1] == j)
           {
               std::vector < std::vector <int> > routeCopy = allRoutes[m];
               std::vector < std::vector < std::vector <int> > >::iterator it = allRoutes.begin();
               allRoutes.erase(it + m);
               for(int l = 0; l < neighbors.size(); l++)
               {
                   bool reached = false;
                   reached = isReached(allRoutes[m], neighbors[l][0], neighbors[l][1]);
                   if(reached = false)
                   {
                        std::vector < std::vector <int> > temp = routeCopy;
                        temp.push_back(neighbors[l]);
                        allRoutes.push_back(temp);
                   }
               }
           }
        }
    }
}

void Chillncode::setEntryPositions()
{
    entryPositions.clear();
    auto currentSquarePos = squares()[currentSquareNum].position();
    auto mostWeightedSquarePos = squares()[mostWeightedSquareNum].position();
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
            i += iSteps[k], j += jSteps[k];
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
        }
        if(breakFor == true)
            break;
        n++;
        i = currentSquarePos[0][0], j = currentSquarePos[0][1];
    }
}

void Chillncode::findBestRoute(std::vector < std::vector <int> > &finalRoute, std::vector <int> &lastPos)
{
    int maxWeight = 0;
    std::vector < std::vector < std::vector <int> > > mostWeightedRoutes;
    for(int i = 0; i < entryPositions.size(); i++)
    {
        std::vector < std::vector < std::vector <int> > > allRoutes;
        findAllRoutes(allRoutes, entryPositions[i], lastPos);
        std::vector < std::vector <int> > maxRoute;
        for(int j = 0; j < allRoutes.size(); j++)
        {
            if(allRoutes[j].back() == entryPositions[i])
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
        }
        mostWeightedRoutes.push_back(maxRoute);
    }

    maxWeight = 0;
    for(std::vector < std::vector <int> > route : mostWeightedRoutes)
    {
        int weight = findRouteWeight(route);
        if(weight > maxWeight)
        {
            maxWeight = weight;
            finalRoute = route;
        }
    }
}

bool Chillncode::isRouteReachable(std::vector < std::vector <int> > &route)
{
    auto myWall = ECell::Empty;
    if(myAgent.name() == "Blue")
        myWall = ECell::BlueWall;
    else
        myWall = ECell::YellowWall;

    int reachedWalls = 0;
    for(std::vector <int> pos : route)
        if(board()[pos[0]][pos[1]] == ECell::BlueWall || board()[pos[0]][pos[1]] == ECell::YellowWall)
            reachedWalls++;


    if(myAgent.wallBreakerCooldown() == 0)
    {
        if(reachedWalls < myAgent.wallBreakerRemTime())
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

bool Chillncode::isNewSquare(int current)
{
    if(current != currentSquareNum)
        return true;
    else
        return false;
}

void Chillncode::findMostWeightedNearestSquare(std::vector <int> &nearestSquares)
{
    auto myWall = ECell::Empty;

    if(teamName == "Blue")
        myWall = ECell::BlueWall;
    else
        myWall = ECell::YellowWall;

    int myWallsNum = 0, enemyWallsNum = 0, emptyWallsNum;
    for(int k = 0; k < nearestSquares.size(); k++)
    {
        std::vector < std::vector <int> > squarePos = squares()[nearestSquares[k]].position();
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
        int weight = (emptyWallsNum * 1) + (myWallsNum * -1) + (enemyWallsNum * 0.6);
        squares()[k].setWeight(weight);
    }

    int maxWeight = 0, index;
    for(int i = 0; i < nearestSquares.size(); i++)
    {
        int temp = squares()[nearestSquares[i]].getWeight();
        if(temp > maxWeight)
        {
            maxWeight = temp;
            index = i;
        }
    }
    mostWeightedSquareNum = index;
}

int Chillncode::findSquareNum(int posY, int posX)
{
    int squareNum = 0;
    for(int i = 0; i < squares().size(); i++)
    {
        auto squarePos = squares()[i].position();
        if((squarePos[0][0] <= posY && posY <= squarePos[1][0])
                && (squarePos[0][1] <= posX && posX <= squarePos[1][1]))
        {
            break;
        }
        squareNum++;
    }
    return squareNum;
}

void Chillncode::mostWeightedNearestSquare(std::vector <int> &nearestSquares)
{
    findMostWeightedNearestSquare(nearestSquares);
    setEntryPositions();
}

void Chillncode::findNearestSquares(std::vector <int> &nearestSquares)
{
    auto squarePos = squares()[currentSquareNum].position();
    int sides = squarePos[1][0] - squarePos[0][0] + 1;
    int i = squarePos[0][0], j = squarePos[0][1];
    bool breakFor = false;

    int n = 0;
    std::vector <int> iSteps = {-1, n, sides, n};
    std::vector <int> jSteps = {n, sides, n, -1};

    for(int l = 0; l < sides; l++)
    {
        for(int k = 0; k < 4; k++)
        {
            i += iSteps[k], j += jSteps[k];
            int number = findSquareNum(i, j);
            bool isExist = false;
            for(int k = 0; k < nearestSquares.size(); k++)
                if(nearestSquares[k] == number)
                {
                    isExist = true;
                    break;
                }
            if(isExist == false)
                nearestSquares.push_back(number);
        }
        if(breakFor == true)
            break;
        n++;
        i = squarePos[0][0], j = squarePos[0][1];
    }
}

EDirection Chillncode::nextDirection(std::vector <int> &nextPos)
{
    std::vector <int> agentPos = {myAgent.position().y(), myAgent.position().x()};
    if(agentPos[0] == nextPos[0])
    {
        if(nextPos[1] - agentPos[1] == 1)
            return EDirection::Right;
        else
            return EDirection::Left;
    }
    else if(agentPos[1] == nextPos[1])
    {
        if(nextPos[0] - agentPos[0] == 1)
            return EDirection::Up;
        else
            return EDirection::Down;
    }
}

#endif // CHILLNCODE_H
