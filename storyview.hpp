#ifndef STORYVIEW_HPP
#define STORYVIEW_HPP

#include "iview.hpp"
#include "sprite.hpp"

using namespace stf;
using namespace stf::smv;

class GameModel;

class StoryView : public IView
{
  public:

  StoryView(GameModel* model);
  ~StoryView() override = default;
  void show(Renderer& renderer) final;
  IView* keyEventsHandler(const int key) override;
  IView* update(const float) override { return this; }

protected:

    stf::SpriteFromBMP m_logo;
};

#endif // STORYVIEW_HPP
