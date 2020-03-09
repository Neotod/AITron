#ifndef CHILLNCODE_H
#define CHILLNCODE_H

#endif // CHILLNCODE_H

#include "ks/models.h"
#include "ks/commands.h"
#include "ai.h"

using namespace koala::chillin::client;
using namespace ks::models;
using namespace ks::commands;

#include <vector>
#include <fstream>
#include <iostream>

int findSides(std::vector < std::vector<ECell> > &board, int posY, int posX)
{
    int i  = posY, j = posX;
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
void makeSquares(std::vector < std::vector<ECell> > &board, std::vector < std::vector < std::vector <int> > > &positions)
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


