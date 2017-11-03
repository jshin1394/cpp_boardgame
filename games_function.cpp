#include "games_header.h"
#include <cassert>


////////////////////////////GameRunner functions///////////////////////////////
//returns 0 if no occurrence of runtime error, 1 otherwise
int GameRunner::startBattle()
{
    std::cout << "Battleship" << std::endl;

    BattleshipPlayer p1(1);
    BattleshipPlayer p2(2);

    //placing ships
    try {
        p1.addingShips();
    } catch (std::exception& e) {
        return 1;
    }

    try {
        p2.addingShips();
    } catch (std::exception& e) {
        return 1;
    }

    //attack strts
    std::cout << "Begin game" << std::endl;

    bool showMap = false;
    while (p1.isOn() && p2.isOn()) {
        if (runBattle(p1, p2, showMap) == 1) {
            return 1;
        }
        if (p2.isOn() ==  false) {
            p1.printVictoryMessage();
            continue;
        }
        if (runBattle(p2, p1, showMap) == 1) {
            return 1;
        }
        if (p1.isOn() ==  false) {
            p2.printVictoryMessage();
            continue;
        }
    } // end of while loop
    return 0;
} // end of startBattle method

//returns 0 if no occurrence of runtime error, 1 otherwise
int GameRunner::runBattle(BattleshipPlayer& attackingP, BattleshipPlayer& defendingP, bool& showMap)
{
    //checking validity of attackingP's  move
    bool valid = false;
    std::string userinput;
    while (!valid) {
        attackingP.playerTurnMessage();

        //toggle map
        if (showMap == true) {
            std::cout << "      Attack Board" << std::endl;
            defendingP.printBoard(MISS);
            std::cout << "\n";
            std::cout << "      Ship Placement" << std::endl;
            attackingP.printBoard(EXIST);
        }

        std::cin >> userinput;

        try {
            if (userinput[0] == 'q' || userinput[0] == 'Q') {
                throw std::runtime_error ("");
            }
        } catch (std::exception& e) {
            return 1;
        }

        //make this a separate function somewhere
        if (userinput[0] == 'b' || userinput[0] == 'B') {
            if (showMap == true) {
                showMap = false;
            } else if (showMap == false) {
                showMap = true;
            }
            continue;
        }

        valid = attackingP.checkForValidAttack(userinput);
        if (!valid) {
            std::cout << "Invalid move" << std::endl;
        }
    } // end of while

    int x = userinput[0] - '0';
    int y = userinput[1] - '0';

    defendingP.attackedOnBoard(x, y);
    attackingP.incrementTurn();

    return 0;
} //end of method


int GameRunner::runChecker()
{
    CheckerBoard board;

    //char userInput[10]; //assuming that the max length of userinput =10
    std::string userInput;


    bool turn = true;  //true team (player 1) starts first
    bool boardOn = false;
    int player1Turn = 1;
    int player2Turn = 1;

    std::cout << "Checkers" << std::endl;


    while (board.gameOn(&player1Turn, &player2Turn, &turn,
                        userInput)) { //gameON()==true means that game is ongoing. else the game is over

        if (turn) {
            printf("Player 1 turn %d:\n", player1Turn);
        } else {
            printf("Player 2 turn %d\n", player2Turn);
        }

        if (boardOn) {
            //receives a dummy integer variable
            board.printBoard(0);
        }

        std::string filename;
        getline(std::cin >> userInput, filename);

        ///////////takes in userInput for next move (ex. 43r)
        /// also, just in case the player wants to save/load game, it also gets
        ////another string cin
        filename.erase(remove_if(filename.begin(), filename.end(), isspace), filename.end());


        ////////beside 'b', 'l', 'surrender', all the other userInput will be thought as signals for the next move////////////
        if (userInput[0] == 'b') {
            boardOn = !boardOn;
        } else if ((userInput.size() == 1) && (userInput[0] == 's')) {
            board.saveGame(player1Turn, player2Turn, filename);
        } else if (userInput[0] == 'l') {
            board.loadGame(&player1Turn, &player2Turn, filename);
            // assuming that player1 always starts first,
            if (player1Turn > player2Turn) {
                turn = false;
            } else {
                turn = true;
            }
        } else if (userInput[0] == 'q') {
            board.deleteAll();
            return 1;
        } else if (userInput.compare("surrender") == 0) {
            continue; // suurrender message that went into userInput will have its effect on the next loop. It will be picked up by gameON()
        } else {
            /// rest of the userInput messages now go into the gameProcessor for the next move.
            if (board.gameProcessor(userInput,
                                    turn) != 1) { ////if the input is valid// input is processed if valid
                if (board.mustJumpList(
                            turn)) { // but  there is a piece that must jump, and user didn't take that option// rejected
                    continue;
                } else {            //// valid input, and there is not piece that must jump ///user's input is accepted
                    if (turn) {
                        player1Turn++;
                    } else {
                        player2Turn++;
                    }
                    turn = !turn; ////if no further jump is available && if a valid input was made, next turn
                }
            }
        } // end of if/elseif's

    } // end of while

    return 0;

} // end of runChecker method



////////////////////////// Checker functions ///////////////////////////////////////////////////////

bool CheckerPiece::whichTeam()
{
    return mteam;
}

bool CheckerPiece::isKing()
{
    return mking;
}

void CheckerPiece::turnKing()
{
    mking = true;
}

