#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>
#include <algorithm>

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
            if(counter>=50){
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


//    bool placeShipsAux(Board &b, int correctShips); //recursive algorithm for placeShips
//    bool placeIsolatedShip(Board &b, Point p, int shipLength, Direction d);
//    vector<Point> cellsOfPlacedShips;
//    int findShortestShip();
//    int findLongestShip();
//    int probe(Point p, int x, int y);
//    bool isGoodGuess(Point p, int shipLength);
//    vector<int> shipsISunk;
//    vector<int> shipsRemaining;
//bool GoodPlayer::placeIsolatedShip(Board &b, Point p, int shipLength, Direction d){ //(placeIsolatedShip(b, p, correctShips, HORIZONTAL) && (placeIsolatedShip(b, p, correctShips, VERTICAL))) &&
//    if(d == HORIZONTAL){
//        //checking top and bottom boundary of horizontal ship
//        for(int i=-1; i<2; i++){
//            Point test (p.r + i, p.c);
//            if(!b.placeShip(test, shipLength, HORIZONTAL)){ //if u cant place the same ship right next to the ship at point test, it means that there is another ship occupying that space, therefore find a new place to place a ship
//                b.display(false);
//                cout << p.r << p.c << endl;
//                return false;
//            }
//            else{
//                b.unplaceShip(test, shipLength, HORIZONTAL); //unplace test ship
//            }
//
//        }
//    }
//    if(d == VERTICAL){
//        //checking left and right boundary of vertical ship
//        for(int i=-1; i<2; i++){
//            Point test (p.r, p.c+i);
//            if(!b.placeShip(test, shipLength, VERTICAL)){ //if u cant place the same ship right next to the ship at point test, it means that there is another ship occupying that space, therefore find a new place to place a ship
//                return false;
//            }
//            else{
//                b.unplaceShip(test, shipLength, VERTICAL); //unplace test ship
//            }
//
//        }
//    }
//    b.placeShip(p, shipLength, d);
//    return true;
//}

//bool GoodPlayer::placeShipsAux(Board &b, int correctShips)
//{
//    vector<Point> badCells; //keeps track of visited cells that did not previously work
//
//
//    if(correctShips==game().nShips()) //if the number of succesfully placed ships is the number of ships return true
//        return true;
//    for(int i=0; i<game().rows(); i++){ //loop until finds placeable cell for ship
//        for(int j=0; j<game().cols(); j++){
////            Point p(i, j);
//            Point p = game().randomPoint();
//            bool pInBadCells = false;
//            for(int r=0; r<badCells.size(); r++){ //if point is in visited bad cell set pInBadCells to true
//                if(p.r == badCells[r].r && p.c == badCells[r].c)
//                    pInBadCells = true;
//            }
//
//            if(!pInBadCells &&  (b.placeShip(p, correctShips, HORIZONTAL) || b.placeShip(p, correctShips, VERTICAL))){ //if placing partially isolated ship was sucessful
//                cout << "BOARD ATTEMPT" << endl;
//                b.display(false);
//                cout << "BOARD ATTEMPT" << endl;
//
//
//                if(!placeShipsAux(b, correctShips + 1)){ //place next ship, if placement was unsucessful (returned false), unplace the ship and look for a new spot
//                    if(!b.unplaceShip(p, correctShips, HORIZONTAL))
//                       b.unplaceShip(p, correctShips, VERTICAL);
//                    badCells.push_back(p); //add current cell to badcells
//                    cout << "UNPLACE AT" << endl;
//                    b.display(false);
//                    cout << "UNPLACE AT" << endl;
//                    i=0;
//                    j=0; //research the list for possible spots
//                }
//                else{ //if next ship's placement was successful return true
//                    return true;
//                }
//            }
////            cout << "REFINDING AT "  << i << " " << j << endl;
//        }
//    }
//    return false; //if ship could not be placed anywhere return false
//}


//bool GoodPlayer::placeShips(Board &b){
//    for(int i=0; i<50; i++){ //50 attempts
////        b.block(); //block random half of board
//
//        //recursive algorithm
//        if(placeShipsAux(b, 0)){
////            b.unblock();
//            return true;
//        }
//
////        b.unblock(); //unblock board
//    }
//    return false;
//}
//NEW STATE 1*************************
//    if(state1 || potentialTargets.empty()){ //state 1
//        cout << findShortestShip() << endl;
//        cout << "I AM IN STATE ONE" << endl;
//        state1 = true; //if no more targets to attack in stack go back to state 1
//        int count = 0;
//        bool goodGuess = false;
//        do{
//            p = game().randomPoint();
//            bool goodGuess = isGoodGuess(p, findShortestShip());
//            count++;
//            if(count>=100)
//                goodGuess=true;
//        } while(!isValidAttack(p) || goodGuess); //pick random points such that it hasnt been attacked before in a checkerboard pattern
//
//        return p; //recommend p
//    }
//NEW STATE 1*************************

//"OPTIMAL PLACEMENT"
//bool GoodPlayer::placeShips(Board &b){
//    Point A (4,4);
//    b.placeShip(A, 0, HORIZONTAL);
//
//    Point B (6,3);
//    b.placeShip(B, 1, VERTICAL);
//
//    Point C (2, 1);
//    b.placeShip(C, 2, HORIZONTAL);
//
//    Point D (7, 8);
//    b.placeShip(D, 3, VERTICAL);
//
//    Point E (1, 7);
//    b.placeShip(E, 4, HORIZONTAL);
//
//    return true;
//}

//bool GoodPlayer::placeShips(Board &b){
//
//
//
//
//    for(int i=0; i<game().nShips(); i++){
//        bool isolated = true;
//        Point p;
//        do{
//            do{
//                p = game().randomPoint();
//                for(vector<Point>::iterator it = cellsOfPlacedShips.begin(); it!=cellsOfPlacedShips.end(); it++){
//                    if(abs(it->r - p.r)==1 && abs(it->c - p.c)==1){
//                        isolated = false;
//                    }
//                }
//            }while(!isolated);
//        } while(!b.placeShip(p, i, HORIZONTAL) && !b.placeShip(p, i, VERTICAL));
//        cellsOfPlacedShips.push_back(
//    }
//}

//THIS IS FOR BETTER STATE 1
//int GoodPlayer::findShortestShip(){
//    int shortest = game().shipLength(shipsRemaining[0]);
//    for(int i=0; i<shipsRemaining.size(); i++){ //loop through all ships
//        int x = game().shipLength(shipsRemaining[i]);
//        if(x<shortest){
//            shortest = x; //set shortest to length of shortest ship
//        }
//    }
//    return shortest;
//}

//THIS IS FOR BETTER STATE 2
//int GoodPlayer::findLongestShip(){
//    int longest = game().shipLength(shipsRemaining[0]);
//    for(int i=0; i<shipsRemaining.size(); i++){ //loop through all ships
//        int x = game().shipLength(shipsRemaining[i]);
//        if(x>longest){
//            longest = x; //set shortest to length of shortest ship
//        }
//    }
//    return longest;
//}



//int GoodPlayer::probe(Point p, int x, int y){
//    int freeSpots = 0;
//    Point a (p.r+x, p.c+y);
//    while(isValidAttack(a)){
//        Point b (a.r+x, a.c+y);
//        a = b;
//        freeSpots++;
////        cout << freeSpots << endl;
//    }
//    return freeSpots;
//}

//bool GoodPlayer::isGoodGuess(Point p, int shipLength){
//    int up = probe(p, 0,1);
//    int down = probe(p, 0,-1);
//    int left = probe(p, -1,0);
//    int right = probe(p, 1,0);
//    if (left + right < shipLength - 1 || up + down < shipLength - 1) {
//        return false;
//    }
//    return true;
//}
//
//{
//    if(!validShot) //if shot was not valid do nothing
//        return;
//
//    attackedCells.push_back(p); //add p to list of attacked cells
//
//
//
//    if(state1){ //state 1
//        if(!shotHit) //if shot didnt hit, stay in state 1
//            return;
//        else{
//            if(shipDestroyed) //if ship luckily destroyed, stay in state 1
//                return;
//            else{ //if shot hit and didnt destroy, set to state 2
//                state1 = false;
//                m_state2Center = p; //set point to the center of the "cross" of state 2
//                m_lastCellAttacked = p;
//                return;
//            }
//        }
//    }
//
//    else{ //state 2
//        int alreadyAttackedInHunt = 0;
//        int biggestPossibleHunt = 0;
//
//        //checking vertical hunt for already attacked cells
//        if(hunt==VERTICAL){
//            for(int i=-4; i<5; i++){
//                Point a (m_state2Center.r + i, m_state2Center.c);
//                if(game().isValid(a))
//                    biggestPossibleHunt++;
//                for(int v=0; v< attackedCells.size(); v++){
//                    if((attackedCells[v].r == m_state2Center.r + i) && (attackedCells[v].c == m_state2Center.c))
//                        alreadyAttackedInHunt++;
//                }
//            }
//        }
//
//        //checking horizontal hunt for already attacked cells, evaluating biggest possible hunt size
//        if(hunt==HORIZONTAL){
//            for(int i=-4; i<5; i++){
//                Point a (m_state2Center.r, m_state2Center.c+i);
//                if(game().isValid(a))
//                    biggestPossibleHunt++;
//                for(int v=0; v< attackedCells.size(); v++){
//                    if((attackedCells[v].c == m_state2Center.c + i) && (attackedCells[v].r == m_state2Center.r))
//                        alreadyAttackedInHunt++;
//                }
//            }
//        }
//
//
//
//        if(alreadyAttackedInHunt == biggestPossibleHunt){ //if cross is full, set state back to 1 (counts center twice so have to do 18)
//            cout << "hii" << endl;
//            biggestPossibleHunt = 0;
//            state1 = true;
//            return;
//        }
//
//        if(!shotHit){ //if shot didnt hit, stay in state 2
//            return;
//        }
//        else{
//            if(shipDestroyed){ //ship got destroyed
//                state1 = true; //switch back to state 1
//                return;
//            }
//            else{ //if shot hit and didnt destroy, stay in state 2
//
////                if(m_lastCellAttacked.c == p.c)
////                    hunt = VERTICAL;
////                else{
////                    hunt = HORIZONTAL;
////                }
////                m_lastCellAttacked = p;
//                return;
//            }
//        }
//    }
//}

////given hit, add four surrounding cells to stack of potential next (only added if not attacked before)
//bool pInAttackedCells = false;
//
////adding South Cell (if viable)
//Point S (p.r + 1, p.c);
//for(int r=0; r<attackedCells.size(); r++){ //if point is in attacked cell set pInAttackedCells to true
//    if(S.r == attackedCells[r].r && S.c == attackedCells[r].c)
//        pInAttackedCells = true;
//}
//if(game().isValid(S) && !pInAttackedCells)
//    potentialTargets.push(S);
//
////adding North Cell (if viable)
//pInAttackedCells = false;
//Point N (p.r - 1, p.c);
//for(int r=0; r<attackedCells.size(); r++){ //if point is in attacked cell set pInAttackedCells to true
//    if(N.r == attackedCells[r].r && N.c == attackedCells[r].c)
//        pInAttackedCells = true;
//}
//if(game().isValid(N) && !pInAttackedCells)
//    potentialTargets.push(N);
//
////adding East Cell (if viable)
//pInAttackedCells = false;
//Point E (p.r, p.c + 1);
//for(int r=0; r<attackedCells.size(); r++){ //if point is in attacked cell set pInAttackedCells to true
//    if(E.r == attackedCells[r].r && E.c == attackedCells[r].c)
//        pInAttackedCells = true;
//}
//if(game().isValid(E) && !pInAttackedCells)
//    potentialTargets.push(E);
//
////adding West Cell (if viable)
//pInAttackedCells = false;
//Point W (p.r, p.c - 1);
//for(int r=0; r<attackedCells.size(); r++){ //if point is in attacked cell set pInAttackedCells to true
//    if(W.r == attackedCells[r].r && W.c == attackedCells[r].c)
//        pInAttackedCells = true;
//}
//if(game().isValid(W) && !pInAttackedCells)
//    potentialTargets.push(W);
//void GoodPlayer::recordAttackByOpponent(Point p){
//    ;
//}
//                cout << "HELLO" << endl;
//                bool pInAttackedCells = false;
//
//                if(m_lastCellAttacked.r == p.r && m_lastCellAttacked.c + 1 == p.c) //east
//                    m_dir = 3;
//                if(m_lastCellAttacked.r == p.r && m_lastCellAttacked.c - 1 == p.c) //west
//                    m_dir = 2;
//                if(m_lastCellAttacked.r - 1 == p.r && m_lastCellAttacked.c == p.c) //north
//                    m_dir = 1;
//                if(m_lastCellAttacked.r + 1 == p.r && m_lastCellAttacked.c == p.c) //south
//                    m_dir = 0;
//
//                if(m_dir==0){
//                    cout << "adwada" << endl;
//                    pInAttackedCells = false;
//
////                adding South Cell (if viable)
//                Point S (p.r + 1, p.c);
//                    for(int r=0; r<attackedCells.size(); r++){ //if point is in attacked cell set pInAttackedCells to true
//                        if(S.r == attackedCells[r].r && S.c == attackedCells[r].c)
//                            pInAttackedCells = true;
//                    }
//                if(game().isValid(S) && !pInAttackedCells){
//                    cout<< "ADDING ANOTHER MF" << endl;
//                        potentialTargets.push(S);
//                }
//                }
//
//                if(m_dir==1){
//                    cout << "yooo" << endl;
//
//                    //adding North Cell (if viable)
//                    pInAttackedCells = false;
//                    Point N (p.r - 1, p.c);
//                    for(int r=0; r<attackedCells.size(); r++){ //if point is in attacked cell set pInAttackedCells to true
//                        if(N.r == attackedCells[r].r && N.c == attackedCells[r].c)
//                            pInAttackedCells = true;
//                    }
//                    if(game().isValid(N) && !pInAttackedCells)
//                        potentialTargets.push(N);
//                }
//
//                if(m_dir==3){
//                    cout << "XD" << endl;
//
//                    //adding East Cell (if viable)
//                    pInAttackedCells = false;
//                    Point E (p.r, p.c + 1);
//                    for(int r=0; r<attackedCells.size(); r++){ //if point is in attacked cell set pInAttackedCells to true
//                        if(E.r == attackedCells[r].r && E.c == attackedCells[r].c)
//                            pInAttackedCells = true;
//                    }
//                    if(game().isValid(E) && !pInAttackedCells)
//                        potentialTargets.push(E);
//                }
//
//                if(m_dir==2){
//                    //adding West Cell (if viable)
//                    pInAttackedCells = false;
//                    Point W (p.r, p.c - 1);
//                    for(int r=0; r<attackedCells.size(); r++){ //if point is in attacked cell set pInAttackedCells to true
//                        if(W.r == attackedCells[r].r && W.c == attackedCells[r].c)
//                            pInAttackedCells = true;
//                    }
//                    if(game().isValid(W) && !pInAttackedCells)
//                        potentialTargets.push(W);
//                }

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
