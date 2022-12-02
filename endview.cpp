#include "endview.hpp"
#include "gamemodel.hpp"

EndView::EndView(GameModel *model)
    : stf::smv::IView(model) { }

void EndView::show(stf::Renderer &renderer)
{
    GameModel *GM = static_cast<GameModel*>(m_model);

    const int hw = renderer.Size.x >> 1;
    const int hh = renderer.Size.y >> 1;

    if(GM->points() == GM->TotalBombs)
        renderer.draw({hw - 8, hh}, "CONGRATULATIONS!");
    else
        renderer.draw({hw - 5, hh}, "GAME  OVER");

    renderer.draw({hw - 5, hh+1}, "Points: %d", GM->points());
    renderer.draw({hw - 5, hh+2}, "Lifes:  %d", GM->lifes());
    renderer.draw({hw - 10, hh+3}, "Game time:  %d%d%d%d",
                  daysFromSeconds(GM->gameTime()),
                  hoursFromSeconds(GM->gameTime()),
                  minutesFromSeconds(GM->gameTime()),
                  secondsFromSeconds(GM->gameTime()));
}

stf::smv::IView *EndView::keyEventsHandler(const int)
{
    return new CloseView(static_cast<GameModel*>(m_model));
}

bool CloseView::isContinue() const
{
    return false;
}
