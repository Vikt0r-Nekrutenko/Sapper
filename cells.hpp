#ifndef CELLS_HPP
#define CELLS_HPP

#include "chunk.hpp"
#include "renderer.hpp"

class Chunk;

class Cell : public stf::sdb::ICell
{
public:
    size_t sizeOfSelf() const override
    {
        return sizeof(mView) + sizeof(mColor);
    }

    void save(FILE *file) override
    {
        fwrite(&mView, sizeof(mView), 1, file);
        fwrite(&mColor, sizeof(mColor), 1, file);
    }

    void load(FILE *file) override
    {
        fread(&mView, sizeof(mView), 1, file);
        fread(&mColor, sizeof(mColor), 1, file);
    }

    virtual uint8_t view() const
    {
//        return mView;
        return mIsActivated ? mView : '-';
    }

    virtual stf::ColorTable color() const
    {
        return mColor;
    }

    int bombsAround() const
    {
        return mBombsAround;
    }

    int& bombsAround()
    {
        return mBombsAround;
    }

    bool activate()
    {
        return mIsActivated = true;
    }

    virtual bool activate(Chunk &chunk);

protected:

    int mBombsAround = -2;
    bool mIsActivated = false;
    uint8_t mView = '-';
    stf::ColorTable mColor = stf::ColorTable::Default;
};

class EmptyCell : public Cell
{
public:
    EmptyCell()
    {
        mView = '.';
        mBombsAround = -3;
    }
};

class BombCell : public Cell
{
public:
    BombCell()
    {
        mView = 'o';
        mBombsAround = 0;
    }
};

class BombsNeighborCell : public Cell
{
public:
    BombsNeighborCell()
    {
        mColor = stf::ColorTable::Red;
        mBombsAround = 1;
    }

    uint8_t view() const override
    {
        return mIsActivated ? '0' + mBombsAround : '-';
    }
};

#endif // CELLS_HPP
