#ifndef MENUVIEW_HPP
#define MENUVIEW_HPP

#include "sprite.hpp"
#include "iview.hpp"

class GameModel;

class MenuView : public stf::smv::IView
{
  public:

  MenuView(GameModel* model);
  ~MenuView() override = default;
  void show(stf::Renderer& renderer) override;
  stf::smv::IView* keyEventsHandler(const int key) override;
  stf::smv::IView* update(const float) override { return this; }

  protected:

  stf::SpriteFromBMP m_logo;
  stf::Sprite m_smenu;
  int m_cursor = 0;
  int m_menuItemsCount;

  virtual IView* menuSelectConfirm();
};


#endif // MENUVIEW_HPP
