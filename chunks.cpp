#include "chunks.hpp"

Chunk::Chunk()
    : stf::sdb::IChunk({Width, Height})
{
    mArray.resize(Width * Height, nullptr);
    for(auto &cell : mArray) {
        cell = new Cell;
    }
}

stf::sdb::IChunk *Chunk::getNew()
{
    return new Chunk;
}

bool Chunk::isInitialised() const
{
    return mIsInitialised;
}

void Chunk::initialise()
{
    mIsInitialised = true;
}

stf::sdb::IChunk &Chunk::load(FILE *file)
{
    stf::sdb::IChunk::load(file);
    fread(&mIsInitialised, sizeof(mIsInitialised), 1, file);
    if(!mIsInitialised) {
        int bombs = BombsPerChunk;
        while(bombs) {
            stf::Vec2d pos { rand() % Width, rand() % Height };
            /*if(at(pos) == nullptr)*/ {
                put(pos, new BombCell);
                mBombsPositions.push_back(pos);
                --bombs;
            }
        }
        mIsInitialised = true;
    }
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

size_t Chunk::sizeOfSelf() const
{
    return stf::sdb::IChunk::sizeOfSelf() + sizeof(mIsInitialised);
}

stf::sdb::IChunk &Chunk::save(FILE *file)
{
    stf::sdb::IChunk::save(file);
    fwrite(&mIsInitialised, sizeof(mIsInitialised), 1, file);
    return *this;
}
