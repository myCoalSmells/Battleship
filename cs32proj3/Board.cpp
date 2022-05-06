#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    const Game& m_game;
    char m_board[MAXROWS][MAXCOLS]; //keeps track of the visual board
    int m_row; //DO I NEED THESE?
    int m_col;
    vector <int> m_shipIDs; //keeps track of the shipIDs
    vector <int> m_sunkShips; //keeps track of the sunken ships
    
};

BoardImpl::BoardImpl(const Game& g)
: m_game(g), m_row(g.rows()), m_col(g.cols()) //set rows and cols to how they were intialized in game
{
    //fill board with '.'
    for(int i=0; i<m_game.rows(); i++){
        for(int j=0; j<m_game.cols(); j++){
            m_board[i][j] = '.';
        }
    }

}

void BoardImpl::clear()
{
    //fill board with '.'
    for(int i=0; i<m_game.rows(); i++){
        for(int j=0; j<m_game.cols(); j++){
            m_board[i][j] = '.';
        }
    }
    
    //clear vectors that store ships
    m_shipIDs.clear();
    m_sunkShips.clear();
}

void BoardImpl::block() //redo this with random position function
{
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
                m_board[r][c] = '#'; //# will signify a blocked spot
            }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if(m_board[r][c]=='#'){
                m_board[r][c]='.'; //if blocked, unblock back to '.'
            }
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if(shipId > 0) //check if shipID is valid FIX THIS
        return false;
    
    if(find(m_shipIDs.begin(), m_shipIDs.end(), shipId) != m_shipIDs.end()) //return false if m_shipIDs already contains this ship using find algorithm
        return false;
    
    if(!m_game.isValid(topOrLeft)) //return false if point given is outside of board
        return false;
    
    if(dir==HORIZONTAL){
        if(m_game.shipLength(shipId) + topOrLeft.r > m_game.rows()) //return false if horizontal ship will be outside of board
            return false;
        
        for(int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++){ //return false if horizontal ship will overlap other ship or blocked position
            if(m_board[i][topOrLeft.c]!='.'){
                return false;
            }
        }
        
        for(int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++){ //place ship on board
            m_board[i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
        m_shipIDs.push_back(shipId); //add ship to m_shipIDs
        return true;
    }
    
    if(dir==VERTICAL){
        if(m_game.shipLength(shipId) + topOrLeft.c > m_game.cols()) //return false if vertical ship will be outside of board
            return false;
        
        for(int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++){ //return false if vertical ship will overlap other ship or blocked position
            if(m_board[topOrLeft.r][i]!='.'){
                return false;
            }
        }
        
        for(int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++){ //place ship on board
            m_board[topOrLeft.r][i] = m_game.shipSymbol(shipId);
        }
        m_shipIDs.push_back(shipId); //add ship to m_shipIDs
        return true;
    }
    return false; //return false if not vertical or horizontal
}
    

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if(shipId > 0) //check if shipID is valid FIX THIS
        return false;
    
    if(!m_game.isValid(topOrLeft)) //return false if point given is outside of board
        return false;
    
    if(find(m_shipIDs.begin(), m_shipIDs.end(), shipId) == m_shipIDs.end()) //return false if m_shipIDs does not contain this ship using find algorithm
        return false;
    
    //should not have to check if ship goes off board because from previous coniditional, if ship is in m_shipIDs, it must have been placed correctly. THIS ACTUALLY MAY BE WRONG
    
    if(dir==HORIZONTAL){
        if(m_game.shipLength(shipId) + topOrLeft.r > m_game.rows()) //return false if horizontal ship is outside of board
            return false;
        
        for(int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++){ //return false if entire horizontal ship is not there
            if(m_board[i][topOrLeft.c]!=m_game.shipSymbol(shipId)){
                return false;
            }
        }
        
        for(int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++){ //remove horizontal ship on board
            m_board[i][topOrLeft.c] = '.';
        }
        
//        m_shipIDs.erase(remove(m_shipIDs.begin(), m_shipIDs.end(), shipId), m_shipIDs.end()); //remove ship from m_shipIDs
        return true;
    }
    
    if(dir==VERTICAL){
        if(m_game.shipLength(shipId) + topOrLeft.c > m_game.cols()) //return false if vertical ship is outside of board
            return false;
        
        for(int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++){ //return false if entire ship is not there
            if(m_board[topOrLeft.r][i]!=m_game.shipSymbol(shipId)){
                return false;
            }
        }
        
        for(int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++){ //remove vertical ship on board
            m_board[topOrLeft.r][i] = '.';
        }
        
//        m_shipIDs.erase(remove(m_shipIDs.begin(), m_shipIDs.end(), shipId), m_shipIDs.end()); //remove ship from m_shipIDs
        return true;
    }
    
    return false; //return false if not vertical or horizontal
}

void BoardImpl::display(bool shotsOnly) const
{
    cout << "  "; //2 spaces
    for(int i=0; i<m_game.cols(); i++){
        cout << i; //column indicators
    }
    cout << '\n'; //new line
    
    for(int i=0; i<m_game.rows(); i++){
        cout << i << " "; //row indicator + space
        for(int j=0; j<m_game.cols(); j++){
            if(shotsOnly){
                if(m_board[i][j]!='X' || m_board[i][j]!='o')
                    cout << "."; //board contents of shots only, otherwise just '.'
                else{
                    cout << m_board[i][j]; //print out shots
                }
            }
            else{
                cout << m_board[i][j]; //board contents that displays everything
            }
        }
        cout << '\n'; //new row
    }
    
    
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    
    return false; // This compiles, but may not be correct
}

bool BoardImpl::allShipsDestroyed() const
{
    if(m_sunkShips.size() == m_shipIDs.size()) //if the number of sunk ships is equal to the number of added ships, all ships are destroyed
        return true;
    return false;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
