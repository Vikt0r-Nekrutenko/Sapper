#include "gamefield.hpp"

Cell *GameField::put(const stf::Vec2d &pos, Cell *cell)
{
    delete mField.at(pos);
    return static_cast<Cell*>(mField[pos]->put(pos, cell));
}

void GameField::update()
{
    for(auto &rec : mField.cache().chunksTable()) {
        if(!static_cast<Chunk*>(rec.mChunkRec.mChunk)->isInitialised())
            initChunks();
    }
}

void GameField::activate(const stf::Vec2d cursor)
{
    std::list<stf::Vec2d> emptyCells;

    Cell *cell = static_cast<Cell*>(mField.at(cursor));
    if(cell->uniqueIntView() == BombsNeighborCell().uniqueIntView()) {
        cell->activate();
        return;
    }

    emptyCells.push_back(cursor);

    for(auto pos : emptyCells) {
        activateCells(pos, emptyCells);
    }
}

void GameField::initChunks()
{
    mBombsPositions.clear();
    for(auto &ichunk : mField.cache().chunksTable()) {
        Chunk *chunk = static_cast<Chunk*>(ichunk.mChunkRec.mChunk);
        for(auto &bombcell : chunk->mBombsPositions)
            if(!chunk->isInitialised())
                putBombMarkers(ichunk.mChunkRec.mPos * stf::Vec2d{Chunk::Width, Chunk::Height} + bombcell);
        static_cast<Chunk*>(ichunk.mChunkRec.mChunk)->initialise();
    }
}

void GameField::activateCells(const stf::Vec2d &pos, std::list<stf::Vec2d> &emptyCells)
{
    for(int y = pos.y-1; y <= pos.y+1; ++y) {
        for(int x = pos.x-1; x <= pos.x+1; ++x) {
            Cell *cell = static_cast<Cell*>(mField.at({x,y}));

            if(x<0 || y<0 || x > Width * Chunk::Width - 1 || y > Height * Chunk::Height - 1)
                continue;

            else if(cell->uniqueIntView() == Cell().uniqueIntView()) {
                put({x,y}, new EmptyCell);
                emptyCells.push_back({x,y});
                static_cast<Cell*>(mField.at({x,y}))->activate();
            }

            else if(cell->uniqueIntView() == BombsNeighborCell().uniqueIntView()) {
                cell->activate();
            }
        }
    }
}

int GameField::calculateBombsAround(const stf::Vec2d &pos)
{
    int bombCount = 0;
    for(int y = pos.y - 1; y <= pos.y + 1; ++y) {
        for(int x = pos.x - 1; x <= pos.x + 1; ++x) {
            Cell *neighbor = static_cast<Cell*>(mField.at({x,y}));

            if(x<0 || y<0 || x > Width * Chunk::Width - 1 || y > Height * Chunk::Height - 1)
                continue;

            else if(x == pos.x && y == pos.y)
                continue;

            else if(neighbor->uniqueIntView() == BombCell().uniqueIntView())
                ++bombCount;
        }
    }
    return bombCount;
}

void GameField::putBombMarkers(const stf::Vec2d &pos)
{
    for(int y = pos.y-1; y <= pos.y+1; ++y) {
        for(int x = pos.x-1; x <= pos.x+1; ++x) {

            Cell *cell = static_cast<Cell*>(mField.at({x,y}));

            if(x<0 || y<0 || x > Width * Chunk::Width - 1 || y > Height * Chunk::Height - 1)
                continue;

            else if(x == pos.x && y == pos.y)
                continue;

            else if(cell->uniqueIntView() != BombCell().uniqueIntView()) {
                put({x,y}, new BombsNeighborCell());
                cell = static_cast<Cell*>(mField.at({x,y}));

                cell->bombsAround(calculateBombsAround({x,y}));
            }
        }
    }
}
