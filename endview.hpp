#ifndef ENDVIEW_HPP
#define ENDVIEW_HPP


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

class CloseView : public stf::smv::IView
{
public:

    CloseView(GameModel* model);
    ~CloseView() override = default;
    IView* update(const float) override { return this; }
    void show(stf::Renderer &) override { }
    bool isContinue() const override;
};


#endif // ENDVIEW_HPP
