#include <iostream>
#include <string>
#include <iview.hpp>
#include "window.hpp"
#include "gamemodel.hpp"

class GameView : public stf::smv::IView
{
    bool mIsConsoleShow = false;

public:
    GameView(GameModel *model)
        : stf::smv::IView(model) { }

    void show(stf::Renderer &renderer) override
    {
        GameModel *GM = static_cast<GameModel*>(m_model);

        constexpr int halfHeight = Chunk::Height >> 1;
        constexpr int halfWidth  = Chunk::Width  >> 1;

        for(int j = 0, y = GM->mCursor.y - halfHeight; y <= GM->mCursor.y + halfHeight; ++j, ++y) {
            for(int i = 0, x = GM->mCursor.x - halfWidth; x <= GM->mCursor.x + halfWidth; ++i, ++x) {
                Chunk *chunk = (Chunk*)GM->field()[{x,y}];
                if(chunk != nullptr) {
                    Cell *cell = (Cell*)chunk->at({x,y});
                    renderer.drawPixel({ i * 2 + 1, j + 2 }, cell->view(), cell->color());
                } else {
                    renderer.drawPixel({ i * 2 + 1, j + 2 }, '.');
                }

                if(GM->mCursor != stf::Vec2d{-1,-1})
                {
                    renderer.drawPixel({halfWidth * 2,     halfHeight + 2}, '>');
                    renderer.drawPixel({halfWidth * 2 + 2, halfHeight + 2}, '<');
                }
                renderer.draw({0,11}, "Points: %d", GM->mPoints);
                renderer.draw({0,12}, "Lifes:  %d", GM->mLifes);
            }
        }
        if(mIsConsoleShow)
            showConsole(renderer, *GM);
    }

    void showConsole(stf::Renderer &renderer, GameModel &GM)
    {
        renderer.draw({40, 2}, "Selectable: X[%d]:Y[%d]", GM.mCursor.x, GM.mCursor.y);

        renderer.draw({40,5}, "New/Del OP : [%d] [%d]", (int)GM.field().cache().newOperationsCount(), (int)GM.field().cache().deleteOperationsCount());
        renderer.draw({40,6}, "Load/Save  : [%d] [%d]", (int)GM.field().cache().loadCount(), (int)GM.field().cache().saveCount());
        renderer.draw({40,7}, "Chunks     : %d", (int)GM.field().cache().cacheSize());
        renderer.draw({40,8}, "Memory     : %fkb", (float)GM.field().cache().memUsage()/1'000.f);

        renderer.draw({40,10}, "Load T          : %dmicS", GM.field().cache().loadTime());

        renderer.draw({40, 12}, "Misses : %d", (int)GM.field().cache().cacheMisses());
        renderer.draw({40, 13}, "Hits   : %d", (int)GM.field().cache().cacheHits());
        renderer.draw({40, 14}, "M&S    : %d", (int)GM.field().cache().cacheCalls());
        renderer.draw({40, 15}, "M/S    : %d", (int)GM.field().cache().cacheHits() / ((int)GM.field().cache().cacheMisses() ? (int)GM.field().cache().cacheMisses() : 1));

        renderer.draw({40, 17}, "Empty    NEW/DEL : [%d] [%d]", Cell::mECellNewCount, Cell::mECellDelCount);
        renderer.draw({40, 18}, "Cell     NEW/DEL : [%d] [%d]", Cell::mCellNewCount, Cell::mCellDelCount);
        renderer.draw({40, 19}, "Neighbor NEW/DEL : [%d] [%d]", Cell::mNCellNewCount, Cell::mNCellDelCount);
        renderer.draw({40, 20}, "Bomb     NEW/DEL : [%d] [%d]", Cell::mBCellNewCount, Cell::mBCellDelCount);

        int y = 21;
        int x = 40;
        for(auto &chunk : GM.field().cache().chunksTable()) {
            renderer.draw({x, y++}, "%d (%d) [%d]=[%d]---%d:%d",
                          y - 21,
                          chunk.mIsActive,
                          chunk.mLifeTime,
                          chunk.mHits - chunk.mCalls,
                          chunk.mChunkRec.mPos.x,
                          chunk.mChunkRec.mPos.y);

        }
    }

    stf::smv::IView *keyEventsHandler(const int key) override
    {
        if(key == '`') {
            mIsConsoleShow = mIsConsoleShow ? false : true;
        }
        return stf::smv::IView::keyEventsHandler(key);
    }
};

class Game : public stf::Window
{
    GameModel mModel = GameModel();
    GameView mView = GameView(&mModel);
    stf::smv::IView *mCurrentView = &mView;
    bool isContinue = true;

public:

    Game()
    {
        enableLog();
        stf::Renderer::log.setX(0);
        stf::Renderer::log.setY(14);
        stf::Renderer::log.setHeight(14);
    }

    bool onUpdate(const float dt) override
    {
        mCurrentView = mCurrentView->update(dt);
        mView.show(renderer);
        return mModel.mLifes == 0 ? false : isContinue;
    }

    void keyEvents(const int key) override
    {
        switch (key) {
        case 'q':
            isContinue = false;
            break;
        default:
            break;
        }

        mCurrentView = mView.keyEventsHandler(key);
    }

    void mouseEvents(const stf::MouseRecord &mr) override
    {

    }
};

int main()
{
    srand(clock());
    int result = Game().run();

    printf("Empty    NEW/DEL : [%d] [%d]\n", Cell::mECellNewCount, Cell::mECellDelCount);
    printf("Cell     NEW/DEL : [%d] [%d]\n", Cell::mCellNewCount, Cell::mCellDelCount);
    printf("Neighbor NEW/DEL : [%d] [%d]\n", Cell::mNCellNewCount, Cell::mNCellDelCount);
    printf("Bomb     NEW/DEL : [%d] [%d]\n", Cell::mBCellNewCount, Cell::mBCellDelCount);

    return result;
}
