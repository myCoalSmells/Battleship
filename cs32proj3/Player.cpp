#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

// TODO:  You need to replace this with a real class declaration and
//        implementation.


class HumanPlayer : public Player {
public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

HumanPlayer::HumanPlayer(string nm, const Game& g) : Player(nm, g), m_lastCellAttacked(0,0){} //constructor

bool HumanPlayer::isHuman() const{
    return true; //return true because is human
}

bool HumanPlayer::placeShips(Board& b){
    cout<<name()<<" must place "<<game().nShips() <<" ships."<<endl;
    
    for(int i=0; i<game().nShips(); i++){
        b.display(false); //display player's board
        
        
        //GET DIRECTION
        cout<<"Enter h or v for direction of " << game().shipName(i) <<" (length " <<game().shipLength(i) <<"): "; //prompt direction
        string direction;
        getline(cin, direction); //get input for direction
        Direction d = HORIZONTAL; //default value in case
        while(direction[0]!='h' && direction[0]!='v'){
            cout<<"Direction must be h or v"<<endl; //reprompt
            cout<<"Enter h or v for direction of " << game().shipName(i) <<" (length " <<game().shipLength(i) <<"): ";
            getline(cin, direction); //get input for direction
        }
        
        //setting enum properly
        if(direction[0]=='h')
            d = HORIZONTAL;
        if(direction[0]=='v')
            d = VERTICAL;

        
        int r, c;
        cout<<"Enter row and column of leftmost cell (e.g. 3 5): "; //prompt coordinates
        while(!getLineWithTwoIntegers(r, c)){ //get input for coordinates
            cout<<"You must enter two integers."<<endl;
            cout<<"Enter row and column of leftmost cell (e.g. 3 5): "; //reprompt coordinates
        }
        
        while(!b.placeShip(Point(r,c), i, d)){ //while ship placement is invalid
            cout<<"The ship can not be placed there."<<endl;
            
            cout<<"Enter row and column of leftmost cell (e.g. 3 5): "; //prompt coordinates
            while(!getLineWithTwoIntegers(r, c)){ //get input for coordinates
                cout<<"You must enter two integers."<<endl;
                cout<<"Enter row and column of leftmost cell (e.g. 3 5): "; //reprompt coordinates
            }
        }
    }
    b.display(false);
    return true;
}

Point HumanPlayer::recommendAttack(){
    Point p;
    int r, c;
    cout<<"Enter row and column of leftmost cell (e.g. 3 5): "; //prompt coordinates
    while(!getLineWithTwoIntegers(r, c)){ //get input for coordinates
        cout<<"You must enter two integers."<<endl;
        cout<<"Enter row and column of leftmost cell (e.g. 3 5): "; //reprompt coordinates
    }
    p.r =r;
    p.c = c;
    return p;
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId){
    ; //human does not need this
}

void HumanPlayer::recordAttackByOpponent(Point p){
    ; //human does not need this
}




//typedef AwfulPlayer HumanPlayer;

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
//typedef AwfulPlayer MediocrePlayer;
// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.


class MediocrePlayer : public Player {
public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g) : Player(nm, g), m_lastCellAttacked(0,0){}

bool MediocrePlayer::placeShips(Board &b){
    return false;
}

Point MediocrePlayer::recommendAttack(){
    Point p(0,0);
    return p;
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId){
    ;
}

void MediocrePlayer::recordAttackByOpponent(Point p){
    ;
}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
//typedef AwfulPlayer GoodPlayer;

class GoodPlayer : public Player {
public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

GoodPlayer::GoodPlayer(string nm, const Game& g) : Player(nm, g), m_lastCellAttacked(0,0){}

bool GoodPlayer::placeShips(Board &b){
    return false;
}

Point GoodPlayer::recommendAttack(){
    Point p(0,0);
    return p;
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId){
    ;
}

void GoodPlayer::recordAttackByOpponent(Point p){
    ;
}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
