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
        mField.update();
        mField.activate(mCursor);
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

//        mField.update();
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

        for(int j = 0, y = GM->mCursor.y - halfHeight; y <= GM->mCursor.y + halfHeight + 2; ++j, ++y) {
            for(int i = 0, x = GM->mCursor.x - halfWidth; x <= GM->mCursor.x + halfWidth+2; ++i, ++x) {
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
    return Game().run();
}
