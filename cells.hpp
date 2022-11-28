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
        return mIsActivated ? mView : '+';
    }

    virtual int uniqueIntView() const
    {
        return 0;
    }

    virtual stf::ColorTable color() const
    {
        return mIsActivated ? mColor : stf::ColorTable::Default;
    }

    int bombsAround() const
    {
        return mBombsAround;
    }

    void bombsAround(int value)
    {
        mBombsAround = value;
        switch (mBombsAround) {
            case 1: mColor = stf::ColorTable::Cyan; break;
            case 2: mColor = stf::ColorTable::Green; break;
            case 3: mColor = stf::ColorTable::Red; break;
            case 4: mColor = stf::ColorTable::Blue; break;
            case 5: mColor = stf::ColorTable::Yellow; break;
            case 6: mColor = stf::ColorTable::Magenta; break;
            case 7: mColor = stf::ColorTable::Default; break;
            case 8: mColor = stf::ColorTable::White; break;
        }
    }

    bool activate()
    {
        return mIsActivated = true;
    }

protected:

    int mBombsAround = 0;
    bool mIsActivated = false;
    uint8_t mView = '+';
    stf::ColorTable mColor = stf::ColorTable::Default;
};

class EmptyCell : public Cell
{
public:
    EmptyCell()
    {
        mView = '-';
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

    int uniqueIntView() const override
    {
        return 2;
    }
};

class BombsNeighborCell : public Cell
{
public:
    int uniqueIntView() const override
    {
        return 3;
    }

    uint8_t view() const override
    {
        return mIsActivated ? '0' + mBombsAround : '-';
    }
};

#endif // CELLS_HPP
