#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP

#include "cells.hpp"
#include "chunkedmap.hpp"

class Chunk : public stf::sdb::IChunk
{
public:
    static constexpr int Width  = 9;
    static constexpr int Height = 9;
    std::vector<stf::Vec2d> mBombsPositions;

    Chunk() : stf::sdb::IChunk({Width, Height})
    {
        mArray.resize(Width * Height);
        for(auto &cell : mArray) {
            cell = new Cell;
        }

        int bombs = 10;
        do {
            for(int i = 0; i < Width * Height; ++i) {
                const stf::Vec2d pos { i % Width, i / Width };
                if(rand() % 100 < 10 && bombs > 0) {
                    delete at(pos);
                    put(pos, new BombCell);
                    mBombsPositions.push_back(pos);
                    --bombs;
                }
            }
        } while(bombs > 0);
    }

    stf::sdb::IChunk *getNew() override
    {
        return new Chunk;
    }

    bool isInitialised() const
    {
        return mIsInitialised;
    }

    bool &initialise()
    {
        return mIsInitialised;
    }

protected:

    bool mIsInitialised = false;
};

class GameField
{
public:
    static constexpr int Width  = 2;
    static constexpr int Height = 2;

    Chunk mBegin = Chunk();
    stf::sdb::ChunkedMap mField = stf::sdb::ChunkedMap({Width,Height}, &mBegin, true, "sapper.schnks");
    std::vector<stf::Vec2d> mBombsPositions;


    Cell* put(const stf::Vec2d &pos, Cell* cell)
    {
        delete mField.at(pos);
        return static_cast<Cell*>(mField[pos]->put(pos, cell));
    }

    void activate(const stf::Vec2d cursor)
    {
        std::list<stf::Vec2d> emptyCells;

        Cell *cell = static_cast<Cell*>(mField.at(cursor));
        if(cell->uniqueIntView() == BombsNeighborCell().uniqueIntView()) {
            cell->activate();
            return;
        }

        auto checkAround = [&](const stf::Vec2d &pos) {

            for(int y = pos.y-1; y <= pos.y+1; ++y) {
                for(int x = pos.x-1; x <= pos.x+1; ++x) {
                    Cell *cell = static_cast<Cell*>(mField.at({x,y}));

                    if(x<0 || y<0 || x > Width * Chunk::Width - 1 || y > Height * Chunk::Height - 1)
                        continue;
                    else if(cell->uniqueIntView() == Cell().uniqueIntView()) {
//                        delete cell;
                        put({x,y}, new EmptyCell);
                        emptyCells.push_back({x,y});
                        static_cast<Cell*>(mField.at({x,y}))->activate();
                    }
                    else if(cell->uniqueIntView() == BombsNeighborCell().uniqueIntView()) {
                        cell->activate();
                    }
                }
            }
        };

        emptyCells.push_back(cursor);

        for(auto pos : emptyCells) {
            checkAround(pos);
        }
    }

    void init(const stf::Vec2d &cursor)
    {
//        if(static_cast<Chunk*>(mField[cursor])->isInitialised())
//            return;

        auto checkAroundForBombs = [&](const stf::Vec2d &pos) {
            Cell *curcell = static_cast<Cell*>(mField.at(pos));

            for(int y = pos.y-1; y <= pos.y+1; ++y) {
                for(int x = pos.x-1; x <= pos.x+1; ++x) {

                    Cell *cell = static_cast<Cell*>(mField.at({x,y}));

                    if(x<0 || y<0 || x > Width * Chunk::Width - 1 || y > Height * Chunk::Height - 1)
                        continue;
                    else if(x == pos.x && y == pos.y)
                        continue;
                    else if(cell->uniqueIntView() == Cell().uniqueIntView()) {
                        put({x,y}, new BombsNeighborCell());
                        cell = static_cast<Cell*>(mField.at({x,y}));

                        int mineCount = 0;
                        for(int ny = y-1; ny <= y+1; ++ny) {
                            for(int nx = x-1; nx <= x+1; ++nx) {
                                Cell *neighbor = static_cast<Cell*>(mField.at({nx,ny}));
                                if(nx<0 || ny<0 || nx > Width * Chunk::Width - 1 || ny > Height * Chunk::Height - 1)
                                    continue;
                                if(nx == x && ny == y)
                                    continue;
                                if(neighbor->uniqueIntView() == BombCell().uniqueIntView())
                                    ++mineCount;
                            }
                        }
                        cell->bombsAround() = mineCount;
                    }
                }
            }
            return curcell;
        };

        mBombsPositions.clear();
        for(auto &ichunk : mField.cache().chunksTable()) {
            Chunk *chunk = static_cast<Chunk*>(ichunk.mChunkRec.mChunk);
            for(auto &bombcell : chunk->mBombsPositions)
                if(!chunk->isInitialised())
                    checkAroundForBombs(ichunk.mChunkRec.mPos * stf::Vec2d{Chunk::Width, Chunk::Height} + bombcell);
            static_cast<Chunk*>(ichunk.mChunkRec.mChunk)->initialise() = true;
        }

//        for(auto &pos : mBombsPositions) {
//            checkAroundForBombs(pos);
//        }
//        activate(cursor);
//        mIsInitialised = true;
    }
};


#endif // GAMEFIELD_HPP
