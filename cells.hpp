#ifndef CELLS_HPP
#define CELLS_HPP

#include "chunk.hpp"
#include "renderer.hpp"

class Chunk;

class Cell : public stf::sdb::ICell
{
public:
    static const uint8_t UninitialisedCellView = '+';
    static const uint8_t MarkedCellView = 'F';

    Cell() = default;
    Cell(Cell &cell) = default;

    static size_t mCellNewCount;
    static size_t mCellDelCount;

    static size_t mECellNewCount;
    static size_t mECellDelCount;

    static size_t mBCellNewCount;
    static size_t mBCellDelCount;

    static size_t mNCellNewCount;
    static size_t mNCellDelCount;

    static size_t mLCellNewCount;
    static size_t mLCellDelCount;

    void* operator new(size_t size);
    void operator delete(void *ptr);

    void save(FILE *file) override;
    void load(FILE *file) override;

    virtual uint8_t view() const;
    virtual int uniqueIntView() const;
    virtual stf::ColorTable color() const;
    virtual int pickUpPoints();

    Cell *activate();
    bool isActivated() const;
    int bombsAround() const;
    void bombsAround(int value);
    size_t sizeOfSelf() const override;
    uint8_t mark();

protected:

    int mBombsAround = 0;
    int mUniqueView = 0;
    int mPoints = 0;
    bool mIsActivated = false;
    uint8_t mView = UninitialisedCellView;
    uint8_t mAlterView = UninitialisedCellView;
    stf::ColorTable mColor = stf::ColorTable::Default;
    uint8_t __empty[12];
};

class EmptyCell : public Cell
{
public:
    EmptyCell();

    void* operator new(size_t size);
    void operator delete(void *ptr);
};

class LifeCell : public Cell
{
public:
    LifeCell();

    void* operator new(size_t size);
    void operator delete(void *ptr);
};

class BombCell : public Cell
{
public:
    BombCell();

    stf::ColorTable color() const override;

    void* operator new(size_t size);
    void operator delete(void *ptr);
};

class BombsNeighborCell : public Cell
{
public:
    BombsNeighborCell();

    uint8_t view() const override;

    void* operator new(size_t size);
    void operator delete(void *ptr);
};

#endif // CELLS_HPP
