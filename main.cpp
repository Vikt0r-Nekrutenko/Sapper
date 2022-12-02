#include <iostream>
#include <string>
#include <iview.hpp>
#include "window.hpp"
#include "gameview.hpp"


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
