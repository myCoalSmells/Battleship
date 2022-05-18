#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>
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
};

HumanPlayer::HumanPlayer(string nm, const Game& g) : Player(nm, g){} //constructor

bool HumanPlayer::isHuman() const{
    return true; //return true because is human
}

bool HumanPlayer::placeShips(Board& b){ //ADD CONDITION WHERE NOT ALL SHIPS WILL FIT ON BOARD(?)
    cout<<name()<<" must place "<<game().nShips() <<" ships."<<endl;
    
    for(int i=0; i<game().nShips(); i++){
        b.display(false); //display player's board
        
        
        //get direction
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
    p.r = r;
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
    bool placeShipsAux(Board &b, int correctShips); //recursive algorithm for placeShips
    
    bool state1;
    vector<Point> attackedCells;
    Point m_state2Center; //keeps track of the point that caused transition to state 2
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g) : Player(nm, g), state1(true){}


bool MediocrePlayer::placeShipsAux(Board &b, int correctShips)
{
    vector<Point> badCells; //keeps track of visited cells that did not previously work
    
    
    if(correctShips==game().nShips()) //if the number of succesfully placed ships is the number of ships return true
        return true;
    for(int i=0; i<game().rows(); i++){ //loop until finds placeable cell for ship
        for(int j=0; j<game().cols(); j++){
            Point p(i, j);
            bool pInBadCells = false;
            for(int r=0; r<badCells.size(); r++){ //if point is in visited bad cell set pInBadCells to true
                if(p.r == badCells[r].r && p.c == badCells[r].c)
                    pInBadCells = true;
            }
            
            if(!pInBadCells && (b.placeShip(p, correctShips, HORIZONTAL) || b.placeShip(p, correctShips, VERTICAL))){ //if placing ship was sucessful

                
                if(!placeShipsAux(b, correctShips + 1)){ //place next ship, if placement was unsucessful (returned false), unplace the ship and look for a new spot
                    if(!b.unplaceShip(p, correctShips, HORIZONTAL))
                       b.unplaceShip(p, correctShips, VERTICAL);
                    badCells.push_back(p); //add current cell to badcells
                    i=0;
                    j=0; //re-search the list for possible spots
                }
                else{ //if next ship's placement was successful return true
                    return true;
                }
            }
        }
    }
    return false; //if ship could not be placed anywhere return false
}


bool MediocrePlayer::placeShips(Board &b){
    for(int i=0; i<50; i++){ //50 attempts
        b.block(); //block random half of board
        
        //recursive algorithm
        if(placeShipsAux(b, 0)){
            b.unblock();
            return true;
        }
        
        b.unblock(); //unblock board
    }
    return false;
}


Point MediocrePlayer::recommendAttack(){
    Point p;
    if(state1){ //state 1
        bool pInAttackedCells = false;
        do {
            pInAttackedCells = false;
            p = game().randomPoint(); //set p to new random point
            for(int r=0; r<attackedCells.size(); r++){ //if point is in attacked cell set pInAttackedCells to true
                if(p.r == attackedCells[r].r && p.c == attackedCells[r].c)
                    pInAttackedCells = true;
            }
        } while (pInAttackedCells); //while p is contained within attacked cell set
        
        return p; //recommend p
    }
    
    else{ //state 2
        
        bool pInAttackedCells = false;
        bool pInCross = false;
        
        do{
            pInAttackedCells = false;
            pInCross = false;
            p = game().randomPoint(); //pick new random point
            for(int r=0; r<attackedCells.size(); r++){ //if point is in attacked cell set pInAttackedCells to true
                if(p.r == attackedCells[r].r && p.c == attackedCells[r].c)
                    pInAttackedCells = true;
            }
            if(((abs(m_state2Center.r - p.r) <= 4) && m_state2Center.c == p.c) || ((abs(m_state2Center.c - p.c) <= 4) && m_state2Center.r == p.r)){ //if p is in the attack cross
                pInCross = true;
            }
            
        } while(pInAttackedCells || !pInCross); //while p is contained within attacked cell set and while p is not in the cross
        
        return p;
    }

    return p;
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId){
    if(!validShot) //if shot was not valid do nothing
        return;

    attackedCells.push_back(p); //add p to list of attacked cells
    
    if(state1){ //state 1
        if(!shotHit) //if shot didnt hit, stay in state 1
            return;
        else{
            if(shipDestroyed) //if ship luckily destroyed, stay in state 1
                return;
            else{ //if shot hit and didnt destroy, set to state 2
                state1 = false;
                m_state2Center = p; //set point to the center of the "cross" of state 2
                return;
            }
        }
    }
    
    if(!state1){ //state 2
        int alreadyAttackedInCross = 0;
        int biggestPossibleCross = 0;
        
        //checking rows of cross for already attacked cells
        for(int i=-4; i<5; i++){
            Point a (m_state2Center.r + i, m_state2Center. c);
            if(game().isValid(a))
                biggestPossibleCross++;
            for(int v=0; v< attackedCells.size(); v++){
                if((attackedCells[v].r == m_state2Center.r + i) && (attackedCells[v].c == m_state2Center.c))
                    alreadyAttackedInCross++;
                }
        }
        
        //checking columns of cross for already attacked cells
        for(int i=-4; i<5; i++){
            Point a (m_state2Center.r, m_state2Center.c+i);
            if(game().isValid(a))
                biggestPossibleCross++;
            for(int v=0; v< attackedCells.size(); v++){
                if((attackedCells[v].c == m_state2Center.c + i) && (attackedCells[v].r == m_state2Center.r))
                    alreadyAttackedInCross++;
                }
        }
        
        if(alreadyAttackedInCross == biggestPossibleCross){ //if cross is full, set state back to 1 (counts center twice so have to add 1)
            state1 = true;
            return;
        }
        
        
        if(!shotHit) //if shot didnt hit, stay in state 2
            return;
        else{
            if(shipDestroyed){ //ship got destroyed
                state1 = true; //switch back to state 1
                return;
            }
            else{ //if shot hit and didnt destroy, stay in state 2
                return;
            }
        }
        
    }
}

void MediocrePlayer::recordAttackByOpponent(Point p){
    ; //does nothing for mediocre player
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
    vector<Point> attackedCells;
    Point m_state2Center;
    bool isValidAttack(Point p);
    bool state1;
    stack<Point> potentialTargets;
    void pushPotentialTargets(int dir, Point p);
    int m_dir; //0 = vert, 1 = hor
};

GoodPlayer::GoodPlayer(string nm, const Game& g) : Player(nm, g), state1(true), m_dir(0){}



bool GoodPlayer::placeShips(Board &b){ //places ships in alternate directions in random places
    Direction d;
    for(int i=0; i<game().nShips(); i++){ //place ships on game
        Point p;
        int counter = 0;
        if(i%2==0){ //alternate directions
            d = HORIZONTAL;
        }
        else{
            d = VERTICAL;
        }

        do{
            counter++;
            if(counter==50) //fails if 50 attempts
                return false;
            p = game().randomPoint();
        } while(!b.placeShip(p, i, d)); //place random ship s.t. it fits on board
    }
    return true;
}


bool GoodPlayer::isValidAttack(Point p){ //returns if attack will be valid
    if(!game().isValid(p))
        return false;
    for(int r=0; r<attackedCells.size(); r++){ //if point is in attacked cell set pInAttackedCells to true
        if(p.r == attackedCells[r].r && p.c == attackedCells[r].c)
            return false;;
    }
    return true;

}

void GoodPlayer::pushPotentialTargets(int dir, Point p){
    if(dir==0){ //vertical
        //adding South Cell (if viable)
        Point S (p.r + 1, p.c);
        if(isValidAttack(S))
            potentialTargets.push(S);
        
        //adding North Cell (if viable)
        Point N (p.r - 1, p.c);
        if(isValidAttack(N))
            potentialTargets.push(N);
    }
    if(dir==1){ //horizontal
        //adding East Cell (if viable)
        Point E (p.r, p.c + 1);
        if(isValidAttack(E))
            potentialTargets.push(E);
        
        //adding West Cell (if viable)
        Point W (p.r, p.c - 1);
        if(isValidAttack(W))
            potentialTargets.push(W);
    }
    


}

Point GoodPlayer::recommendAttack(){ //two states, state1: go along diagonals. state 2: pick direction from hit, and if X, go along that direction largest ship size (basically cross but u pick one direction)
    Point p;
    
    if(state1 || potentialTargets.empty()){ //state 1
        state1 = true; //if no more targets to attack in stack go back to state 1
        int counter = 0;
        bool isEven = false;
        do{
            p = game().randomPoint();
            isEven = (p.r + p.c)%2 == 0;
            counter++;
            if(counter>=MAXROWS*MAXCOLS/2){
                isEven = true; //if checkerboard is filled, any random point is fine
            }
        } while(!isValidAttack(p) || !isEven); //pick random points such that it hasnt been attacked before in a checkerboard pattern

        return p; //recommend p
    }

    else{ //state 2
        p = potentialTargets.top(); //attack point on top of the stack
        potentialTargets.pop();
        return p;
    }
    return p;
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId){
    if(!validShot) //if shot was not valid do nothing
        return;

    attackedCells.push_back(p); //add p to list of attacked cells
    
    if(state1){ //state 1
        if(!shotHit) //if shot didnt hit, stay in state 1
            return;
        else{
            if(shipDestroyed){ //if ship luckily destroyed, stay in state 1
                return;
            }
            else{ //if shot hit and didnt destroy, set to state 2
                state1 = false;
                //push 4 surrounding cells
                m_dir=0;
                pushPotentialTargets(m_dir, p);
                m_dir=1;
                pushPotentialTargets(m_dir, p);
                m_state2Center = p;
                return;
            }
        }
    }
    
    else{ //state 2
        if(!shotHit) //if shot didnt hit, stay in state 2
            return;
        else{
            if(shipDestroyed){ //ship got destroyed
                state1 = true; //switch back to state 1
                while(!potentialTargets.empty()){ //clear potential targets
                    potentialTargets.pop();
                }
                return;
            }
            else{ //if shot hit and didnt destroy, stay in state 2
                //determine direction of the hit based on its position in the context of the original hit
                if(m_state2Center.c == p.c){ //vertical
                    m_dir = 0;
                }
                else if(m_state2Center.r == p.r){ //horizontal
                    m_dir = 1;
                }
                pushPotentialTargets(m_dir, p); //push new potential targets
                return;
            }
        }
        
    }
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
