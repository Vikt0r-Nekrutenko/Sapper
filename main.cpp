#include <iostream>
#include <string>
#include <iview.hpp>
#include "window.hpp"
#include "imodel.hpp"
#include "chunkedmap.hpp"

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

    void init(const stf::Vec2d &cursor)
    {
        int bombs = 10;
        do {
            for(int i = 0; i < Width * Height; ++i) {
                const stf::Vec2d pos { i % Width, i / Width };
                if(rand() % 100 < 10 && bombs > 0 && cursor != pos) {
                    delete at(pos);
                    put(pos, new BombCell);
                    mBombs.push_back(pos);
                    --bombs;
                }
            }
        } while(bombs > 0);

        for(stf::Vec2d &pos : mBombs) {
            for(int y = pos.y-1; y <= pos.y+1; ++y) {
                for(int x = pos.x-1; x <= pos.x+1; ++x) {
                    if(x<0 || y<0 || x > Width - 1 || y > Height - 1)
                        continue;
                    else if(x == pos.x && y == pos.y)
                        continue;
                }
            }
        }
        activate(cursor);
        mIsInitialised = true;
    }

    void activate(const stf::Vec2d &cursor)
    {
        std::list<stf::Vec2d> emptyCells;
        auto checkAround = [&](const stf::Vec2d &pos) {
            for(int y = pos.y-1; y <= pos.y+1; ++y) {
                for(int x = pos.x-1; x <= pos.x+1; ++x) {
                    if(x<0 || y<0 || x > Width - 1 || y > Height - 1)
                        continue;
                    else if(x == pos.x && y == pos.y)
                        continue;
                    else if(static_cast<Cell*>(at({x,y}))->bombsAround() == -2) {
                        delete at({x,y});
                        put({x,y}, new EmptyCell);
                        static_cast<Cell*>(at({x,y}))->activate();
                        emptyCells.push_back({x,y});
                    }
                    else if(static_cast<Cell*>(at({x,y}))->bombsAround() == -1) {
                        delete at({x,y});
                        put({x,y}, new BombsNeighborCell);
                    }
                    else if(static_cast<Cell*>(at({x,y}))->bombsAround() > 0)
                    {
                        static_cast<BombsNeighborCell*>(at({x,y}))->bombsAround()++;
                    }
                }
            }
        };
        static_cast<Cell*>(at(cursor))->activate();
        emptyCells.push_back(cursor);

        for(auto pos : emptyCells) {
            checkAround(pos);
            emptyCells.pop_front();
        }
    }

    bool isInitialised() const
    {
        return mIsInitialised;
    }

protected:

    std::vector<stf::Vec2d> mBombs;
    std::vector<stf::Vec2d> mBombsNeighbors;
    std::vector<stf::Vec2d> mEmptys;
    bool mIsInitialised = false;
};

class GameField
{
public:
    static constexpr int Width  = 1;
    static constexpr int Height = 1;

    Chunk mBegin = Chunk();
    stf::sdb::ChunkedMap mField = stf::sdb::ChunkedMap({Width,Height}, &mBegin, true, "sapper.schnks");

    Cell* put(const stf::Vec2d &pos, Cell* cell)
    {
        delete mField.at(pos);
        return static_cast<Cell*>(mField[pos]->put(pos, cell));
    }
};

class GameModel : public stf::smv::BaseModel
{
public:
    GameField mField = GameField();
    stf::Vec2d mCursor { Chunk::Width >> 1, Chunk::Height >> 1 };

    GameModel()
    {

    }

    stf::smv::IView *put(stf::smv::IView *sender)
    {
        if(!static_cast<Chunk*>(mField.mField[mCursor])->isInitialised()) {
            static_cast<Chunk*>(mField.mField[mCursor])->init(mCursor);
        } else {
            static_cast<Chunk*>(mField.mField[mCursor])->activate(mCursor);
        }
        return sender;
    }

    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) override
    {
        switch (key)
        {
        case 'w':
            if(mCursor.y > 0)
                mCursor.y -= 1;
            else
                mCursor.y = GameField::Height * Chunk::Height - 1;
            break;

        case 'a':
            if(mCursor.x > 0)
                mCursor.x -= 1;
            else
                mCursor.x = GameField::Width * Chunk::Width - 1;
            break;

        case 's':
            if(mCursor.y < GameField::Height * Chunk::Height-1)
                mCursor.y += 1;
            else
                mCursor.y = 0;
            break;

        case 'd':
            if(mCursor.x < GameField::Width * Chunk::Width-1)
                mCursor.x += 1;
            else
                mCursor.x = 0;
            break;
        case ' ':
            return put(sender);
        }
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

        constexpr int halfHeight = Chunk::Height >> 1;
        constexpr int halfWidth  = Chunk::Width  >> 1;

        for(int j = 0, y = GM->mCursor.y - halfHeight; y <= GM->mCursor.y + halfHeight; ++j, ++y) {
            for(int i = 0, x = GM->mCursor.x - halfWidth; x <= GM->mCursor.x + halfWidth; ++i, ++x) {
                Chunk *chunk = (Chunk*)GM->mField.mField[{x,y}];
                if(chunk != nullptr) {
                    Cell *cell = (Cell*)chunk->at({x,y});
                    renderer.drawPixel({ i * 2 + 1, j + 2 }, cell->view(), cell->color());
                } else {
                    renderer.drawPixel({ i * 2 + 1, j + 2 }, '.');
                }

                if(GM->mCursor != stf::Vec2d{-1,-1})
                {
                    renderer.drawPixel({halfWidth * 2,     halfHeight + 2}, '>');
                    renderer.drawPixel({halfWidth * 2 + 2, halfHeight + 2}, '<');
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
