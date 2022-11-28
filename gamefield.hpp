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

    void initialise()
    {
        mIsInitialised = true;
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

    void update();
    void activate(const stf::Vec2d cursor);

private:

    Cell* put(const stf::Vec2d &pos, Cell* cell);
    int calculateBombsAround(const int x, const int y);
    void activateCells(const stf::Vec2d &pos, std::list<stf::Vec2d> &emptyCells);
    void putBombMarkers(const stf::Vec2d &pos);
    void initChunks();
};


#endif // GAMEFIELD_HPP
