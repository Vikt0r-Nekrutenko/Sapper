#include <iostream>
#include "window.hpp"
#include "imodel.hpp"
#include "chunkedmap.hpp"

class Cell : public stf::sdb::ICell
{
public:
    size_t sizeOfSelf() const override;
    void save(FILE *file) override;
    void load(FILE *file) override;
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
