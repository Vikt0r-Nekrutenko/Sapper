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
        mArray.resize(Width * Height, nullptr);
        int bombs = BombsPerChunk;
        while(bombs) {
            stf::Vec2d pos { rand() % Width, rand() % Height };
            if(at(pos) == nullptr) {
                put(pos, new BombCell);
                mBombsPositions.push_back(pos);
            }
            else continue;
            --bombs;
        }
        for(auto &cell : mArray) {
            if(cell == nullptr) {
                cell = new Cell;
            }
        }
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
        fread(&mIsInitialised, sizeof(mIsInitialised), 1, file);

        for(size_t i = 0; i < mArray.size(); ++i) {
            switch (static_cast<Cell*>(mArray[i])->uniqueIntView()) {
            case 1: {
                auto tmp = new EmptyCell(*static_cast<EmptyCell *>(mArray[i]));
                delete mArray[i];
                mArray[i] = tmp;
                break;
            }
            case 2: {
                auto tmp = new BombCell(*static_cast<BombCell *>(mArray[i]));
                delete mArray[i];
                mArray[i] = tmp;
                break;
            }
            case 3: {
                auto tmp = new BombsNeighborCell(*static_cast<BombsNeighborCell *>(mArray[i]));
                delete mArray[i];
                mArray[i] = tmp;
                break;
            }
            }
        }
        return *this;
    }

    size_t sizeOfSelf() const override
    {
        return stf::sdb::IChunk::sizeOfSelf() + sizeof(mIsInitialised);
    }

    stf::sdb::IChunk &save(FILE *file) override
    {
        stf::sdb::IChunk::save(file);
        fwrite(&mIsInitialised, sizeof(mIsInitialised), 1, file);
        return *this;
    }

protected:

    bool mIsInitialised = false;
};

class GameField
{
public:
    static constexpr int Width  = 5;
    static constexpr int Height = Width;

    Chunk mBegin = Chunk();
    stf::sdb::ChunkedMap mField = stf::sdb::ChunkedMap({Width,Height}, &mBegin, true, "sapper.schnks");
    std::vector<stf::Vec2d> mBombsPositions;

    void update();
    void activate(const stf::Vec2d cursor);

private:

    Cell* put(const stf::Vec2d &pos, Cell* cell);
    int calculateBombsAround(const stf::Vec2d &pos);
    void activateCells(const stf::Vec2d &pos, std::list<stf::Vec2d> &emptyCells);
    void putBombMarkers(const stf::Vec2d &pos);
};


#endif // GAMEFIELD_HPP
