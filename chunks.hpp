#ifndef CHUNKS_H
#define CHUNKS_H

#include "chunk.hpp"
#include "cells.hpp"

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

#endif // CHUNKS_H
