#ifndef CHILLNCODE_H
#define CHILLNCODE_H


#include "ks/models.h"
#include "ks/commands.h"
#include "ai.h"

using namespace koala::chillin::client;
using namespace ks::models;
using namespace ks::commands;

#include <vector>
#include <fstream>
#include <iostream>

class chillncode
{
public:
    std::vector < std::vector < std::vector <int> > > squarePositions() const;
    int findSides(std::vector < std::vector<ECell> > &, int, int);
    void makeSquares(std::vector < std::vector < std::vector <int> > > &positions);
    void findBestRoute(int startPos[], int endPos[], std::vector < std::vector <int> > &positions);
    int giveRouteLength(int startPos[], int endPos[]);
    int findSquareWeight(std::vector < std::vector <ECell> > &board,int agentPos[], int squarePos[][2],
                         AI *ai, std::string teamName);
    int getRouteLength(int startPos[], int endPos[]);
    void findNearestSquarePos(int agentPos[], int squarePos[][2], int nearestPos[]);

private:
    std::vector < std::vector < std::vector <int> > > ــsquarePositions;
};

inline std::vector < std::vector < std::vector <int> > > chillncode::squarePositions() const
{
    return ــsquarePositions;
}

int chillncode::findSides(std::vector<std::vector<ECell> > &board, int posY, int posX)
{
    int i = posY, j = posX;
    int counter = 1;

    if(posY < board.size()-3)
    {
        while(board[i][j] != ECell::AreaWall)
        {
            counter++;
            if((i == posY) && (posX <= j) && (j < posX+2))
            {
                j++;
            }
            else if((i < posY+2) && (j == posX+2))
            {
                i++;
            }
            else if((i == posY+2) && (posX < j) && (j <= posX+2))
            {
                j--;
            }
            else if((posY < i) && (i <= posY+2) && (j == posX))
            {
                i--;
            }
            if(counter == 9)
            {
                if(board[posY+1][posX+1] != ECell::AreaWall)
                    return 3;
                else
                    break;
            }
        }
    }
    if(posY < board.size()-2)
    {
        counter = 1;
        i = posY, j = posX;
        while(board[i][j] != ECell::AreaWall)
        {
            counter++;
            if((i == posY) && (posX <= j) && (j < posX+1))
            {
                j++;
            }
            else if((i < posY+1) && (j == posX+1))
            {
                i++;
            }
            else if((i == posY+1) && (posX < j) && (j <= posX+1))
            {
                j--;
            }
            else if((posY < i) && (i <= posY+1) && (j == posX))
            {
                i--;
            }
            if(counter == 5)
                return 2;
        }
    }
    return 1;
}

void chillncode::makeSquares(std::vector < std::vector < std::vector <int> > > &positions)
{
    int i = 1, j = 1;
    while(i < board.size() - 1)
    {
        if(board[i][j] != ECell::AreaWall)
        {
            int sides = findSides(board, i, j);

            std::vector < std::vector<int> > tempVector = {{i, j}, {i + (sides - 1), j + (sides - 1)}};
            positions.push_back(tempVector);

            if(sides == 3)
            {
                j += 3;
            }
            if(sides == 2)
            {
                if(i < board.size()-3)
                {
                    int storeI = i;
                    i += 2;
                    for(int k = 0; k < 2; k++)
                    {
                        if(board[i][j] != ECell::AreaWall)
                        {
                            std::vector < std::vector<int> > tempVector = {{i, j}, {i, j}};
                            positions.push_back(tempVector);
                        }
                        j++;
                    }
                    i = storeI;
                }
                else
                    j+=2;
            }
            if(sides == 1)
            {
                if(i < board.size()-2)
                {
                    int storeI = i;
                    i++;
                    for(int k = 0; k < 2; k++)
                    {
                        if(board[i][j] != ECell::AreaWall)
                        {
                            std::vector < std::vector<int> > tempVector = {{i, j}, {i, j}};
                            positions.push_back(tempVector);
                        }
                        i++;
                    }
                    i = storeI;
                    j++;
                }
                else
                {
                    j++;
                }
            }
        }
        else
            j++;
        if(j == board[0].size() - 1)
        {
            j = 1;
            i += 3;
        }
    }
}

void chillncode::findBestRoute(int startPos[], int endPos[], std::vector < std::vector <int> > &positions)
{

}

int chillncode::getRouteLength(int startPos[], int endPos[])
{

}

void chillncode::findNearestSquarePos(int agentPos[], int squarePos[][2], int nearestPos[])
{
    int sides = squarePos[0][0] - squarePos[1][0] + 1;

    if(sides != 1)
    {
        int minDistanceX = 0, minDistanceY = 0;
        unsigned int distanceX = 0, distanceY = 0;
        int i = squarePos[0][0], j = squarePos[0][1];
        for(int k = 0; k < sides*sides; k++)
        {
            distanceY = i - agentPos[0];
            distanceX = j - agentPos[1];
            if((distanceY < minDistanceY) && (distanceX < minDistanceX))
            {
                minDistanceY = distanceY;
                nearestPos[0] = i;
                minDistanceX = distanceX;
                nearestPos[1] = j;
            }

            if((i == squarePos[0][0]) && (squarePos[0][1] <= j) && (j < squarePos[1][1]))
                j++;
            else if((squarePos[0][0] <= i) && (i < squarePos[1][0]) && (j == squarePos[1][1]))
                i++;
            else if((i == squarePos[1][0]) && (squarePos[0][1] < j) && (j <= squarePos[1][1]))
                j--;
            else if((squarePos[0][0] < i) && (i <= squarePos[1][0]) && (j == squarePos[0][0]))
                i--;
        }
    }
    else
    {
        nearestPos[0] = squarePos[0][0];
        nearestPos[1] = squarePos[0][1];
    }
}

int chillncode::findSquareWeight(std::vector < std::vector <ECell> > &board, int agentPos[], int squarePos[][2],
                                 AI *ai, std::string teamName)
{
    //    int agentPos[] = {ai->world->agents()[ai->mySide].position().y(), ai->world->agents()[ai->mySide].position().x()};
    auto myWall = ECell::Empty;
    if(teamName == "Blue")
        myWall = ECell::BlueWall;
    else
        myWall = ECell::YellowWall;

    int emptyWalls = 0, enemyWalls = 0, myWalls = 0;
    for(int i = squarePos[0][0]; i <= squarePos[1][0]; i++)
    {
        for(int j = squarePos[0][1]; j <= squarePos[1][1]; j++)
        {
            if(board[i][j] == ECell::Empty)
                emptyWalls++;
            else if(board[i][j] == myWall)
                myWalls++;
            else
                enemyWalls++;
        }
    }

    int nearestPos[2];
    this->findNearestSquarePos(agentPos, squarePos, nearestPos);

}

#endif // CHILLNCODE_H


