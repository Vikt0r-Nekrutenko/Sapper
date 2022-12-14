#include "cells.hpp"

size_t Cell::mCellNewCount = 0;
size_t Cell::mCellDelCount = 0;

size_t Cell::mECellNewCount = 0;
size_t Cell::mECellDelCount = 0;

size_t Cell::mBCellNewCount = 0;
size_t Cell::mBCellDelCount = 0;

size_t Cell::mNCellNewCount = 0;
size_t Cell::mNCellDelCount = 0;

size_t Cell::mLCellNewCount = 0;
size_t Cell::mLCellDelCount = 0;

void *Cell::operator new(size_t size)
{
    void *ptr = std::malloc(size);
    ++mCellNewCount;
    return ptr;
}

void Cell::operator delete(void *ptr)
{
    ++mCellDelCount;
    std::free(ptr);
}

size_t Cell::sizeOfSelf() const
{
    return sizeof(mView) + sizeof(mColor) + sizeof(mIsActivated) + sizeof(mAlterView) + sizeof(mBombsAround) + sizeof(mPoints) + sizeof(mUniqueView);
}

uint8_t Cell::mark()
{
    return mAlterView = mAlterView == MarkedCellView ? mView : MarkedCellView;
}

void Cell::save(FILE *file)
{
    fwrite(&mView, sizeof(mView), 1, file);
    fwrite(&mColor, sizeof(mColor), 1, file);
    fwrite(&mIsActivated, sizeof(mIsActivated), 1, file);
    fwrite(&mAlterView, sizeof(mIsActivated), 1, file);
    fwrite(&mBombsAround, sizeof(mBombsAround), 1, file);
    fwrite(&mUniqueView, sizeof(mUniqueView), 1, file);
    fwrite(&mPoints, sizeof(mPoints), 1, file);
}

void Cell::load(FILE *file)
{
    fread(&mView, sizeof(mView), 1, file);
    fread(&mColor, sizeof(mColor), 1, file);
    fread(&mIsActivated, sizeof(mIsActivated), 1, file);
    fread(&mAlterView, sizeof(mIsActivated), 1, file);
    fread(&mBombsAround, sizeof(mBombsAround), 1, file);
    fread(&mUniqueView, sizeof(mUniqueView), 1, file);
    fread(&mPoints, sizeof(mPoints), 1, file);
}

uint8_t Cell::view() const
{
    return mIsActivated ? mView : mAlterView;
}

int Cell::uniqueIntView() const
{
    return mUniqueView;
}

stf::ColorTable Cell::color() const
{
    return mIsActivated ? mColor : stf::ColorTable::Default;
}

int Cell::pickUpPoints()
{
    if(mPoints)
        return mPoints--;
    return mPoints;
}

int Cell::bombsAround() const
{
    return mBombsAround;
}

void Cell::bombsAround(int value)
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

Cell *Cell::activate()
{
    mIsActivated = true;
    return this;
}

bool Cell::isActivated() const
{
    return mIsActivated;
}

EmptyCell::EmptyCell()
{
    mView = '-';
    mUniqueView = 1;
}

void *EmptyCell::operator new(size_t size)
{
    void *ptr = std::malloc(size);
    ++mECellNewCount;
    return ptr;
}

void EmptyCell::operator delete(void *ptr)
{
    ++mECellDelCount;
    std::free(ptr);
}

BombCell::BombCell()
{
    mView = 'o';
    mUniqueView = 2;
    mPoints = 1;
}

stf::ColorTable BombCell::color() const
{
    return mIsActivated ? stf::ColorTable::Magenta : mColor;
}

void *BombCell::operator new(size_t size)
{
    void *ptr = std::malloc(size);
    ++mBCellNewCount;
    return ptr;
}

void BombCell::operator delete(void *ptr)
{
    ++mBCellDelCount;
    std::free(ptr);
}

BombsNeighborCell::BombsNeighborCell()
{
    mUniqueView = 3;
}

uint8_t BombsNeighborCell::view() const
{
    return mIsActivated ? '0' + mBombsAround : mAlterView;
}

void *BombsNeighborCell::operator new(size_t size)
{
    void *ptr = std::malloc(size);
    ++mNCellNewCount;
    return ptr;
}

void BombsNeighborCell::operator delete(void *ptr)
{
    ++mNCellDelCount;
    std::free(ptr);
}

LifeCell::LifeCell()
{
    mUniqueView = 4;
    mView = 'L';
    mColor = stf::ColorTable::Red;
}

void *LifeCell::operator new(size_t size)
{
    void *ptr = std::malloc(size);
    ++mLCellNewCount;
    return ptr;
}

void LifeCell::operator delete(void *ptr)
{
    ++mLCellDelCount;
    std::free(ptr);
}
