#include "gamemodel.hpp"
#include "endview.hpp"
#include "menuview.hpp"

GameSaveModel::GameSaveModel(GameModel *model)
    : stf::sdb::StackModel("sapper_saves.sdb"), mModel(model) {}

void GameSaveModel::save()
{
    mCursorX = mModel->mCursor.x;
    mCursorY = mModel->mCursor.y;
    mLifes = mModel->mLifes;
    mPoints = mModel->mPoints;
    mGameT = mModel->mGameTime;

    push<GameSaveModel>();
}

void GameSaveModel::load()
{
    pop<GameSaveModel>();

    mModel->mCursor.x = mCursorX();
    mModel->mCursor.y = mCursorY();
    mModel->mLifes = mLifes();
    mModel->mPoints = mPoints();
    mModel->mGameTime = mGameT();
}

GameResultModel::GameResultModel(GameModel *model)
    : stf::sdb::Model("sapper_results.sdb"), mModel(model) {}

void GameResultModel::gameOverHandler()
{
    mTime = stf::Time(nullptr);
    mPoints = mModel->mPoints;
    mGameTime = mModel->mGameTime;
}


Cell *GameModel::put(const stf::Vec2d &pos, Cell *cell)
{
    delete mField.at(pos);
    return static_cast<Cell*>(mField[pos]->put(pos, cell));
}

stf::sdb::ChunkedMap &GameModel::field()
{
    return mField;
}

GameModel::GameModel()
    : mBegin(Chunk()),
      mField(stf::sdb::ChunkedMap({Width,Height}, &mBegin, false, "sapper.schnks"))
{
    try {
        results.load(results.header().size - 1);
    } catch(...) { }
}

Cell *GameModel::onClick(const stf::Vec2d &cursor)
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
                if(rand() % 100 < 2)
                    put(npos, new LifeCell);
                else
                    put(npos, new EmptyCell);
                emptyCells.push_back(npos);
                static_cast<Cell*>(mField.at(npos))->activate();
            }
        }
    }
    return static_cast<Cell*>(mField.at(emptyCells.back()));
}

void GameModel::reset()
{
    mField.dropFile();
    mCursor = { Width * Chunk::Width >> 1, Height * Chunk::Height >> 1 };
    mLifes  = 1;
    mPoints = 0;
    mGameTime = 0;
}

stf::smv::IView *GameModel::put(stf::smv::IView *sender)
{
    Cell *selected = onClick(mCursor);
    if (selected->uniqueIntView() == BombCell().uniqueIntView()){
        if(mLifes == 1) {
            results.gameOverHandler();
            results.save();
            return new EndView(this);
        }
        --mLifes;
    } else if(selected->uniqueIntView() == LifeCell().uniqueIntView()) {
        put(mCursor, new EmptyCell);
        static_cast<Cell*>(mField.at(mCursor))->activate();
        ++mLifes;
    }

    return sender;
}

stf::smv::IView *GameModel::keyEventsHandler(stf::smv::IView *sender, const int key)
{
    switch (key)
    {
    case 'w':
        if(mCursor.y > 0)
            mCursor.y -= 1;
        else
            mCursor.y = Height * Chunk::Height - 1;
        break;

    case 'a':
        if(mCursor.x > 0)
            mCursor.x -= 1;
        else
            mCursor.x = Width * Chunk::Width - 1;
        break;

    case 's':
        if(mCursor.y < Height * Chunk::Height-1)
            mCursor.y += 1;
        else
            mCursor.y = 0;
        break;

    case 'd':
        if(mCursor.x < Width * Chunk::Width-1)
            mCursor.x += 1;
        else
            mCursor.x = 0;
        break;
    case 'f':
    {
        Cell *selected = static_cast<Cell*>(mField.at(mCursor));
        selected->mark();
        mPoints += selected->pickUpPoints();

        if(mPoints % 8 == 0)
            ++mLifes;
        break;
    }
    case ' ':
        return put(sender);
    case 'q':
        try {
            saves.save();
            return new MenuView(this);
        } catch(...) { }
    }
    return sender;
}

float elapsed = 0;

stf::smv::IView *GameModel::update(stf::smv::IView *sender, const float dt)
{
    if(elapsed > 1000.f) {
        ++mGameTime;
        elapsed = 0.f;
    }
    elapsed += dt;

    mField.cache().update(dt);
    return sender;
}
