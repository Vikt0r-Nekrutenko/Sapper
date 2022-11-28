#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP

#include "cells.hpp"
#include "chunkedmap.hpp"

class Chunk : public stf::sdb::IChunk
{
public:
    static constexpr int Width  = 9;
    static constexpr int Height = 9;
    static constexpr int BombsPerChunk = 10;
    std::vector<stf::Vec2d> mBombsPositions;

    Chunk() : stf::sdb::IChunk({Width, Height})
    {
        mArray.resize(Width * Height);
        for(auto &cell : mArray) {
            cell = new Cell;
        }

        int bombs = BombsPerChunk;
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

    stf::sdb::IChunk &load(FILE *file) override
    {
        stf::sdb::IChunk::load(file);

        for(size_t i = 0; i < mArray.size(); ++i) {
            switch (static_cast<Cell*>(mArray[i])->uniqueIntView()) {
            case 1: delete mArray[i]; mArray[i] = new EmptyCell(*static_cast<EmptyCell *>(mArray[i])); break;
            case 2: delete mArray[i]; mArray[i] = new BombCell(*static_cast<BombCell *>(mArray[i])); break;
            case 3: delete mArray[i]; mArray[i] = new BombsNeighborCell(*static_cast<BombsNeighborCell *>(mArray[i])); break;
            }
        }
        return *dynamic_cast<stf::sdb::IChunk*>(this);
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
    stf::sdb::ChunkedMap mField = stf::sdb::ChunkedMap({Width,Height}, &mBegin, false, "sapper.schnks");
    std::vector<stf::Vec2d> mBombsPositions;

    void update();
    void activate(const stf::Vec2d cursor);

private:

    Cell* put(const stf::Vec2d &pos, Cell* cell);
    int calculateBombsAround(const stf::Vec2d &pos);
    void activateCells(const stf::Vec2d &pos, std::list<stf::Vec2d> &emptyCells);
    void putBombMarkers(const stf::Vec2d &pos);
    void initChunks();
};


#endif // GAMEFIELD_HPP
