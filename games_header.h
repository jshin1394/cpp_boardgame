#ifndef HEADER
#define HEADER

#include <sstream>
#include <cstdbool>
#include <memory>
#include <string>
#include <utility>
#include <map>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <algorithm>


enum status { EXIST = 1, HIT, SUNK, MISS };

typedef std::map<std::pair<int, int>, status> Element;

//Universal parent classes
//the parent Board class shared by both battleship and checker games
class Board {
public:
    virtual void printBoard(int a) = 0;
    //virtual bool gameOn(int *turn1, int*turn2, bool *turn, std::string &input) = 0;
}; // end of Board

//the parent Piece class shared by both battleship and checker games
class Piece {
protected:
    std::pair<int, int> location;
}; // end of Piece



//Battleship game classes
class ShipPiece : public Piece {
public:
    friend class UnitTest;
    ShipPiece (std::string name, int x, int y, char hv, int space)
    {
        location = {x, y};
        direction = hv;
        this->space = space;
        this->name = name;
        setCoordinates();
    } // constructor

    Element boardspot;
    std::string& getName();
    int& getSpace();

private:
    int space;
    std::string name;
    char direction;
    void setCoordinates();
}; // end of ShipPiece class

typedef std::map<std::pair<int, int>, ShipPiece*> ShipMap;

class ShipBoard: public Board {
public:
    friend class UnitTest;
    ShipBoard(int playerNum)
    {
        this->playerNum = playerNum;
        sunkShip = 0;
        onOff = true;
    } // constructor

    ShipMap board;
    std::vector<std::shared_ptr<ShipPiece>> missed;

    bool checkSpot(ShipPiece& pieceToAdd, std::vector<ShipPiece>& ships);
    void addShipsToBoard(std::vector<ShipPiece>& ships);
    void attackedOnBoard(int x, int y);
    virtual void printBoard(int option) override;
    bool gameOn();
private:
    bool onOff;
    int sunkShip;
    int playerNum;
    bool hitOrSink(ShipPiece* ship, int x, int y);
}; // end of ShipBoard class

class BattleshipPlayer {
public:
    friend class UnitTest;
    BattleshipPlayer(int playerNum) : shipBoard(playerNum)
    {
        this->playerNum = playerNum;
        turnNum = 1;
    } // constructor

    bool addingShips();
    bool checkForValidAttack(std::string& userinput);
    void attackedOnBoard(int x, int y);
    void playerTurnMessage();
    void incrementTurn();
    void printBoard(int option);
    bool isOn ();
    void printVictoryMessage();

private:
    int turnNum;
    int playerNum;
    ShipBoard shipBoard;
    std::vector<ShipPiece> ships;
    bool checkForValidPlace(std::string& userinput);
}; // end of BattleshipPlayer class

class GameRunner {
public:
    int startBattle();
    int runBattle(BattleshipPlayer& attackingP, BattleshipPlayer& defendingP, bool& showMap);
    int runChecker();
}; // end of GameRunner class



//Checker game classes
class CheckerPiece : public Piece {
public:
    //'true' team and 'false' team
    // true team -- player 1, false team -- player 2
    CheckerPiece(bool kng, bool tm)
    {
        mking = kng;
        mteam = tm;
    } // constructor

    bool whichTeam();
    bool isKing();
    void turnKing();
    bool ifshouldCrowned(std::pair<int, int>loc);
    bool isWall(std::pair<int, int>loc); // judges wheter its next intended location
    // is out of boundary
    bool isEmpty(std::pair<int, int>loc, std::map<std::pair<int, int>, CheckerPiece*>& pieceMap);
    bool isEnemy(std::pair<int, int>loc, std::map<std::pair<int, int>, CheckerPiece*>& pieceMap);
    std::pair<int, int> getLocation();
    void moveableSpace();
    std::vector<std::pair<int, int>> getmvReach();
    std::vector<std::pair<int, int>> getjpReach();
    bool inRange(std::string userInput);
    /////////////////////all actual move is made here//////////////////////////////////
    int move(std::string userInput, std::map<std::pair<int, int>, CheckerPiece*>& pieceMap);

protected:
    //std::pair<int, int> location;
    std::map<std::pair<int, int>, CheckerPiece*>::iterator it;
    std::pair<int, int> nextLoc;
    std::pair<int, int> nextnextLoc;
    std::vector<std::pair<int, int>>
                                  mvReach; //// space reachable just by regular move, disregarding obstacles
    std::vector<std::pair<int, int>> jpReach; //// space reachable by jumping, disregarding obstacles
    bool mking;
    bool mteam;
};

class xPiece: public CheckerPiece {
public:
    xPiece(std::pair<int, int> loc) : CheckerPiece(0, 0)   //(not king, 'false' team)
    {
        location = loc;
    }
};

class oPiece: public CheckerPiece {
public:
    oPiece(std::pair<int, int> loc) : CheckerPiece(0, 1)   //(not king, 'true' team)
    {
        location = loc;
    }
};

class CheckerBoard: public Board {
public:
    friend class UnitTest;
    CheckerBoard()   // assuming American Checker is ALWAYS 8x8
    {
        bool toggle = true;
        for (int i = 0; i < 8;
                i++) { // transverse the board in horizontal direction and fills up all entries column by column
            pieceloc.first = i;
            if (toggle) {
                pieceloc.second = 0;
                pieceMap[pieceloc] = new oPiece(pieceloc);
                pieceloc.second = 2;
                pieceMap[pieceloc] = new oPiece(pieceloc);
                pieceloc.second = 6;
                pieceMap[pieceloc] = new xPiece(pieceloc);
                toggle = !toggle;
            } else {
                pieceloc.second = 1;
                pieceMap[pieceloc] = new oPiece(pieceloc);
                pieceloc.second = 5;
                pieceMap[pieceloc] = new xPiece(pieceloc);
                pieceloc.second = 7;
                pieceMap[pieceloc] = new xPiece(pieceloc);
                toggle = !toggle;
            }
        }
    } // constructor

    /////////may  be hard to read/// I tried to be very general//
    bool mustJumpList(bool turn); //check whether there is a piece that MUST jump.
    ////also, this function makes a vector of string that user MUST input, in case when mustJump is true.

    //////////////////////gameProcessor returns 1 if user inputs an invalid input, else returns 0//////////////
    ////////////////////for valid userInput, it triggers move() in piece class///////////////////////////
    int gameProcessor(std::string userInput,
                      bool turn);  //if turn==true, 'true' player's turn, else 'false' player's turn
    void printBoard(int dummy);
    void saveGame(int& player1Turn, int& player2turn, std::string& namefile);
    void loadGame(int* player1turn, int* player2turn, std::string& namefile);
    void deleteAll();
    bool gameOn(int* turn1, int* turn2, bool* turn, std::string& input);

private:
    std::map<std::pair<int, int>, CheckerPiece*> pieceMap;
    std::map<std::pair<int, int>, CheckerPiece*>::iterator it;
    std::vector<std::string> forcedList;
    std::pair<int, int> pieceloc;
}; // end of CheckerPiece class


//UnitTest class
class UnitTest {
public:
    void runBattleTest(ShipPiece& test);
    void runBattleTest(ShipBoard& test);
    void runBattleTest(BattleshipPlayer& test);
    void runCheckerUnitTest(CheckerBoard& cb);
    void runTest();

}; // end of UnitTest

#endif
