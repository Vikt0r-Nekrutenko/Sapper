#include <iostream>
#include <string>
#include <iview.hpp>
#include "window.hpp"
#include "imodel.hpp"
#include "chunkedmap.hpp"

class Cell : public stf::sdb::ICell
{
public:
    size_t sizeOfSelf() const override
    {
        return sizeof(mView) + sizeof(mColor);
    }

    void save(FILE *file) override
    {
        fwrite(&mView, sizeof(mView), 1, file);
        fwrite(&mColor, sizeof(mColor), 1, file);
    }

    void load(FILE *file) override
    {
        fread(&mView, sizeof(mView), 1, file);
        fread(&mColor, sizeof(mColor), 1, file);
    }

    virtual uint8_t view() const
    {
        return mView;
    }

    virtual stf::ColorTable color() const
    {
        return mColor;
    }

protected:

    uint8_t mView = '-';
    stf::ColorTable mColor = stf::ColorTable::Default;
};

class Chunk : public stf::sdb::IChunk
{
public:
    static constexpr int Width  = 9;
    static constexpr int Height = 9;

    Chunk() : stf::sdb::IChunk({Width, Height})
    {
        mArray.resize(Width * Height);
        for(auto &cell : mArray) {
            cell = new Cell;
        }
    }

    stf::sdb::IChunk *getNew() override
    {
        return new Chunk;
    }
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
    stf::Vec2d mCursor { -1, -1 };

    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) override
    {
        return sender;
    }
};

class GameView : public stf::smv::IView
{
public:
    GameView(GameModel *model)
        : stf::smv::IView(model) { }

    void show(stf::Renderer &renderer) override
    {
        GameModel *GM = static_cast<GameModel*>(m_model);

        for(int y = 0; y < Chunk::Height; ++y) {
            for(int x = 0; x < Chunk::Width; ++x) {
                Chunk *chunk = (Chunk*)GM->mField.mField[{x,y}];
                if(chunk != nullptr) {
                    Cell *cell = (Cell*)chunk->at({x,y});
                    renderer.drawPixel({ x * 2 + 1, y + 2 }, cell->view(), cell->color());
                }
            }
        }

    }
};

class Game : public stf::Window
{
    GameModel mModel = GameModel();
    GameView mView = GameView(&mModel);
    bool isContinue = true;

public:
    bool onUpdate(const float dt) override
    {
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

        mView.keyEventsHandler(key);
    }

    void mouseEvents(const stf::MouseRecord &mr) override
    {

    }
};

int main()
{
    return Game().run();
}