bool CheckerPiece::ifshouldCrowned(std::pair<int, int>loc)
{
    if (whichTeam()) { //////team 1////
        if (loc.second == 7) { ////reached end
            return true;
        } else {
            return false;
        }
    } else { /////////team 2///////////
        if (loc.second == 0) {
            return true;
        } else {
            return false;
        }

    }

}


bool CheckerPiece::isWall(std::pair<int, int>loc) // judges wheter its next intended location
{
    // is out of boundary

    return (loc.first < 0 || loc.first > 7) || (loc.second < 0 || loc.second > 7);
}

bool CheckerPiece::isEmpty(std::pair<int, int>loc,
                           std::map<std::pair<int, int>, CheckerPiece*>& pieceMap)
{

    return (pieceMap.count(loc)) == 0;

}

bool CheckerPiece::isEnemy(std::pair<int, int>loc,
                           std::map<std::pair<int, int>, CheckerPiece*>& pieceMap)
{
    if (isWall(loc) || isEmpty(loc, pieceMap)) {
        return false;
    } else {
        return (pieceMap[loc])->whichTeam() != whichTeam();
    }

}



std::pair<int, int> CheckerPiece::getLocation()
{
    return location;
}


void CheckerPiece::moveableSpace()  ///gives a list of space that a given piece can move to (including both regular move and jump move), disregarding obstacles
{

    mvReach.clear();////space reachable by move
    mvReach.shrink_to_fit(); // making a brand new empty vector
    jpReach.clear();  ///space reachable by jump
    jpReach.shrink_to_fit();

    std::pair<int, int> upleft (location.first - 1, location.second + 1);
    std::pair<int, int> upleft2 (location.first - 2, location.second + 2);

    std::pair<int, int> upright (location.first + 1, location.second + 1);
    std::pair<int, int> upright2 (location.first + 2, location.second + 2);

    std::pair<int, int> downleft (location.first - 1, location.second - 1);
    std::pair<int, int> downleft2 (location.first - 2, location.second - 2);

    std::pair<int, int> downright (location.first + 1, location.second - 1);
    std::pair<int, int> downright2 (location.first + 2, location.second - 2);



    if (whichTeam() && (!isKing())) { //player1's regular piece.. can only move up right or up left

        /////start with  up right move////
        mvReach.push_back(upright);
        jpReach.push_back(upright2);
        /////start with  up left move////
        mvReach.push_back(upleft);
        jpReach.push_back(upleft2);

    } else if (!whichTeam() &&
               (!isKing())) { //player2's regular piece.. can only move down right or down left
        /////start with  down right move////
        mvReach.push_back(downright);
        jpReach.push_back(downright2);
        /////start with  down left move////
        mvReach.push_back(downleft);
        jpReach.push_back(downleft2);


    } else if (isKing()) { /// can move in all directions
        /////start with  up right move////
        mvReach.push_back(upright);
        jpReach.push_back(upright2);
        /////  up left move////
        mvReach.push_back(upleft);
        jpReach.push_back(upleft2);
        /////  down right move////
        mvReach.push_back(downright);
        jpReach.push_back(downright2);
        /////down left move////
        mvReach.push_back(downleft);
        jpReach.push_back(downleft2);

    }


}

std::vector<std::pair<int, int>> CheckerPiece::getmvReach()
{
    return mvReach;
}

std::vector<std::pair<int, int>> CheckerPiece::getjpReach()
{
    return jpReach;
}

bool CheckerPiece::inRange(std::string userInput)
{
    if ((userInput[0] >= (0 + '0')) && (userInput[0] <= (7 + '0')) &&
            (userInput[1] >= (0 + '0')) && (userInput[1] <= (7 + '0'))) {

        return true;
    } else {
        return false;
    }
} // end of inRange method

/////////////////////all actual move is made here//////////////////////////////////
int CheckerPiece::move(std::string userInput,
                       std::map<std::pair<int, int>, CheckerPiece*>& pieceMap)
