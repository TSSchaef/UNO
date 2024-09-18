#include <cstdlib>
#include <ctime>

#include "util.h"
#include "deck.h"

class Stats{
    public:
        Stats(){}
};

class Game{
    private:
        int numPlayers;
        Deck deck;
        int numTurns;
        Stats cardStats;
        std::vector<Hand> players;
        std::vector<Hand>::iterator currPlayer;

    public:
        Game(int numPlayers){
            this->numPlayers = numPlayers;
            numTurns = 0;
            deck = Deck();
            cardStats = Stats();
        }

        bool finished(){
            std::vector<Hand>::iterator vi;
            for(vi = players.begin(); vi != players.end(); vi++){
                if((*vi).numCards() == 0) return true;
            }
            return false;
        }

        void run(){
            deck.shuffle(); 
            players = deck.deal(numPlayers);
            deck.setup();
            currPlayer = players.begin();

            /*deck.print();
            std::cout << std::endl;
            std::cout << std::endl;

            int i;
            for(i = 0; i < DECK_SIZE - (7 * NUM_STARTING_CARDS) - 1; i++){
                deck.playCard(deck.drawCard());
            }


            deck.print();
            std::cout << std::endl;
            std::cout << std::endl;*/
            
            while(!finished()){
                const char *cardPlayed = (*currPlayer).playCard(deck.topCard());
                //TO DO: Implement logic for utility cards;

                //Checking there was a cardPlayed (Not NULL)
                if(cardPlayed){
                    deck.playCard(cardPlayed);
                }

                currPlayer++;
                numTurns++;
                if(currPlayer == players.end()){
                    currPlayer = players.begin();
                }
            }
        }

        void printInfo(){
            deck.print();
        }
};

int main(int argc, char *argv[]){
    srand(time(0));
    std::cout << "Enter the number of players: " << std::endl;
    int numPlayers = 0;

    std::cin >> numPlayers;

    if(numPlayers <= 1){
        std::cerr << "ERROR: Invalid number of players" << std::endl;
        return -1;
    }

    Game game = Game(numPlayers);
    game.run();
    game.printInfo();

    return 0;
}
