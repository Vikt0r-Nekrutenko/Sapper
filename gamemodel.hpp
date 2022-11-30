#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include "imodel.hpp"
#include "chunks.hpp"
#include "chunkedmap.hpp"

class GameModel : public stf::smv::BaseModel
{
public:
    static constexpr int Width  = 5;
    static constexpr int Height = Width;

    stf::Vec2d mCursor { Chunk::Width >> 1, Chunk::Height >> 1 };
    uint32_t mLifes = 1;
    uint32_t mPoints = 0;

    Cell *onClick(const stf::Vec2d &cursor);

    stf::smv::IView *put(stf::smv::IView *sender);
    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) override;
    stf::smv::IView *update(stf::smv::IView *sender, const float dt) override;

    stf::sdb::ChunkedMap &field();

private:

    Cell *put(const stf::Vec2d &pos, Cell *cell);

    Chunk mBegin = Chunk();
    stf::sdb::ChunkedMap mField = stf::sdb::ChunkedMap({Width,Height}, &mBegin, false, "sapper.schnks");
    std::vector<stf::Vec2d> mBombsPositions;
};


#endif // GAMEMODEL_HPP