// takes in userinput and current board situation
{
    moveableSpace(); //////////generates a list of movements that can be made. See the function above.

    ///NOTE//// user cannot CHOOSE to jump over an enemy directly with his input. It will be done AUTOMATICALLY, since user CANNOT chose
    ///not to jump when it can. In other words, to the user, there will be no instance when an opponent piece exists at a
    /// spot attackable by an user piece.
    //nxtnxtloc is used for jumping over
    if (isKing()) { ////////////////king command is same for player1 and player 2
        if (inRange(userInput) && (userInput.at(2) == 't') && (userInput.at(3) == 'r')) { // tr
            nextLoc.first = (mvReach[0]).first;
            nextLoc.second = (mvReach[0]).second;
            nextnextLoc.first = (jpReach[0]).first;
            nextnextLoc.second = (jpReach[0]).second;
        } else if (inRange(userInput) && (userInput.at(2) == 't') && (userInput.at(3) == 'l')) { // tl
            nextLoc.first = (mvReach[1]).first;
            nextLoc.second = (mvReach[1]).second;
            nextnextLoc.first = (jpReach[1]).first;
            nextnextLoc.second = (jpReach[1]).second;
        } else if (inRange(userInput) && (userInput.at(2) == 'b') && (userInput.at(3) == 'r')) { // br
            nextLoc.first = (mvReach[2]).first;
            nextLoc.second = (mvReach[2]).second;
            nextnextLoc.first = (jpReach[2]).first;
            nextnextLoc.second = (jpReach[2]).second;
        } else if (inRange(userInput) && (userInput.at(2) == 'b') && (userInput.at(3) == 'l')) { // bl
            nextLoc.first = (mvReach[3]).first;
            nextLoc.second = (mvReach[3]).second;
            nextnextLoc.first = (jpReach[3]).first;
            nextnextLoc.second = (jpReach[3]).second;
        } else {
            puts("Invalid move");
            return 1;
        }

    } else { //////////////////////////for player1 and 2, right and left are same. left/right is in perspecitve of player 1.

        if (inRange(userInput) && (userInput.at(2) == 'r')) { // right move
            nextLoc.first = (mvReach[0]).first;
            nextLoc.second = (mvReach[0]).second;
            nextnextLoc.first = (jpReach[0]).first;
            nextnextLoc.second = (jpReach[0]).second;
        } else if (inRange(userInput) && (userInput.at(2) == 'l')) { //  left move
            nextLoc.first = (mvReach[1]).first;
            nextLoc.second = (mvReach[1]).second;
            nextnextLoc.first = (jpReach[1]).first;
            nextnextLoc.second = (jpReach[1]).second;

        } else {
            puts("Invalid move");
            return 1;
        }

    }


    if (isWall(nextLoc)) {
        puts("Invalid move"); ///out of bound//
        return 1;
    } else if (pieceMap.count(nextLoc)) { // if a piece exists at the intended nextLocation

        if (isEnemy(nextLoc, pieceMap)) { //////////////jumping over enemy/////////////
            if (isEmpty(nextnextLoc, pieceMap)) {
                location = nextnextLoc;
                if (ifshouldCrowned(location) && (!isKing())) {
                    puts("Crowned a King!");
                    turnKing();
                }

                it = pieceMap.find (nextLoc);// deletes an old key after piece movement
                pieceMap.erase (it);
                delete(it->second);
                puts("Jumped!");
                return 0;

            } else {
                puts("Invalid move"); //jumping on opponent's piece when jump is unavailiable
                return 1;
            }

        } else { //jumping on your own piece
            puts("Invalid move");
            return 1;
        }
    } else { // valid move into empty space
        location = nextLoc;
        if (ifshouldCrowned(location)) {
            puts("Crowned a King!");
            turnKing();
        }

        return 0;
    }
}








/////////may  be hard to read/// I tried to be very general//

bool CheckerBoard::mustJumpList(bool turn)  //check whether there is a piece that MUST jump.
{
    ////also, this function makes a vector of string that user MUST input, in case when mustJump is true.
    forcedList.clear();
    forcedList.shrink_to_fit(); // making a brand new empty vector
    std::string temp; /// encodes an input string that user MUST provide
    bool must = false;
    std::vector<std::pair<int, int>> moveRange;
    std::vector<std::pair<int, int>> jumpRange;


    for (it = pieceMap.begin(); it != pieceMap.end();
            ++it) {  // iterating through all pieces in the map

        ////remember, pieceMap maps between xy coordinate and a piece that is located at the coordinate.
        it->second->moveableSpace();
        moveRange.clear();
        jumpRange.clear();
        moveRange = (it->second)->getmvReach();
        jumpRange = (it->second)->getjpReach();

        //////////////////////////jumpRange and moveRange always have the same size for a given piece//////////////////////////
        //////////////////////////for regular piece, size(moveRange)==size(jumpRange)==2, for king piece, 4////////////////////

        for (unsigned int i = 0; i < moveRange.size();
                i++) { //////iterating through all movable space (and jumpable space concurrently) of a given piece
            temp.clear();
            ///it->second == piece that 'it' points to////
            if (((it->second)->isEnemy(moveRange[i], pieceMap)) &&
                    ((it->second)->isEmpty(jumpRange[i], pieceMap)) && (!((it->second)->isWall(jumpRange[i])))) {
                /////ex) if there is an enemy on its right, and if there is an empty space on the right side of the enemy, must jump over that enemy///
                ////but, no jump if there is a wall on the right side of the enemy///////////////////////////////////
                if ((turn && (it->second->whichTeam())) || ((!turn) && (!(it->second->whichTeam())))) {
                    must = true; //if there is any piece that must jump, mustJumpList() WILL return true;


                    temp += ((it->second)->getLocation()).first + '0'; // x coordinate
                    temp += ((it->second)->getLocation()).second + '0'; // y coordiate

                    if ((!(it->second->isKing()))) { //for regular pieces
                        if (i == 0) {
                            temp += 'r';   ///////in move() method of Piece class, [0] is encoded as 'r'
                        } else {
                            temp += 'l';   //////////in move() method of Piece class, [1] is encoded as 'l'
                        }

                        ////only i=0 and i=1 exist for regular pieces (they can either have left movement or right movement)

                    } else { //////for king piece///////  [0] [1] [2] [3] is eoncded for tr tl br bl
                        if (i == 0) {
                            temp += 't';
                            temp += 'r';
                        } else if (i == 1) {
                            temp += 't';
                            temp += 'l';
                        } else if (i == 2) {
                            temp += 'b';
                            temp += 'r';
                        } else if (i == 3) {
                            temp += 'b';
                            temp += 'l';
                        }

                    }
                    forcedList.push_back(temp);
                }//22r 15r
            }
        }
    }

    return must;
}

