#include <iostream>
#include <iview.hpp>
#include "window.hpp"
#include "imodel.hpp"
#include "chunkedmap.hpp"

class Cell : public stf::sdb::ICell
{
public:
    size_t sizeOfSelf() const override { return 0; }
    void save(FILE *file) override { return; }
    void load(FILE *file) override { return; }

    virtual uint8_t view() const = 0;
    virtual stf::ColorTable color() const = 0;
};

class Chunk : public stf::sdb::IChunk
{
public:
    static constexpr int Width  = 9;
    static constexpr int Height = 9;

    Chunk() : stf::sdb::IChunk({Width, Height}) { }
    stf::sdb::IChunk *getNew() override { return new Chunk; }
};

class GameField
{
public:
    Chunk mBegin = Chunk();
    stf::sdb::ChunkedMap mField = stf::sdb::ChunkedMap({1,1}, &mBegin, true, "sapper.schnks");
};

class GameModel : public stf::smv::BaseModel
{
public:
    GameField mField = GameField();

    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) override
    {
        return sender;
    }
};

class GameView : public stf::smv::IView
{
    GameView(GameModel *model)
        : stf::smv::IView(model) { }

public:
    void show(stf::Renderer &renderer) override
    {
        GameModel *GM = static_cast<GameModel*>(m_model);

        for(int y = 0; y < Chunk::Height; ++y) {
            for(int x = 0; x < Chunk::Width; ++x) {
                Chunk *chunk = (Chunk*)GM->mField.mField[{x,y}];
                if(chunk != nullptr) {
                    Cell *cell = (Cell*)chunk->at({x,y});
                    renderer.drawPixel({ x + 2 + 2, y + 2 }, cell->view(), cell->color());
                }
            }
        }

    }
};

class Game : public stf::Window
{
    bool isContinue = true;

public:
    bool onUpdate(const float dt) override
    {
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
    }

    void mouseEvents(const stf::MouseRecord &mr) override
    {

    }
};

int main()
{
    return Game().run();
}
