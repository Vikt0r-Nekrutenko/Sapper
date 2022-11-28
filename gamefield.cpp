#include "gamefield.hpp"

Cell *GameField::put(const stf::Vec2d &pos, Cell *cell)
{
    delete mField.at(pos);
    return static_cast<Cell*>(mField[pos]->put(pos, cell));
}

void GameField::onClick(const stf::Vec2d &cursor)
{
    std::list<stf::Vec2d> emptyCells;

    Cell *cell = static_cast<Cell*>(mField.at(cursor));
    if(cell->uniqueIntView() == BombsNeighborCell().uniqueIntView()) {
        cell->activate();
        return;
    }
    if(cell->uniqueIntView() == BombCell().uniqueIntView()) {
        cell->activate();
        return;
    }

    emptyCells.push_back(cursor);

    for(auto pos : emptyCells) {
        for(int y = pos.y-1; y <= pos.y+1; ++y) {
            for(int x = pos.x-1; x <= pos.x+1; ++x) {

                Cell *cell = static_cast<Cell*>(mField.at({x,y}));

                if(x<0 || y<0 || x > Width * Chunk::Width - 1 || y > Height * Chunk::Height - 1)
                    continue;

                int aroundBombCount = 0;
//                if(cell->uniqueIntView() == Cell().uniqueIntView()) {

                    for(int by = pos.y - 1; by <= pos.y + 1; ++by) {
                        for(int bx = pos.x - 1; bx <= pos.x + 1; ++bx) {
                            auto cell = static_cast<Cell*>(mField.at({bx,by}));
                            if(bx<0 || by<0 || bx > Width * Chunk::Width - 1 || by > Height * Chunk::Height - 1)
                                continue;

                            if(cell->uniqueIntView() == BombCell().uniqueIntView())
                                ++aroundBombCount;
                        }
                    }

                    if(!aroundBombCount && cell->uniqueIntView() == Cell().uniqueIntView()) {
                        put({x,y}, new EmptyCell);
                        emptyCells.push_back({x,y});
                        static_cast<Cell*>(mField.at({x,y}))->activate();
                    }
//                }
                else if(aroundBombCount && cell->uniqueIntView() == BombCell().uniqueIntView()) {
                    put(pos, new BombsNeighborCell());
                    cell = static_cast<Cell*>(mField.at(pos));
                    cell->activate();

//                    for(int by = pos.y - 1; by <= pos.y + 1; ++by) {
//                        for(int bx = pos.x - 1; bx <= pos.x + 1; ++bx) {
//                            Cell *neighbor = static_cast<Cell*>(mField.at({bx,by}));

//                            if(bx<0 || by<0 || bx > Width * Chunk::Width - 1 || by > Height * Chunk::Height - 1)
//                                continue;

//                            if(neighbor->uniqueIntView() == BombCell().uniqueIntView())
//                                ++bombCount;
//                        }
//                    }
                    cell->bombsAround(cell->bombsAround() + aroundBombCount);
                }
            }
        }
    }
}