//////////////////////gameProcessor returns 1 if user inputs an invalid input, else returns 0//////////////
////////////////////for valid userInput, it triggers move() in piece class///////////////////////////
int CheckerBoard::gameProcessor(std::string userInput,
                                bool turn)  //if turn==true, 'true' player's turn, else 'false' player's turn
{

    if (mustJumpList(turn) &&
            !(count(forcedList.begin(), forcedList.end(), userInput))) { ///////////forced jump/////////////////
        //////if jump is required, but user didn't make a jump/////////////////////////////////////////////////////////

        puts("Invalid move");
        //   puts("must make a jump\nAvailable options: "); //////////needs to be changed to "invalid move", but just fast for debugging purpose
        // for (int i=0;i<forcedList.size();i++){
        //  std::cout<< forcedList[i]<< ' ';
        // }
        return 1;

    } else {
        ////Either when the user is not forced to make any jump, or when user made a jump when he was forced to.

        //pieceloc is just used as a place to store the binary value cuz it's available
        pieceloc.first = userInput.at(0) - 48; //x coordinate chosen by user
        pieceloc.second = userInput.at(1) - 48; // y ''
        for (it = pieceMap.begin(); it != pieceMap.end(); ++it) {

            if (pieceloc == (it->first)) {

                if (((it->second)->whichTeam()) == turn) { // Player chose his own piece


                    if ((it->second)->move(userInput,
                                           pieceMap) ==
                            0) { /////// move() return 0 for valid move, 1 for invalid move //////
                        /////////////////////////the conditional statement triggers movement///////////////////////

                        pieceMap[(it->second)->getLocation()] = (it->second);
                        it = pieceMap.find (pieceloc);// deletes an old key after piece movement
                        pieceMap.erase (it);
                        return 0;
                        /////////////////////////////every move is made through this encapsulation/////////////////////////////

                    } else {/////////////move returned 1///////////no movement was made/////////////////////////
                        return 1;   ///'invalid move signal' from move() triggers 'invalid userInput signal' from gameProcessor()
                    }

                } else {
                    //selected opponent's piece
                    puts("Invalid move");
                    return 1;
                    break;
                }
            }
        }
        ///there is no piece at  selected xy coordinate, if control reaches this point
        puts("Invalid move");
        return 1;

    }

}


void CheckerBoard::printBoard(int dummy)
/// dummy is not used in this code. just to be consistent with ship board.. dummpy will always be 0.
{
    if (dummy) {
        printf("\n");
    }
    char status[8][8] = {{0}};
    puts("~~~~~~~~~~~American Checker~~~~~~~~~~");
    puts("     --------------------------");

    for (it = pieceMap.begin(); it != pieceMap.end(); ++it) {

        if ((it->second)->whichTeam()) {
            if ((it->second)->isKing()) {
                status[std::get<0>(it->first)][std::get<1>(it->first)] = 'A'; //true team King->A

            } else {
                status[std::get<0>(it->first)][std::get<1>(it->first)] = '1'; //true team ->1
            }

        } else if (!((it->second)->whichTeam())) {

            if ((it->second)->isKing()) {
                status[std::get<0>(it->first)][std::get<1>(it->first)] = 'B'; //false team King ->B

            } else {
                status[std::get<0>(it->first)][std::get<1>(it->first)] = '2'; //false team ->2
            }
        }

    }

    for (int j = 7; j >= 0; j--) {
        printf("(%d) |  ", j);
        for (int i = 0; i < 8; i++) {

            if (status[i][j] == 0) {
                printf("0  ");
            } else {
                printf("%c  ", status[i][j]);
            }

        }
        printf("|    ! ");
        printf("\n");
    }
    puts("     --------------------------");
    puts("      (0)(1)(2)(3)(4)(5)(6)(7)");
}

void CheckerBoard::saveGame(int& player1Turn, int& player2turn, std::string& namefile)
{


    char team;
    char king;
    std::ofstream ofile;
    ofile.open(namefile);
    ofile << "checkers" << std::endl;
    ofile << "player 1 turn " << player1Turn << " player 2 turn " << player2turn << std::endl;
    for (it = pieceMap.begin(); it != pieceMap.end(); ++it) {
        if (it->second->isKing()) {
            king = 'k';
        } else {
            king = 'p';
        }
        if (it->second->whichTeam()) {
            team = '1';
        } else {
            team = '2';
        }
        ofile << team << " " << (it->first).first << " " << (it->first).second << " " << king << std::endl;
    }

}

void CheckerBoard::loadGame(int* player1turn, int* player2turn, std::string& namefile)
{

    pieceMap.clear();
    std::ifstream loadfile(namefile);
    std::string line;
    std::pair<int, int> loc;
    int count = 0;
    std::string temp;
    while (std::getline(loadfile, line)) {
        int c = 0;
        std::istringstream iss(line);
        if (count == 0) {
            count++;
            // continue;
        }

        else if (count == 1) {
            //file line format "player 1 turn 3 player 2 turn 2"

            do {
                iss >> temp;
                if (c == 3) {
                    *player1turn = atoi(temp.c_str());
                } else if (c == 7) {
                    *player2turn = atoi(temp.c_str());
                }
                c++;
            } while (iss);
            count++;
        } else {
            //file line format "1 2 0 p"

            loc.first = line[2] - '0';
            loc.second = line[4] - '0';
            if (line[0] == '1') {
                pieceMap[loc] = new oPiece(loc);
            } else {
                pieceMap[loc] = new xPiece(loc);
            }

            if (line[6] == 'k') {
                (pieceMap[loc])->turnKing();
            }
            count++;
        }

    }
}
void CheckerBoard::deleteAll()
{
    for (it = pieceMap.begin(); it != pieceMap.end(); ++it) {

        delete(it->second);
    }
} // end of deleteAll

