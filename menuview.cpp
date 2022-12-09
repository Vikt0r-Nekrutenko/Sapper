#include "menuview.hpp"
#include "gameview.hpp"
#include "endview.hpp"
#include "storyview.hpp"

MenuView::MenuView(GameModel* model)
  : stf::smv::IView(model),
    m_logo(stf::SpriteFromBMP("sprs/logo.bmp")),
    m_smenu(stf::Sprite("sprs/pause_menu.spr"))
{
    m_menuItemsCount = m_smenu.markers().size() / 2;
}

void  MenuView::show(stf::Renderer& renderer)
{
  IView::show(renderer);
  stf::Vec2d pzero = renderer.Size / 2 - m_smenu.Size() / 2;
  m_logo.show(renderer, true, {0,0}, {0, -m_logo.Size().y - 2});
  m_smenu.show(renderer, true);

  renderer.drawPixel(pzero + m_smenu.markers().at(m_cursor * 2), '>');
  renderer.drawPixel(pzero + m_smenu.markers().at(m_cursor * 2 + 1), '<');
}

stf::smv::IView* MenuView::keyEventsHandler(const int key)
{
  switch(key)
  {
    case 'w':
           if(m_cursor > 0)
               --m_cursor;
      else if(m_cursor == 0)
               m_cursor = m_menuItemsCount - 1;
      break;;

    case 's':
            if(m_cursor < m_menuItemsCount - 1)
                ++m_cursor;
       else if(m_cursor == m_menuItemsCount - 1)
                m_cursor = 0;
      break;
    case ' ': return menuSelectConfirm();
  }
  return this;
}

stf::smv::IView* MenuView::menuSelectConfirm()
{
    switch(m_cursor)
    {

        case 0:
            return new GameView(static_cast<GameModel*>(m_model), true);

        case 1:
            try { static_cast<GameModel*>(m_model)->saves.load();
            return new GameView(static_cast<GameModel*>(m_model));
            } catch(...) { }
        break;

        case 2:
            return new StoryView(static_cast<GameModel*>(m_model));

        case 3:
            try { static_cast<GameModel*>(m_model)->saves.save(); } catch(...) { }
            return new CloseView(static_cast<GameModel*>(m_model));
    }
    return this;
}
