# cpp_boardgame

1. TEAM
Samantha Brayman, sbrayma1 
Jinwoo Oh, joh27
Jiwon Shin, jshin49

2. DESIGN
* The game begins with a switch statement that chooses between the two games: battleship and checkers.  If q is selected it quits without selecting a game.  Depending on which game is selected, we then move to that game.  If quit is selected at any point in the games we catch and throw exceptions in order to exit and free memory.

* For battleship, we used a GameRunner class that is called in main.  This class allowed us to clean up our main.  We then create two player objects using our player class: player 1 and player 2.  These player alternate as attack player and defending player.  P1 attacks p2, followed by P2 attacking P1 in a loop until one player wins (all 5 opponent ships sunk).  Creating these player objects allowed us to significantly reduce code duplication.  When one player attacks the other we move into our battleship boards.  These are maps<pair<int, int>, ShipPiece*>.  We check for a hit, miss, or sink here.  We check this by accessing the ship-- the ship is a map<pair<int, int>, int>.   The value int represents the ship’s status, letting us know what the other player’s move was.  After each ship is sunk we check the number of ships that player has sunk-- if it is 5 that player wins and we return to the game selection screen.

* For checker,mainly two classes were involved: CheckerBoard and ChekcerPiece. They were each inherited from Board class and Piece class. Initialization of a CheckerBoard classes triggers initialization of CheckerPiece objects. CheckerPIece objects, upon initialization, are listed in a map object called pieceLoc, which is sorted by piece object’s xy coordinate. It is more reasonable to use a vector instead of a map, but we chose to use a map for Checkers as well so that BattleBoard class and CheckerBoard class can inherit from the same mother class. When user types in a string of input, the string input is classified with a range of conditional statements. For example, if the string input starts with ‘b’, it toggles between board on and off. If the string input does not fall into any of the listed conditions, the string input is then interpreted by the program as a command for the next move. In this case, the string input is passed to gameProcessor( ) method, which is a member of CheckerBoard class. gameProcessor() method then decides whether the input is valid for a command for next move. If it is decided to be valid, it triggers a selected piece on the board to change its location. This process repeats in a loop, which is controlled by gameON() method. gameON() method contains a list of conditional statements that decide when the game needs to be stopped. For example, when one of the players types in “surrender”, gameON() method will pick up the string input, and stop the game. 

* For the checker game, there are some traditional C features used including puts and printf. We have had a conversation with the instructor and explained that this is because some of the group members started working on the project before being too comfortable using c++ features. The instructor said it is understandable.

* Although we are aware of the fact that it is recommended for member variables to start with letter m, followed by CamelCase, variables that were initially not member variables ultimately became member variables as we created new classes and methods to reduce code duplication and improve scalability. So that is why some of the member variables do not necessarily start with the m prefix.

* Also, we have a beautiful printing methods so please take a look.

3. LANGUAGE FEATURES
* Some of the data Structures used includes Maps:  1) map<pair<int, int>, int>   2) map<pair<int, int>, ShipPiece*> and Vectors:  3) vector<ShipPiece>    4) vector<shr_ptr<ShipPiece>>.

* For Battleship, we used the first map type listed above.  The pair of ints represented the ship’s coordinates.  The int the pair maps to represents the ship’s status.  We use an enum to organize this: 1= ship exists, 2= ship hit, 3= ship sunk, 4= miss.  My using a map we were able to build the ship by pieces- each piece with a specific pair for coordinates.  This also made it easy to transfer ship info to the board.  After each ship was created we added to that ship to the vector (type listed above).  Once all five ships were added to this vector, meaning all 5 were properly placed by the user, we then added these ships to that player’s board.

* Each battleship player has one board.  We chose to use the second map type listed above for the boards.  Again the pair represents the coordinates but this time it maps to a ship pointer. We chose to map to a ship pointer because this allowed us to have multiple coordinates point to the same ship, rather than recreating it for each point.   We use the shared pointer vector listed above when we are tracking misses. Shared pointers were used to account for memory deallocation.

* Board class and Piece class were parent classes from which each game inherited its ShipBoard, CheckerBoard, and ShipPiece, CheckerPiece from.

4. COMPLETENESS
* We believe that our games are completely functional. We covered as many edge cases as we could think of during testing (move out of bounds, non-numeric coordinates, user input with extra character, user input with too few characters, negative numbers, assigning at a preoccupied ship spot, jumping over the wall in checkers, jumping over a piece’s own team member, jumping over two pieces, not jumping when available jump, etc.). In our tests these scenarios all worked.

5. SPECIAL INSTRUCTIONS - Not necessary.
