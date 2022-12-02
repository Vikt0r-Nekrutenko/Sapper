#ifndef GAMEVIEW_HPP
#define GAMEVIEW_HPP

#include "iview.hpp"
#include "gamemodel.hpp"

class GameView : public stf::smv::IView
{
    bool mIsConsoleShow = false;

public:
    GameView(GameModel *model);
    void show(stf::Renderer &renderer) override;
    stf::smv::IView *keyEventsHandler(const int key) override;

private:

    void showConsole(stf::Renderer &renderer, GameModel &GM, const stf::Vec2d &pos = {0,2});

};

#endif // GAMEVIEW_HPP
