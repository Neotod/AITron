#ifndef CHILLNCODE_H
#define CHILLNCODE_H

#endif // CHILLNCODE_H

#include "ks/models.h"
#include "ks/commands.h"
#include "ai.h"

using namespace koala::chillin::client;
using namespace ks::models;
using namespace ks::commands;
using namespace std;

#include <vector>

bool isReached(vector < vector < vector<int> > > &positions, int posX, int posY)
{
}
int findSides(vector < vector<ECell> > &board, int posY, int posX)
{
    int i  = posY, j = posX;
    int counter = 0;
    if(posY < board.size()-3)
    {
        while(board[i][j] != ECell::AreaWall)
        {
            if(i == posY && posX <= j < posX+2)
            {
                j++;
            }
            else if(i < posY+2 && j == posX+2)
            {
                i++;
            }
            else if(i == posY+2 && posX < j <= posX+2)
            {
                j--;
            }
            else if(posY < i <= posY+2 && j == posX)
            {
                i--;
            }
            counter++;
        }
        if(counter == 9)
            return 3;
    }

    while(board[i][j] != ECell::AreaWall)
    {
        if(i == posY && posX <= j < posX+1)
        {
            j++;
        }
        else if(i < posY+1 && j == posX+1)
        {
            i++;
        }
        else if(i == posY+1 && posX < j <= posX+1)
        {
            j--;
        }
        else if(posY < i <= posY+1 && j == posX)
        {
            i--;
        }
        counter++;
    }
    if(counter == 4)
        return 2;

    return 1;
}

void makeSquares(vector < vector<ECell> > &board, vector < vector < vector <int> > > &positions)
{
    int i = 1, j = 1;
    bool reachingCheck[3][board[0].size()-2];
    int arrayI = 0, arrayJ = 0;
    while(i < board.size()-1)
    {
        if(board[i][j] != ECell::AreaWall)
        {
            int sides = findSides(board, j, i);
            if(sides == 3)
            {
                for(int k = 0; k < 9; k++)
                {
                    reachingCheck[arrayI][arrayJ] = 1;
                    arrayJ++;
                    if(arrayJ == 2)
                    {
                        arrayI++;
                        arrayJ = 0;
                    }
                }
                arrayJ+=3;
                arrayI = 0;

                vector < vector<int> > tempVector;
                vector <int> tempVector2;
                tempVector2.push_back(i);
                tempVector2.push_back(j);
                tempVector.push_back(tempVector2);
                tempVector2.clear();
                tempVector2.push_back(i+2);
                tempVector2.push_back(j+2);
                tempVector.push_back(tempVector2);

                j+=3;
            }
            else if(sides == 2)
            {
                for(int k = 0; k < 4; k++)
                {
                    reachingCheck[arrayI][arrayJ] = 1;
                    arrayJ++;
                    if(arrayJ == 1)
                    {
                        arrayI++;
                        arrayJ = 0;
                    }
                }
                arrayJ+=2;
                arrayI = 0;

                vector < vector<int> > tempVector;
                vector <int> tempVector2;
                tempVector2.push_back(i);
                tempVector2.push_back(j);
                tempVector.push_back(tempVector2);
                tempVector2.clear();
                tempVector2.push_back(i+1);
                tempVector2.push_back(j+1);
                tempVector.push_back(tempVector2);

                j+=2;
            }
            else if(sides == 1)
            {
                reachingCheck[arrayI][arrayJ] = 1;
                arrayJ+=1;
                arrayI = 0;

                vector < vector<int> > tempVector;
                vector <int> tempVector2;
                tempVector2.push_back(i);
                tempVector2.push_back(j);
                tempVector.push_back(tempVector2);
                tempVector2.clear();
                tempVector2.push_back(i);
                tempVector2.push_back(j);
                tempVector.push_back(tempVector2);
                j+=1;
            }
        }
        if(j == board[0].size()-1)
        {
            for(int k = 0; k < 3; k++)
            {
                for(int m = 0; m < positions[0].size()-2; m++)
                {
                    if(reachingCheck[k][m] != 1)
                    {
                        vector < vector<int> > tempVector;
                        vector <int> tempVector2;
                        tempVector2.push_back(i+k);
                        tempVector2.push_back(m+1);
                        tempVector.push_back(tempVector2);
                        tempVector2.clear();
                        tempVector2.push_back(i+k);
                        tempVector2.push_back(m+1);
                        tempVector.push_back(tempVector2);
                    }
                }
            }
            j = 1;
            i+=3;
            arrayI = 0;
            arrayJ = 0;
        }
        j++;
        i++;
    }
}


