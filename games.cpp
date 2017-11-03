#include "games_header.h"


int main()
{

    bool end = false;
    while (!end) {
        char choice;
        std::cout << "Select game: (b)attleship or (c)heckers?" << std::endl;
        std::cin >> choice;

        GameRunner game;

        switch (choice) {
        case 'b':
        case 'B': {
            if (game.startBattle() == 1) {
                end = true;
                continue;
            }
            break;
        }
        case 'c':
        case 'C': {
            if (game.runChecker() == 1) {
                end = true;
                continue;
            }
            break;
        }
        case 'q':
        case 'Q': {
            end = true;
            continue;
        }
        default: {
            std::cout << "Invalid Input" << std::endl;
            break;
        }

        } // end of switch
    }

} // end of main
