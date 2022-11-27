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
        return mView;
    }

    virtual int uniqueIntView() const
    {
        return 0;
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

protected:

    int mBombsAround = 0;
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
    }

    int uniqueIntView() const override
    {
        return 1;
    }
};

class BombCell : public Cell
{
public:
    BombCell()
    {
        mView = 'o';
    }

    int uniqueIntView() const
    {
        return 2;
    }
};

class BombsNeighborCell : public Cell
{
public:
    BombsNeighborCell()
    {
        mColor = stf::ColorTable::Red;
    }

    int uniqueIntView() const override
    {
        return 3;
    }

    uint8_t view() const override
    {
        return '0' + mBombsAround;
    }
};

#endif // CELLS_HPP
