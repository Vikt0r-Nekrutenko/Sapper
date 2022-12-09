#include <iostream>
#include <string>
#include <iview.hpp>
#include "window.hpp"
#include "gamemodel.hpp"
#include "menuview.hpp"


class Game : public stf::Window
{
    GameModel mGameModel;
    MenuView mView = MenuView(&mGameModel);
    stf::smv::IView *mCurrentView = &mView;

public:

    Game()
    {
//        enableLog();
//        stf::Renderer::log.setX(0);
//        stf::Renderer::log.setY(14);
//        stf::Renderer::log.setHeight(14);
    }

    ~Game()
    {
//        delete mModel;
    }

    bool onUpdate(const float dt) override
    {
        mCurrentView = mCurrentView->update(dt);
        mCurrentView->show(renderer);
        return mCurrentView->isContinue();
    }

    void keyEvents(const int key) override
    {
        mCurrentView = mCurrentView->keyEventsHandler(key);
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
