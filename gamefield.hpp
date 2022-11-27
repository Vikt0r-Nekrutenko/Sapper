#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP

#include "cells.hpp"
#include "chunkedmap.hpp"

class Chunk : public stf::sdb::IChunk
{
public:
    static constexpr int Width  = 9;
    static constexpr int Height = 9;

    Chunk() : stf::sdb::IChunk({Width, Height})
    {
        mArray.resize(Width * Height);
        for(auto &cell : mArray) {
            cell = new Cell;
        }
    }

    stf::sdb::IChunk *getNew() override
    {
        return new Chunk;
    }

    void init(const stf::Vec2d &cursor)
    {
        auto checkAroundForBombs = [&](const stf::Vec2d &pos) {
            Cell *curcell = static_cast<Cell*>(at(pos));

            for(int y = pos.y-1; y <= pos.y+1; ++y) {
                for(int x = pos.x-1; x <= pos.x+1; ++x) {

                    Cell *cell = static_cast<Cell*>(at({x,y}));

                    if(x<0 || y<0 || x > Chunk::Width - 1 || y > Chunk::Height - 1)
                        continue;
                    else if(x == pos.x && y == pos.y)
                        continue;
                    else if(cell->uniqueIntView() == Cell().uniqueIntView()) {
                        delete cell;
                        put({x,y}, new BombsNeighborCell());
                        cell = static_cast<Cell*>(at({x,y}));
                    }
                    if(cell->uniqueIntView() == BombsNeighborCell().uniqueIntView())
                        ++cell->bombsAround();
                }
            }
            return curcell;
        };

        int bombs = 10;
        std::vector<stf::Vec2d> bombsPos;
        do {
            for(int i = 0; i < Width * Height; ++i) {
                const stf::Vec2d pos { i % Width, i / Width };
                if(rand() % 100 < 10 && bombs > 0 && cursor != pos) {
                    delete at(pos);
                    put(pos, new BombCell);
                    bombsPos.push_back(pos);
                    --bombs;
                }
            }
        } while(bombs > 0);

        for(auto &pos : bombsPos) {
            checkAroundForBombs(pos);
        }
        activate(cursor);
        mIsInitialised = true;
    }

    void activate(const stf::Vec2d cursor)
    {
        std::list<stf::Vec2d> emptyCells;

        Cell *cell = static_cast<Cell*>(at(cursor));
        if(cell->uniqueIntView() == BombsNeighborCell().uniqueIntView()) {
            cell->activate();
            return;
        }

        auto checkAround = [&](const stf::Vec2d &pos) {
            Cell *curcell = static_cast<Cell*>(at(pos));

            for(int y = pos.y-1; y <= pos.y+1; ++y) {
                for(int x = pos.x-1; x <= pos.x+1; ++x) {
                    Cell *cell = static_cast<Cell*>(at({x,y}));

                    if(x<0 || y<0 || x > Chunk::Width - 1 || y > Chunk::Height - 1)
                        continue;
                    else if(cell->uniqueIntView() == BombsNeighborCell().uniqueIntView()) {
                        cell->activate();
//                        continue;
                    }
                    else //if(cell->uniqueIntView() == Cell().uniqueIntView()) {
                        if(cell->uniqueIntView() == Cell().uniqueIntView()) {
                            delete cell;
                            put({x,y}, new EmptyCell);
                            emptyCells.push_back({x,y});
                            static_cast<Cell*>(at({x,y}))->activate();
                        }
//                    }
                }
            }
        };
//        static_cast<Cell*>(at(cursor))->activate();
        emptyCells.push_back(cursor);

        for(auto pos : emptyCells) {
            checkAround(pos);
//            emptyCells.pop_front();
        }
    }

    bool isInitialised() const
    {
        return mIsInitialised;
    }

protected:

    bool mIsInitialised = false;
};

class GameField
{
public:
    static constexpr int Width  = 1;
    static constexpr int Height = 1;

    Chunk mBegin = Chunk();
    stf::sdb::ChunkedMap mField = stf::sdb::ChunkedMap({Width,Height}, &mBegin, true, "sapper.schnks");

    Cell* put(const stf::Vec2d &pos, Cell* cell)
    {
        delete mField.at(pos);
        return static_cast<Cell*>(mField[pos]->put(pos, cell));
    }
};


#endif // GAMEFIELD_HPP
