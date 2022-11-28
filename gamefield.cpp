#include "gamefield.hpp"

Cell *GameField::put(const stf::Vec2d &pos, Cell *cell)
{
    delete mField.at(pos);
    return static_cast<Cell*>(mField[pos]->put(pos, cell));
}

Cell *GameField::onClick(const stf::Vec2d &cursor)
{

    Cell *cell = static_cast<Cell*>(mField.at(cursor));
    if(cell->uniqueIntView() == BombsNeighborCell().uniqueIntView()) {
        return cell->activate();
    }
    if(cell->uniqueIntView() == BombCell().uniqueIntView()) {
        return cell->activate();
    }

    std::list<stf::Vec2d> emptyCells { cursor };

    for(auto pos : emptyCells) {
        int aroundBombCount = 0;
        std::list<stf::Vec2d> tmpEmptyList;

        for(int y = pos.y-1; y <= pos.y+1; ++y) {
            for(int x = pos.x-1; x <= pos.x+1; ++x) {

                Cell *cell = static_cast<Cell*>(mField.at({x,y}));

                if(x<0 || y<0 || x > Width * Chunk::Width - 1 || y > Height * Chunk::Height - 1)
                    continue;

                if(cell->uniqueIntView() == BombCell().uniqueIntView())
                    ++aroundBombCount;
                else if(cell->uniqueIntView() == Cell().uniqueIntView())
                    tmpEmptyList.push_back({x,y});

            }
        }
        if(aroundBombCount) {
            put(pos, new BombsNeighborCell());
            cell = static_cast<Cell*>(mField.at(pos));
            cell->activate();
            cell->bombsAround(cell->bombsAround() + aroundBombCount);
        } else {
            for(auto &npos : tmpEmptyList) {
                put(npos, new EmptyCell);
                emptyCells.push_back(npos);
                static_cast<Cell*>(mField.at(npos))->activate();
            }
        }
    }
    return static_cast<Cell*>(mField.at(emptyCells.back()));
}

Chunk::Chunk() : stf::sdb::IChunk({Width, Height})
{
    mArray.resize(Width * Height, nullptr);
    int bombs = BombsPerChunk;
    while(bombs) {
        stf::Vec2d pos { rand() % Width, rand() % Height };
        if(at(pos) == nullptr) {
            put(pos, new BombCell);
            mBombsPositions.push_back(pos);
            --bombs;
        }
    }
    for(auto &cell : mArray) {
        if(cell == nullptr) {
            cell = new Cell;
        }
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
