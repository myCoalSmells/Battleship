#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    int m_row; //stores row
    int m_col; //stores col
    
    struct Ship{ //struct that stores info for each ship
        Ship(int length, char symbol, string name) : m_length(length), m_symbol(symbol), m_name(name){}
        int m_length;
        char m_symbol;
        string m_name;
        int m_ID;
    };
    
    vector<Ship> m_Ships; //stores added ships
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols) : m_row(nRows), m_col(nCols)
{
    if(m_row>MAXROWS){
        m_row = MAXROWS; //WHAT ELSE CAN I DO HERE IF M_ROW > MAXROWS
    }
    
    if(m_col>MAXCOLS){
        m_col = MAXCOLS;
    }
}

int GameImpl::rows() const
{
    return m_row;
}

int GameImpl::cols() const
{
    return m_col;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    Ship newShip(length, symbol, name);
    if (newShip.m_length <= 0 && (newShip.m_length >= MAXROWS || newShip.m_length >= MAXCOLS )) //ship does not satisfy contraints. IS IT OK IF DIFFERENT SHIPS HAVE THE SAME SYMBOL, IF SO HAVE TO REDO BOARD::ATTACK
        return false;
    //ship satisfies
    
    newShip.m_ID = static_cast<int>(m_Ships.size()); //set id of ship to index in m_ships IS THERE ANYTHING WRONG WITH USING STATIC CAST HERE
    m_Ships.push_back(newShip); //add ship to m_ships
    return true;
}

int GameImpl::nShips() const
{
    return static_cast<int>(m_Ships.size()); //STATIC CAST OK?
}

int GameImpl::shipLength(int shipId) const
{
    return m_Ships.at(shipId).m_length; //ID of ship corresponds to index in m_Ships
}

char GameImpl::shipSymbol(int shipId) const
{
    return m_Ships.at(shipId).m_symbol; //ID of ship corresponds to index in m_Ships
}

string GameImpl::shipName(int shipId) const
{
    return m_Ships.at(shipId).m_name; //ID of ship corresponds to index in m_Ships
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    
    
    
    
    return nullptr;  // This compiles but may not be correct
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

