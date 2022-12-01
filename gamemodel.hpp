#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include "imodel.hpp"
#include "chunks.hpp"
#include "chunkedmap.hpp"
#include "stackmodel.hpp"
#include "fields.hpp"

class GameModel;

class GameSaveModel : public stf::sdb::StackModel
{
public:
    GameSaveModel(GameModel *model);

    void save();
    void load();

    stf::sdb::IntField mCursorX = stf::sdb::IntField(this);
    stf::sdb::IntField mCursorY = stf::sdb::IntField(this);
    stf::sdb::IntField mLifes   = stf::sdb::IntField(this);
    stf::sdb::IntField mPoints  = stf::sdb::IntField(this);

    GameModel *mModel;
};

class GameResultModel : public stf::sdb::Model
{
public:
    GameResultModel();

    void gameOverHandler(int winner, const stf::Vec2d& wins);;

    stf::sdb::DateTimeField gameTime    = stf::sdb::DateTimeField(this);
    stf::sdb::IntField      mPoints     = stf::sdb::IntField(this);
    stf::sdb::DateTimeField mGameTime   = stf::sdb::DateTimeField(this);
};

class GameModel : public stf::smv::BaseModel
{
public:
    static constexpr int Width  = 105;
    static constexpr int Height = Width;

    stf::Vec2d mCursor {0,0};//{ Width * Chunk::Width >> 1, Height * Chunk::Height >> 1 };
    uint32_t mLifes = 1;
    uint32_t mPoints = 0;

    GameModel();
    Cell *onClick(const stf::Vec2d &cursor);

    stf::smv::IView *put(stf::smv::IView *sender);
    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) override;
    stf::smv::IView *update(stf::smv::IView *sender, const float dt) override;

    stf::sdb::ChunkedMap &field();

    GameSaveModel saves = GameSaveModel(this);
    GameResultModel results = GameResultModel();

private:

    Cell *put(const stf::Vec2d &pos, Cell *cell);

    Chunk mBegin = Chunk();
    stf::sdb::ChunkedMap mField = stf::sdb::ChunkedMap({Width,Height}, &mBegin, true, "sapper.schnks");
    std::vector<stf::Vec2d> mBombsPositions;
};

#endif // GAMEMODEL_HPP
