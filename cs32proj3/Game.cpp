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
    if(m_row>MAXROWS){ //defaults to maxrows and maxcols
        m_row = MAXROWS;
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
    if (newShip.m_length <= 0 && (newShip.m_length >= MAXROWS || newShip.m_length >= MAXCOLS )) //ship does not satisfy contraints.
        return false;
    //ship satisfies
    
    newShip.m_ID = static_cast<int>(m_Ships.size()); //set id of ship to index in m_ships
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
    if(!p1->placeShips(b1)){ //p1 places ships
        cout << "L" << endl;
        return nullptr; //return nullptr if failure to place
    }
    if(!p2->placeShips(b2)){ //p2 places ships
        cout << "L2" << endl;
        return nullptr; //return nullptr if failure to place
    }
    bool shotHit;
    bool shipDestroyed;
    int shipId;
    //gameplay starts
    
//    b1.display(false);
    
    
    while(!b1.allShipsDestroyed() && !b2.allShipsDestroyed()){ //game ends once all ships are destroyed for one board
        
        cout<<p1->name()<<"'s turn.  Board for "<<p2->name()<<": "<<endl; //announce p1 turn
        b2.display(p1->isHuman()); //display p2's board, if p1 is human display shots only)
        Point attackOn2 = p1->recommendAttack(); //gets attacked point through player recommendation
        if(b2.attack(attackOn2, shotHit, shipDestroyed, shipId)){ //first player attacks
            if(!shotHit) //if hit water
                cout<<p1->name()<<" attacked "<<"("<<attackOn2.r<<","<<attackOn2.c<<")"<<" and missed, resulting in: "<<endl;
            else{
                if(shipDestroyed) //if destroyed ship
                    cout<<p1->name()<<" attacked "<<"("<<attackOn2.r<<","<<attackOn2.c<<")"<<" and destroyed the "<<p2->game().shipName(shipId)<<", resulting in: "<<endl;
                else{ //if hit shit without destroying
                    cout<<p1->name()<<" attacked "<<"("<<attackOn2.r<<","<<attackOn2.c<<")"<<" and hit something, resulting in: "<<endl;
                }
            }
            p1->recordAttackResult(attackOn2, true, shotHit, shipDestroyed, shipId); //record valid attack
            p2->recordAttackByOpponent(attackOn2); //record attack from opponent
            b2.display(p1->isHuman()); //display result of attack on b2 (only shows if attack is valid)
        }
        
        else{ //invalid attack
            cout<<p1->name()<<" wasted a shot at " <<"("<<attackOn2.r<<","<<attackOn2.c<<")"<<endl;
            p1->recordAttackResult(attackOn2, false, shotHit, shipDestroyed, shipId); //record invalid attack IDK IF I NEED THIS
        }
//        b2.display(p1->isHuman()); //display result of attack on b2
        
        //if p1 destroys last ship
        if(b2.allShipsDestroyed()){ //if p2 lost
            if(p2->isHuman()) //and is human
                b1.display(false); //display board of p1
            cout<<p1->name()<<" wins!"<<endl;
            return p1; //return p1 as winner
        }
        
        
        //pause game if necessary
        if(p1->isHuman() || p2->isHuman()){
            if(shouldPause)
                waitForEnter();
        }
        else{
            if(shouldPause)
                waitForEnter();
        }
        
        //repeat for p2
        
        cout<<p2->name()<<"'s turn.  Board for "<<p1->name()<<": "<<endl;
        b1.display(p2->isHuman());
        Point attackOn1 = p2->recommendAttack();
        if(b1.attack(attackOn1, shotHit, shipDestroyed, shipId)){
            if(!shotHit) //if hit water
                cout<<p2->name()<<" attacked "<<"("<<attackOn1.r<<","<<attackOn1.c<<")"<<" and missed, resulting in: "<<endl;
            else{
                if(shipDestroyed) //if destroyed ship
                    cout<<p2->name()<<" attacked "<<"("<<attackOn1.r<<","<<attackOn1.c<<")"<<" and destroyed the "<<p1->game().shipName(shipId)<<", resulting in: "<<endl;
                else{ //if hit shit without destroying
                    cout<<p2->name()<<" attacked "<<"("<<attackOn1.r<<","<<attackOn1.c<<")"<<" and hit something, resulting in: "<<endl;
                }
            }
            p2->recordAttackResult(attackOn1, true, shotHit, shipDestroyed, shipId); //record valid attack
            p1->recordAttackByOpponent(attackOn1); //record attack from opponent
            b1.display(p2->isHuman()); //display result of attack on b1 (only shows if attack is valid)
        }
        else{ //invalid attack
            cout<<p2->name()<<" wasted a shot at " <<"("<<attackOn1.r<<","<<attackOn1.c<<")"<<endl;
            p2->recordAttackResult(attackOn1, false, shotHit, shipDestroyed, shipId); //record invalid attack IDK IF I NEED THIS
            
        }
//        b1.display(p2->isHuman()); //display result of attack on b1
        
        //if p2 destroys last ship
        if(b1.allShipsDestroyed()){ //if p1 lost
            if(p1->isHuman()) //and is human
                b2.display(false); //display board of p2
            cout<<p2->name()<<" wins!"<<endl;
            return p2; //return p2 as winner
        }
        
        //pause game if necessary
        if(p1->isHuman() || p2->isHuman()){
            if(shouldPause)
                waitForEnter();
        }
        else{
            if(shouldPause)
                waitForEnter();
        }
    }
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

