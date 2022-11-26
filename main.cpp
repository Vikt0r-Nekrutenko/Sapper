#include <iostream>
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

class GameModel : public stf::smv::BaseModel
{
public:
    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) override
    {
        return sender;
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
