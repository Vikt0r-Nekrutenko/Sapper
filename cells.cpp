#include "cells.hpp"
#include "gamefield.hpp"



Cell *Cell::activate(Chunk &chunk)
{
    std::list<stf::Vec2d> emptyCells;
    auto checkAround = [&](const stf::Vec2d &pos) {
        for(int y = pos.y-1; y <= pos.y+1; ++y) {
            for(int x = pos.x-1; x <= pos.x+1; ++x) {
                if(x<0 || y<0 || x > Chunk::Width - 1 || y > Chunk::Height - 1)
                    continue;
                else if(x == pos.x && y == pos.y)
                    continue;

                else if(static_cast<Cell*>(chunk.at({x,y}))->bombsAround() == ) {
                    delete at({x,y});
                    put({x,y}, new EmptyCell);
                    static_cast<Cell*>(at({x,y}))->activate();
                    emptyCells.push_back({x,y});
                }
                else if(static_cast<Cell*>(at({x,y}))->bombsAround() == -1) {
                    delete at({x,y});
                    put({x,y}, new BombsNeighborCell);
                }
                else if(static_cast<Cell*>(at({x,y}))->bombsAround() > 0)
                {
                    static_cast<BombsNeighborCell*>(at({x,y}))->bombsAround()++;
                }
            }
        }
    };
    static_cast<Cell*>(at(cursor))->activate();
    emptyCells.push_back(cursor);

    for(auto pos : emptyCells) {
        checkAround(pos);
        emptyCells.pop_front();
    }
    return this;
}
