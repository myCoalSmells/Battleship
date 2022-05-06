#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED

#include "globals.h"

class Game;
class BoardImpl;

class Board
{
  public:
    Board(const Game& g);
    ~Board();
    void clear(); //get rid of all shups on board
    void block(); //make certain positions impossible to place item
    void unblock(); //undoes above
    bool placeShip(Point topOrLeft, int shipId, Direction dir); //places ship onto board
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir); //removes shiip
    void display(bool shotsOnly) const; //displays ship accordingly
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId); //attack a point on opponents board return if attack is valid. shothit becomes true if hit shipdestroyed to true if ship was destroyed, shipId is set to the ship that was destroeyd
    bool allShipsDestroyed() const; //if all ships destroyed
      // We prevent a Board object from being copied or assigned
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

  private:
    BoardImpl* m_impl;
};

#endif // BOARD_INCLUDED