bool CheckerBoard::gameOn(int* turn1, int* turn2, bool* turn, std::string& input)
{
    //// over if no available move ||  surrender ||  piece all dead

    int numPiece2 = 0;
    int numPiece1 = 0;
    //there is no parameter anywhere that keep tracks of #pieces for each team. So, I have to count through this function.
    for (it = pieceMap.begin(); it != pieceMap.end(); ++it) {
        if (it->second->whichTeam()) { //pieceMap only maps available pieces. it does not map any empty spaces, so it->second must be either piece1 or piece2
            numPiece1++;
        } else {
            numPiece2++;
        }
    }

    if (numPiece1 == 0 || ((input.compare("surrender") == 0) &&
                           *turn)) { ///if player 1's piece # equals zero, OR (it's player1's turn and someone typed in surrender)
        deleteAll();
        printf("Player 2 wins on turn %d\n", *turn1);
        return false; //game is OFF now
    } else if (numPiece2 == 0 || ((input.compare("surrender") == 0) &&
                                  !(*turn))) { ///if player 2's piece # equals zero, OR (it's player2's turn and someone typed in surrender)

        deleteAll();

        printf("Player 1 wins on turn %d\n", *turn2);
        return false; //games is OFF now


    } else {
        return true; // game is ON yet.
    }
}

///////////////////////Battleship functions//////////////////////////

void ShipPiece::setCoordinates ()
{
    switch (direction) {
    case 'h':
        for (int i = location.first; i < (location.first + space); i++) {
            boardspot[ {i, location.second}] = EXIST;
        }
        break;
    case 'v':
        for (int i = location.second; i > (location.second - space); i--) {
            boardspot[ {location.first, i}] = EXIST;
        }
        break;
    }
} // end of setCoordinates method

std::string& ShipPiece::getName()
{
    return name;
} // end of getName method

int& ShipPiece::getSpace()
{
    return space;
} // end of getSpace method



//checks if shipspots are within board bounds and not already occupied
//returns false if out of bound or already occupied by another ship, true otherwise
bool ShipBoard::checkSpot(ShipPiece& pieceToAdd, std::vector<ShipPiece>& ships)
{
    for (Element::iterator eit = pieceToAdd.boardspot.begin(); eit != pieceToAdd.boardspot.end();
            eit++) {
        std::pair<int, int> coord = eit->first;

        //coordinates out of bound
        if (coord.first > 10 || coord.first < 0 || coord.second > 10 || coord.second < 0 ) {
            return false;
        }

        //spot already occupied by a pre-existing ship from the vector
        for (int i = 0; i < (int) ships.size(); i++) {
            for (Element::iterator eit2 = ships[i].boardspot.begin(); eit2 != ships[i].boardspot.end();
                    eit2++) {
                if (coord == eit2->first) {
                    return false;
                }
            }
        }

    } // end of for loop
    return true;
} // end of checkSpot method



//adding a vector of ships to the board
void ShipBoard::addShipsToBoard(std::vector<ShipPiece>& ships)
{
    for (int i = 0; i < (int) ships.size(); i++) {
        for (Element::iterator eit = ships[i].boardspot.begin(); eit != ships[i].boardspot.end(); eit++) {
            board[eit->first] = &(ships[i]);
        }
    }
} // end of addShipsToBoard method



//the spot is attacked by the other player
void ShipBoard::attackedOnBoard(int x, int y)
{
    ShipMap::iterator sit = board.find({x, y});
    //hit a spot where ship doesn't exist
    if (sit == board.end()) {
        std::cout << "Miss!" << std::endl;
        missed.push_back(std::make_shared<ShipPiece>("miss", x, y, 'h', 1));
        missed[missed.size() - 1]->boardspot[ {x, y}] = MISS;
        board[ {x, y}] = missed[missed.size() - 1].get();
        //hit a spot that has aldready been hit, sunk or identified as miss
    } else if (board[ {x, y}]->boardspot[ {x, y}] == HIT || board[ {x, y}]->boardspot[ {x, y}] == SUNK
               || board[ {x, y}]->boardspot[ {x, y}] == MISS) {
        std::cout << "Miss!" << std::endl;
        //hit a valid spot
    }
    else {
        if (hitOrSink(sit->second, x, y) == true) {
            std::cout << "Hit!" << "\n";
        } else {
            if (playerNum == 1) {
                std::cout << sit->second->getName() << " sunk by player 2!" << std::endl;
            } else if (playerNum == 2) {
                std::cout << sit->second->getName() << " sunk by player 1!" << std::endl;
            }
        }
    }
} // end of attackedOnBoard method



