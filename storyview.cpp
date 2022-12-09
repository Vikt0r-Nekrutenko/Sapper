#include "storyview.hpp"
#include "gamemodel.hpp"
#include "menuview.hpp"
#include "time.hpp"

StoryView::StoryView(GameModel* model)
  : IView(model),
    m_logo(stf::SpriteFromBMP("sprs/logo.bmp"))
{}

void StoryView::show(Renderer& renderer)
{
  const std::string s = Time(nullptr).asString() + std::string(" %s Points: '%s' | GameTime: %d:%d:%d:%d");
  Vec2d zerop = renderer.Size / 2 - Vec2d(s.length()/2, 0);
  m_logo.show(renderer, true, {0,0}, {0, -m_logo.Size().y - 2});

  try {
      stf::sdb::Model::QueryResult *qres = static_cast<GameModel*>(m_model)->results.all();
      int k = 0;
      for(auto it = qres->begin(); it != qres->end(); ++it) {
          GameResultModel* info = qres->get<GameResultModel>(*it);

          renderer.draw(zerop + stf::Vec2d(0, k++ * 2), "%s Points: '%d' | GameTime: %d:%d:%d:%d",
                        info->mTime().asString().c_str(),
                        info->mPoints(),
                        daysFromSeconds(info->mGameTime()),
                        hoursFromSeconds(info->mGameTime()),
                        minutesFromSeconds(info->mGameTime()),
                        secondsFromSeconds(info->mGameTime()));
    }
  } catch(const std::string& ex) {
    std::string s("There are no results here yet");
    Vec2d p = renderer.Size / 2 - Vec2d(s.length() / 2, 0);
    renderer.drawText(p, s.c_str());
  }
}

IView* StoryView::keyEventsHandler(const int)
{
  return new MenuView(static_cast<GameModel*>(m_model));
}
