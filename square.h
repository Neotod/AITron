#ifndef SQUARE_H
#define SQUARE_H

#include "ai.h"
#include "ks/models.h"
#include "ks/commands.h"
#include <vector>

using namespace koala::chillin::client;
using namespace ks::models;
using namespace ks::commands;

class Square
{
private:
    std::vector <int> nearestSquaresVar;
    std::vector < std::vector <int> > positionVar;

public:
    void setPosition(std::shared_ptr< std::vector < std::vector <int> > > position);
    void setNearestSquares(std::shared_ptr < std::vector <int> > nearestSquaresPtr);
    Square();

public: //read-only functions
    std::vector < std::vector <int> > position() const;
    std::vector <int> nearestSquares() const;
};


//implementations:
Square::Square()
{
}

inline std::vector < std::vector <int> > Square::position() const
{
    return positionVar;
}

inline void Square::setPosition(std::shared_ptr< std::vector < std::vector <int> > > positionPtr)
{
    positionVar = *positionPtr;
}

inline void Square::setNearestSquares(std::shared_ptr < std::vector <int> > nearestSquaresPtr)
{
    nearestSquaresVar = *nearestSquaresPtr;
}

inline std::vector <int> Square::nearestSquares() const
{
    return nearestSquaresVar;
}

#endif //SQUARE_H