//return true if the ship was hit, false if the ship was hit and sunk
bool ShipBoard::hitOrSink(ShipPiece* ship, int x, int y)
{
    ship->boardspot[ {x, y}] = HIT;
    ship->getSpace()--;
    if (ship->getSpace() == 0) {
        for (Element::iterator eit = ship->boardspot.begin(); eit != ship->boardspot.end(); eit++) {
            eit->second = SUNK;
        }
        sunkShip++;
        if (sunkShip == 5) {
            onOff = false;
        }
        return false;
    }
    return true;
} // end of hitOrSink method



void ShipBoard::printBoard(int option)
{
    for (int j = 9; j >= -1; j--) {
        for (int i = 0; i <= 9; i++) {
            if (i == 0) {
                if (j != -1) {
                    std::cout << j << " | ";
                } else {
                    std::cout << "   ";
                }
            }
            if (j == -1) {
                std::cout << " " << i;
                continue;
            }
            std::pair <int, int> boardPosition = {i, j};
            ShipMap::iterator sit = board.find(boardPosition);
            if (sit == board.end()) {
                std::cout << "~ ";
                if (i == 9) {
                    std::cout << "|";
                }
                continue;
            }
            int spotStatus = sit->second->boardspot[boardPosition];
            switch (spotStatus) {
            case EXIST:
                if (option == EXIST) {
                    std::cout << board.find(boardPosition)->second->getName()[0] <<
                              " ";  //change to first letter of ship type?
                } else if (option == MISS) {
                    std::cout << "~ ";
                }
                break;
            case HIT:
                std::cout << "@ ";
                break;
            case SUNK:
                std::cout << "X ";
                break;
            case MISS:
                if (option == EXIST) {
                    std::cout << "~ ";
                } else if (option == MISS) {
                    std::cout << "O ";
                }
                break;
            default:
                std::cout << spotStatus;
            }
            if (i == 9) {
                std::cout << "|";
            }
        } // end of inner for loop
        std::cout << "\n";
    }
} // end of printBoard method

bool ShipBoard::gameOn ()
{
    return onOff;
} // end of gameOn method


/* debugging purposes only
void BattleshipPlayer::printAddBoard() {
    for (int j = 9; j >= -1; j--) {
        for (int i = 0; i <= 9; i++) {
            std::pair <int,int> shipSpot = {i, j};

            bool find = false;
            char letter;
            for (int k = 0; k < (int) ships.size(); k++) {
                for (Element::iterator eit3 = ships[k].boardspot.begin();
                        eit3 != ships[k].boardspot.end(); eit3++) {
                    if (shipSpot == eit3->first) {
                        find = true;
                        letter = ships[k].getName()[0];
                    }
                }
            }
            if (i == 0) {
                if (j != -1) {
                    std::cout << j << " ";
                } else {
                    std::cout << "  ";
                }
            }
            if (find) {
                std::cout << "|" << letter;
            } else {
                if (j != -1) {
                    std::cout << "|~";
                } else {
                    std::cout << " " << i;
                }
            }
            if (i == 9 && j != -1) {
                std::cout << "|";
            }
        } // end of inner for loop
        std::cout << "\n";
    } // end of outer for loop
} // end of printAddBoard
*/


