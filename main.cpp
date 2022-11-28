#include <iostream>
#include <string>
#include <iview.hpp>
#include "window.hpp"
#include "imodel.hpp"
#include "gamefield.hpp"

class GameModel : public stf::smv::BaseModel
{
public:
    GameField mField = GameField();
    stf::Vec2d mCursor { Chunk::Width >> 1, Chunk::Height >> 1 };

    GameModel()
    {

    }

    stf::smv::IView *put(stf::smv::IView *sender)
    {
//        mField.update();
//        mField.activate(mCursor);
        mField.onClick(mCursor);
        return sender;
    }

    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) override
    {
        switch (key)
        {
        case 'w':
            if(mCursor.y > 0)
                mCursor.y -= 1;
            else
                mCursor.y = GameField::Height * Chunk::Height - 1;
            break;

        case 'a':
            if(mCursor.x > 0)
                mCursor.x -= 1;
            else
                mCursor.x = GameField::Width * Chunk::Width - 1;
            break;

        case 's':
            if(mCursor.y < GameField::Height * Chunk::Height-1)
                mCursor.y += 1;
            else
                mCursor.y = 0;
            break;

        case 'd':
            if(mCursor.x < GameField::Width * Chunk::Width-1)
                mCursor.x += 1;
            else
                mCursor.x = 0;
            break;
        case ' ':
            return put(sender);
        }
        return sender;
    }

    stf::smv::IView *update(stf::smv::IView *sender, const float dt) override
    {
        return sender;
    }
};

class GameView : public stf::smv::IView
{
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
                Chunk *chunk = (Chunk*)GM->mField.mField[{x,y}];
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
            }
        }
        showConsole(renderer, *GM);
    }

    void showConsole(stf::Renderer &renderer, GameModel &GM)
    {
        renderer.draw({40, 2}, "Selectable: X[%d]:Y[%d]", GM.mCursor.x, GM.mCursor.y);

        renderer.draw({40,5}, "New/Del OP : [%d] [%d]", (int)GM.mField.mField.cache().newOperationsCount(), (int)GM.mField.mField.cache().deleteOperationsCount());
        renderer.draw({40,6}, "Load/Save  : [%d] [%d]", (int)GM.mField.mField.cache().loadCount(), (int)GM.mField.mField.cache().saveCount());
        renderer.draw({40,7}, "Chunks     : %d", (int)GM.mField.mField.cache().cacheSize());
        renderer.draw({40,8}, "Memory     : %fkb", (float)GM.mField.mField.cache().memUsage()/1'000.f);

        renderer.draw({40,10}, "Load T          : %dmicS", GM.mField.mField.cache().loadTime());

        renderer.draw({40, 12}, "Misses : %d", (int)GM.mField.mField.cache().cacheMisses());
        renderer.draw({40, 13}, "Hits   : %d", (int)GM.mField.mField.cache().cacheHits());
        renderer.draw({40, 14}, "M&S    : %d", (int)GM.mField.mField.cache().cacheCalls());
        renderer.draw({40, 15}, "M/S    : %d", (int)GM.mField.mField.cache().cacheHits() / ((int)GM.mField.mField.cache().cacheMisses() ? (int)GM.mField.mField.cache().cacheMisses() : 1));

        renderer.draw({40, 17}, "Empty    NEW/DEL : [%d] [%d]", Cell::mECellNewCount, Cell::mECellDelCount);
        renderer.draw({40, 18}, "Cell     NEW/DEL : [%d] [%d]", Cell::mCellNewCount, Cell::mCellDelCount);
        renderer.draw({40, 19}, "Neighbor NEW/DEL : [%d] [%d]", Cell::mNCellNewCount, Cell::mNCellDelCount);
        renderer.draw({40, 20}, "Bomb     NEW/DEL : [%d] [%d]", Cell::mBCellNewCount, Cell::mBCellDelCount);

        int y = 21;
        int x = 40;
        for(auto &chunk : GM.mField.mField.cache().chunksTable()) {
            renderer.draw({x, y++}, "%d (%d) [%d]=[%d]---%d:%d",
                          y - 21,
                          chunk.mIsActive,
                          chunk.mLifeTime,
                          chunk.mHits - chunk.mCalls,
                          chunk.mChunkRec.mPos.x,
                          chunk.mChunkRec.mPos.y);

        }
    }
};

class Game : public stf::Window
{
    GameModel mModel = GameModel();
    GameView mView = GameView(&mModel);
    bool isContinue = true;

public:
    bool onUpdate(const float dt) override
    {
        mView.update(dt);
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

        mView.keyEventsHandler(key);
    }

    void mouseEvents(const stf::MouseRecord &mr) override
    {

    }
};

int main()
{
//    srand(clock());
    return Game().run();
}
