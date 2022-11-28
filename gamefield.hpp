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

    Chunk();

    stf::sdb::IChunk *getNew() override;
    stf::sdb::IChunk &load(FILE *file) override;
    stf::sdb::IChunk &save(FILE *file) override;

    bool isInitialised() const;
    void initialise();
    size_t sizeOfSelf() const override;

protected:

    bool mIsInitialised = false;
};

class GameField
{
public:
    static constexpr int Width  = 5;
    static constexpr int Height = Width;

    Chunk mBegin = Chunk();
    stf::sdb::ChunkedMap mField = stf::sdb::ChunkedMap({Width,Height}, &mBegin, false, "sapper.schnks");
    std::vector<stf::Vec2d> mBombsPositions;

    Cell *onClick(const stf::Vec2d &cursor);

private:

    Cell* put(const stf::Vec2d &pos, Cell* cell);
};


#endif // GAMEFIELD_HPP
