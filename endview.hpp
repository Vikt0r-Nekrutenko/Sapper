#ifndef ENDVIEW_HPP
#define ENDVIEW_HPP


#include "sprite.hpp"
#include "iview.hpp"

class GameModel;

class EndView : public stf::smv::IView
{
  public:

  EndView(GameModel* model);
  ~EndView() override = default;
  void show(stf::Renderer& renderer) final;
  IView* keyEventsHandler(const int key) final;
  IView* update(const float) override { return this; }
};


#endif // ENDVIEW_HPP
