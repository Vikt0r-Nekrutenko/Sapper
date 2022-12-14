#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include "imodel.hpp"
#include "chunks.hpp"
#include "chunkedmap.hpp"
#include "stackmodel.hpp"
#include "fields.hpp"

#define daysFromSeconds(s)      (s / (3600 * 24))
#define hoursFromSeconds(s)     ((s / 3600) % 24)
#define minutesFromSeconds(s)   ((s / 60) % 60)
#define secondsFromSeconds(s)   (s % 60)

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
    stf::sdb::IntField mGameT   = stf::sdb::IntField(this);

    GameModel *mModel;
};

class GameResultModel : public stf::sdb::Model
{
public:
    GameResultModel(GameModel *model);

    void gameOverHandler();

    stf::sdb::DateTimeField mTime       = stf::sdb::DateTimeField(this);
    stf::sdb::IntField      mPoints     = stf::sdb::IntField(this);
    stf::sdb::IntField      mGameTime   = stf::sdb::IntField(this);

    GameModel *mModel;
};

class GameModel : public stf::smv::BaseModel
{
    friend class GameResultModel;
    friend class GameSaveModel;
    friend class GameView;
public:
    static constexpr int Width  = 5;
    static constexpr int Height = Width;
    static constexpr int TotalBombs = Width * Height * Chunk::BombsPerChunk;


    GameModel();
    Cell *onClick(const stf::Vec2d &cursor);
    void reset();

    inline uint32_t points() const { return mPoints; }
    inline uint32_t lifes() const { return mLifes; }
    inline time_t   gameTime() const { return mGameTime; }

    stf::smv::IView *put(stf::smv::IView *sender);
    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) override;
    stf::smv::IView *update(stf::smv::IView *sender, const float dt) override;

    stf::sdb::ChunkedMap &field();

    GameSaveModel   saves   = GameSaveModel(this);
    GameResultModel results = GameResultModel(this);

private:

    Cell *put(const stf::Vec2d &pos, Cell *cell);

    Chunk mBegin;
    stf::sdb::ChunkedMap mField;
    std::vector<stf::Vec2d> mBombsPositions;

    stf::Vec2d mCursor { Width * Chunk::Width >> 1, Height * Chunk::Height >> 1 };
    uint32_t mLifes  = 1;
    uint32_t mPoints = 0;
    time_t mGameTime = 0;
};

#endif // GAMEMODEL_HPP
