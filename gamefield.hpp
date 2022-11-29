#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP

#include "chunkedmap.hpp"
#include "chunks.hpp"

class GameField
{
public:
    static constexpr int Width  = 5;
    static constexpr int Height = Width;

    Chunk mBegin = Chunk();
    stf::sdb::ChunkedMap mField = stf::sdb::ChunkedMap({Width,Height}, &mBegin, true, "sapper.schnks");
    std::vector<stf::Vec2d> mBombsPositions;

    Cell *onClick(const stf::Vec2d &cursor);
    Cell* put(const stf::Vec2d &pos, Cell* cell);
};


#endif // GAMEFIELD_HPP
