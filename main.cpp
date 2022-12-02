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

        const int halfHeight = renderer.Size.y >> 1;
        const int halfWidth  = renderer.Size.x >> 1;

        const int quarterWidth = renderer.Size.x >> 2;
        const int xBegin = GM->mCursor.x - quarterWidth;
        const int xEnd   = GM->mCursor.x + quarterWidth;

        const int yBegin = GM->mCursor.y - halfHeight;
        const int yEnd   = GM->mCursor.y + halfHeight;

        for(int j = 0, y = yBegin; y <= yEnd - 4; ++j, ++y) {
            for(int i = 0, x = xBegin; x <= xEnd - 1; ++i, ++x) {

                Chunk *chunk = (Chunk*)GM->field()[{x,y}];

                if(chunk != nullptr) {
                    Cell *cell = (Cell*)chunk->at({x,y});
                    renderer.drawPixel({ i * 2 + 1, j + 2 }, cell->view(), cell->color());
                } else {
                    renderer.drawPixel({ i * 2 + 1, j + 2 }, '.');
                }

            }
        }
        if(GM->mCursor != stf::Vec2d{-1,-1})
        {
            renderer.drawPixel({halfWidth,     halfHeight + 2}, '>', stf::ColorTable::Red);
            renderer.drawPixel({halfWidth + 2, halfHeight + 2}, '<', stf::ColorTable::Red);
        }

        // HEADER
        renderer.draw({2,1}, "Points: %d", GM->mPoints);
        renderer.draw({renderer.Size.x - 10, 1}, "Lifes: %d", GM->mLifes);
        constexpr const char gameTimeTitle[] = "Game time: %d:%d:%d:%d";
        int titleX = halfWidth - (int)(std::size(gameTimeTitle) >> 1);
        renderer.draw({titleX, 1},
                      gameTimeTitle,
                      daysFromSeconds(GM->mGameTime),
                      hoursFromSeconds(GM->mGameTime),
                      minutesFromSeconds(GM->mGameTime),
                      secondsFromSeconds(GM->mGameTime));

        // FOOTER
        constexpr const char *creditals = "Author: Viktor Nekrutenko | Date: 12.01.2022";
        renderer.draw({halfWidth - (int)(std::strlen(creditals)>>1), renderer.Size.y - 1}, "%s", creditals);

        if(mIsConsoleShow)
            showConsole(renderer, *GM);
    }

    void showConsole(stf::Renderer &renderer, GameModel &GM, const stf::Vec2d &pos = {0,2})
    {
        renderer.draw({pos.x,pos.y}, "Selectable: X[%d]:Y[%d]", GM.mCursor.x, GM.mCursor.y);

        renderer.draw({pos.x,pos.y+1}, "New/Del OP : [%d] [%d]", (int)GM.field().cache().newOperationsCount(), (int)GM.field().cache().deleteOperationsCount());
        renderer.draw({pos.x,pos.y+2}, "Load/Save  : [%d] [%d]", (int)GM.field().cache().loadCount(), (int)GM.field().cache().saveCount());
        renderer.draw({pos.x,pos.y+3}, "Chunks     : %d", (int)GM.field().cache().cacheSize());
        renderer.draw({pos.x,pos.y+4}, "Memory     : %fkb", (float)GM.field().cache().memUsage()/1'000.f);
        renderer.draw({pos.x,pos.y+5}, "Load T     : %dmicS", GM.field().cache().loadTime());

        renderer.draw({pos.x, pos.y+7}, "Misses : %d", (int)GM.field().cache().cacheMisses());
        renderer.draw({pos.x, pos.y+8}, "Hits   : %d", (int)GM.field().cache().cacheHits());
        renderer.draw({pos.x, pos.y+9}, "M&S    : %d", (int)GM.field().cache().cacheCalls());
        renderer.draw({pos.x, pos.y+10}, "M/S    : %d", (int)GM.field().cache().cacheHits() / ((int)GM.field().cache().cacheMisses() ? (int)GM.field().cache().cacheMisses() : 1));

        renderer.draw({pos.x, pos.y+12}, "Empty    NEW/DEL : [%d] [%d]", Cell::mECellNewCount, Cell::mECellDelCount);
        renderer.draw({pos.x, pos.y+13}, "Cell     NEW/DEL : [%d] [%d]", Cell::mCellNewCount, Cell::mCellDelCount);
        renderer.draw({pos.x, pos.y+14}, "Neighbor NEW/DEL : [%d] [%d]", Cell::mNCellNewCount, Cell::mNCellDelCount);
        renderer.draw({pos.x, pos.y+15}, "Bomb     NEW/DEL : [%d] [%d]", Cell::mBCellNewCount, Cell::mBCellDelCount);

        int y = pos.y + 17;
        int x = pos.x;

        for(auto &chunk : GM.field().cache().chunksTable()) {
            renderer.draw({x, y++}, " %d | (%d) | [%d] | [%d] [%d] | %d:%d",
                          y - pos.y - 17,
                          chunk.mIsActive,
                          chunk.mLifeTime,
                          chunk.mHits,
                          chunk.mCalls,
                          chunk.mChunkRec.mPos.x,
                          chunk.mChunkRec.mPos.y);

        }
    }

    stf::smv::IView *keyEventsHandler(const int key) override
    {
        if(key == '`') {
            mIsConsoleShow = mIsConsoleShow ? false : true;
        } else if(key == 'l') {
            try {
                static_cast<GameModel*>(m_model)->saves.load();
            } catch(...) {  }
        } else if(key == 'k') {
            try {
                static_cast<GameModel*>(m_model)->saves.save();
            } catch(...) {  }
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
//        enableLog();
//        stf::Renderer::log.setX(0);
//        stf::Renderer::log.setY(14);
//        stf::Renderer::log.setHeight(14);
    }

    bool onUpdate(const float dt) override
    {
        mCurrentView = mCurrentView->update(dt);
        mView.show(renderer);
        return isContinue;
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
//    srand(clock());
    int result = Game().run();

    printf("Empty    NEW/DEL : [%lld] [%lld]\n", Cell::mECellNewCount, Cell::mECellDelCount);
    printf("Cell     NEW/DEL : [%lld] [%lld]\n", Cell::mCellNewCount, Cell::mCellDelCount);
    printf("Neighbor NEW/DEL : [%lld] [%lld]\n", Cell::mNCellNewCount, Cell::mNCellDelCount);
    printf("Bomb     NEW/DEL : [%lld] [%lld]\n", Cell::mBCellNewCount, Cell::mBCellDelCount);

    return result;
}
