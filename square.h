#ifndef SQUARE_H
#define SQUARE_H

#include "ks/models.h"
#include "ks/commands.h"
#include <memory>
#include <vector>

using namespace koala::chillin::client;
using namespace ks::models;
using namespace ks::commands;


class Square
{
private:
    int weight;
    std::shared_ptr< std::vector < std::vector <int> > > bestRoutePos;
    std::shared_ptr< std::vector < std::vector <int> > > positionPtr;

public:
    void findBestRoute();
    int findSquareWeight();
    void setWeight(int squareWeight);
    Square();

public: //read-only functions
    std::vector < std::vector <int> > position();
    void setPosition(std::shared_ptr< std::vector < std::vector <int> > > &vector);
    std::shared_ptr< std::vector <int> > getNearestPos() const;
    std::shared_ptr< std::vector < std::vector <int> > > getBestRoutePos() const;
    int getWeight() const;
    int getRouteLength() const;
};

Square::Square()
{
}

void Square::findBestRoute()
{

}

inline std::vector < std::vector <int> > Square::position()
{
    return *positionPtr;
}

inline void Square::setPosition(std::shared_ptr< std::vector < std::vector <int> > > &position)
{
    positionPtr = position;
}

inline std::shared_ptr< std::vector <int> > Square::getNearestPos() const
{
    return nearestPos;
}

inline std::shared_ptr< std::vector < std::vector <int> > > Square::getBestRoutePos() const
{
    return bestRoutePos;
}

inline int Square::getWeight() const
{
    return weight;
}

inline int Square::getRouteLength() const
{

}

inline void Square::setWeight(int squareWeight)
{
    weight = squareWeight;
}

#endif // SQUARE_H