//return true if player chooses to quit while adding ships, return false otherwise
bool BattleshipPlayer::addingShips()
{
    std::vector<std::string> askShipkind = {"aircraft carrier (5):", "battleship (4):", "cruiser (3):", "submarine (3):", "destroyer (2):"};
    std::vector<std::string> shipkind = {"Aircraft carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};
    std::vector<int> numSpaces = {5, 4, 3, 3, 2};

    for (int i = 0; i < (int) askShipkind.size(); i++) {
        bool check = false;
        do {
            std::string userinput;
            std::cout << "Player " << playerNum << " place " << askShipkind[i] << std::endl;
            std::cin >> userinput;

            if (userinput[0] == 'q' || userinput[0] == 'Q') {
                throw std::runtime_error("");
            }

            if (checkForValidPlace(userinput) == false) {
                std::cout << "Invalid Input" << std::endl;
                continue;
            }

            int x = userinput[0] - '0';
            int y = userinput[1] - '0';
            char hv = userinput[2];

            ShipPiece ship(shipkind[i], x, y, hv, numSpaces[i]);
            check = shipBoard.checkSpot(ship, ships);
            if (check) {
                ships.push_back(ship);
            } else {
                std::cout << "Invalid Input" << std::endl;
            }

        } while (!check); // end of do-while

    } // end of for loop

    shipBoard.addShipsToBoard(ships);

    return false;
} // end of addingShips method

//return true if the coordinate is valid, false otherwise
bool BattleshipPlayer::checkForValidPlace(std::string& userinput)
{
    if (userinput.length() != 3) {
        return false;
    }
    int x = userinput[0] - '0';
    int y = userinput[1] - '0';
    if ((x > 10) || (x < 0) || (y > 10) || (y < 0)) {
        return false;
    }
    if (userinput[2] != 'h' && userinput[2] != 'H' && userinput[2] != 'v' && userinput[2] != 'V') {
        return false;
    }
    return true;
} // end of checkForValidPlace method

//return true if attack is valid, false otherwise
bool BattleshipPlayer::checkForValidAttack(std::string& userinput)
{
    if (userinput.length() != 2) {
        return false;
    }
    int x = userinput[0] - '0';
    int y = userinput[1] - '0';
    if ((x > 10) || (x < 0) || (y > 10) || (y < 0)) {
        return false;
    }
    return true;
} // end of checkForValidAttack method

void BattleshipPlayer::attackedOnBoard(int x, int y)
{
    shipBoard.attackedOnBoard(x, y);
} // end of attackedOnBoard method

void BattleshipPlayer::playerTurnMessage()
{
    std::cout << "Player " << playerNum  << " turn " << turnNum << ":" << std::endl;
} // end of playerTurnMessage method

void BattleshipPlayer::incrementTurn()
{
    turnNum++;
} // end of incrementTurn method

void BattleshipPlayer::printBoard(int option)
{
    shipBoard.printBoard(option);
} // end of printBoard method

bool BattleshipPlayer::isOn ()
{
    return shipBoard.gameOn();
} // end of gameOn method

void BattleshipPlayer::printVictoryMessage()
{
    std::cout << "Player " << playerNum << " wins on turn " << turnNum - 1 << "!" << std::endl;
} // end of printVictoryMessage



//UnitTests
void UnitTest::runBattleTest(ShipPiece& test)
{
    //testing setCoordinates function
    std::pair <int, int> pair1 (5, 3);
    std::pair <int, int> pair2 (6, 3);
    std::pair <int, int> pair3 (9, 3);
    assert(test.boardspot[pair1] == EXIST);
    assert(test.boardspot[pair2] == EXIST);
    assert(test.boardspot[pair3] == EXIST);

    std::cout << "passed unit test" << std::endl;
} // end of runBattleTest(ShipPiece& test) method

void UnitTest::runBattleTest(ShipBoard& test)
{
    //virtual void printBoard(int option) override is a simple printing function

    //testing bool gameOn()
    assert(test.gameOn() == true);

    //testing bool hitOrSink(ShipPiece* ship, int x, int y)
    ShipPiece testShip1("test1", 5, 4, 'h', 5);
    ShipPiece testShip2("test2", 1, 8, 'v', 4);
    std::vector<ShipPiece> testShipVector = {testShip1, testShip2};
    assert(test.hitOrSink(&testShip1, 6, 4) == true);
    assert(test.hitOrSink(&testShip1, 8, 4) == true);
    assert(test.hitOrSink(&testShip1, 9, 4) == true);
    assert(test.hitOrSink(&testShip2, 1, 7) == true);
    assert(test.hitOrSink(&testShip2, 1, 5) == true);

    //testing bool checkSpot(ShipPiece& pieceToAdd, std::vector<ShipPiece>& ships)
    ShipPiece testShip3("test3", 9, 3, 'v', 1);
    ShipPiece testShip4("test4", 9, 3, 'v', 1);
    ShipPiece testShip5("test5", 9, 5, 'v', 4);
    assert (test.checkSpot(testShip3, testShipVector) == true);
    testShipVector.push_back(testShip3);
    assert (test.checkSpot(testShip4, testShipVector) == false);
    assert (test.checkSpot(testShip5, testShipVector) == false);

    //testing void addShipsToBoard(std::vector<ShipPiece>& ships)
    test.addShipsToBoard(testShipVector);
    std::pair <int, int> coord1 ({5, 4});
    std::pair <int, int> coord2 ({1, 6});
    std::pair <int, int> coord3 ({9, 3});
    assert(test.board[coord1]->name == "test1");
    assert(test.board[coord2]->name == "test2");
    assert(test.board[coord3]->name == "test3");

    //testing void attackedOnBoard(int x, int y)
    test.attackedOnBoard(5, 4);
    assert(testShipVector[0].boardspot[coord1] == HIT);
    assert(testShipVector[1].boardspot[coord2] == EXIST);
    assert(testShipVector[2].boardspot[coord3] == EXIST);
    test.attackedOnBoard(9, 3);
    assert(testShipVector[2].boardspot[coord3] == SUNK);

} // end of runBattleTest(ShipBoard& test) method

void UnitTest::runBattleTest(BattleshipPlayer& test)
{
    //void playerTurnMessage() is a simple printing function
    //void printBoard(int option) is a simple printing function
    //void printVictoryMessage() is a simple printing function
    //bool addingShips() receives userinput and interacts with the user.
    //void attackedOnBoard(int x, int y) acts as a middleman and simply calls shipBoard.attackedOnBoard(x, y), which is tested above

    //testing bool checkForValidAttack(std::string& userinput)
    std::string input1 = "32";
    assert(test.checkForValidAttack(input1) == true);
    std::string input2 = "100";
    assert(test.checkForValidAttack(input2) == false);
    std::string input3 = "99";
    assert(test.checkForValidAttack(input3) == true);

    //testing void incrementTurn()
    test.turnNum = 5;
    test.incrementTurn();
    assert(test.turnNum == 6);

    //testing bool isOn ()
    assert(test.isOn() == true);

    //testing bool checkForValidPlace(std::string& userinput)
    std::string placeinput1 = "53h";
    assert(test.checkForValidPlace(placeinput1) == true);
    std::string placeinput2 = "530";
    assert(test.checkForValidPlace(placeinput2) == false);
    std::string placeinput3 = "xxx";
    assert(test.checkForValidPlace(placeinput3) == false);
    std::string placeinput4 = "helloworld";
    assert(test.checkForValidPlace(placeinput4) == false);

} // end of runBattleTest(BattleshipPlayer& test) method



void UnitTest::runCheckerUnitTest(CheckerBoard& cb)
{

    //gameProcessor returns 0 when user Input is valid.
    //'true' means player1's turn. 'false' means player2's turn
    // In the game, 'turn' automatically flips back and forth between 'true' and 'false', but in this unittest, I'm manually controlling it.

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // FOR GRADERS: IF WHAT I DO HERE CONFUSES YOU, use this command  "cb.printBoard(0);" anywhere you want in order to see what is going on. //////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    assert(cb.gameProcessor("11r",
                            true) == 1); // invalid move. This is commanding player1's piece at (x=1,y=1) to jump on his own piece at (x=2,y=2)
    assert(cb.gameProcessor("22r",
                            true) == 0); // valid move. Player1's piece can be moved from 22 to r.
    assert(cb.gameProcessor("11r",
                            true) == 0); // valid move.now that player1's piece at (x=2,y=2) moved to (x=3,y=3), player1's piece at (x=1,y=1) can move to (x=2,y=2)
    assert(cb.gameProcessor("00l",
                            true) == 1); // invalid move. player1's piece at (x=0,y=0) is being told to move into the wall, which is illegal.
    assert(cb.gameProcessor("15r",
                            false) == 0); //valid move. player2's piece at (x=1,y=5) is moved to (x=2,y=4)

    //NOW, There is a possible jump that player1 can take. Player1's piece at (x=3,y=3) can jump over Player2's piece at (x=2,y=4)
    //Therefore, Player1 MUST press "33l";
    //Let's ignore to induce an error. In the following code, instead of "33l", I typed in "33r". gameProcessor, as expected, returns 1 for the error.
    assert(cb.gameProcessor("33r", true) == 1); //Invalid move
    assert(cb.gameProcessor("33l", true) == 0); // (valid move) now take the required jump
    //NOW, there is a possible jump that player2 can take. There is player1's piece at (x=1,y=6). There are player2's pieces at (x=0,y=6) and (x=2,y=6). Both of
    //the Player2's pieces can jump. The only two possible commands that player2 can use is "06r" and "26l"
    assert(cb.gameProcessor("55r", false) == 1); // if we ignore the jump, gameProcessor returns 1.
    assert(cb.gameProcessor("26l",
                            false) == 0); // If a jump is teakn. VALId input. player1's piece at (x=1,y=6) is now jumped
    assert(cb.gameProcessor("06l",
                            false) == 1); // Invalid input. this command is telling player2's piece to run in to the wall.

    //gameProcessor returns 1 also when player2's piece is seletected when it is player1's turn.
    assert(cb.gameProcessor("35r",
                            true) == 1); // Recall that 'true' means player1's turn. Player2's piece is located at "35r", therefore returns an error.


    std::pair<int, int>loc (2, 2); //


    ///////////////////KING TEST////////////////////////////////
    //NOW, let's test ifshouldCrowned. This function tests whether a piece at a certain location should turn king.
    ((cb.pieceMap)[loc])->ifshouldCrowned(loc);

    assert((((cb.pieceMap)[loc])->ifshouldCrowned(loc)) ==
           false); // This is false, because the piece at (2,2) did not reach the end of the board.
    //ifshouldCrowned will return true only when a piece reaches the end.

    //Now let's manually turn one of the pieces into a king
    ((cb.pieceMap)[loc])->turnKing(); //The piece at (2,2) is manually turned into a king just for testing purpose.
    assert(((cb.pieceMap)[loc])->isKing() == true);
    //NOW,test that King should be forced to make a jump when there is an enemy piece at a behind-diagonal space.
    //The following list of codes just places the new king in to a spot that forces it to back jump.
    cb.gameProcessor("22tl", true);
    cb.gameProcessor("13tr", true);
    cb.gameProcessor("24tl", true);
    cb.gameProcessor("23tr", true);
    cb.gameProcessor("15tr", true);
    //NOW the king piece of player1 is placed at (2,6), and there is an enemy piece at (3,5). The king piece, which is capable of back-jump,
    //it will be forced to jump. The only valid move for the king piece is "26br"
    assert(cb.gameProcessor("26bl",
                            true) == 1); //attempt to NOT jump over (3,5) will return "invalid input"
    assert(cb.gameProcessor("26br", true) == 0);

    ///there are some minor functions such as isWall().. etc, but they will not be directly tested, although they are pure functions. They are
    // all used in gameProcessor(), and therefore, testing gameProcessor() indirectly tests those minor functions as well.

} // end of runCheckerUnitTest(CheckerBoard &cb) method

void UnitTest::runTest()
{
    //testing methods in the ShipPiece class
    ShipPiece testShip1("Samantha", 5, 3, 'h', 5);
    runBattleTest(testShip1);

    //testing methods in the ShippBoard class
    ShipBoard testShipBoard(1);
    runBattleTest(testShipBoard);

    //testing methods in the BattleShipPlayer class
    BattleshipPlayer testPlayer(1);
    runBattleTest(testPlayer);

    //GameRunner class is added in order to reduce code duplication. Therefore, its methods
    //contain multiples of other methods that ask for userinput, print statements, and require a
    //complete set of vectors and maps. Therefore, methods in this class were not able to be tested
    //during unit test. However, end-to-end test contains a through testing of the methods that belong
    //to this class

    CheckerBoard bg;
    runCheckerUnitTest(bg);
    std::cout << "Passed Unit Tests!" << std::endl;

} // end of runTest method
