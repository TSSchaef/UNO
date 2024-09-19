#include <cstdlib>
#include <ctime>

#include "house_rules.h"
#include "util.h"
#include "deck.h"

class Stats{
    int numCardsPlayed = 0;
    int numDrawTwos = 0;
    int numReverses = 0;
    int numSkips = 0;
    int numWilds = 0;
    int numWildDrawFours = 0;
    int numTurns = 0;
    public:
        Stats(){}
        void noteCard(const char *card){
            numCardsPlayed++;    
            switch(card[CARD_TYPE]){
                case 'D':
                    numDrawTwos++;
                    break;
                case 'R':
                    numReverses++;
                    break;
                case 'S':
                    numSkips++;
                    break;
                default:
                    break;
            }

            if(card[CARD_SUIT] == 'W'){
                if(card[CARD_TYPE] == '0'){
                    numWilds++;
                } else {
                    numWildDrawFours++;
                }
            }
        }

        void setNumTurns(int numTurns){
            this->numTurns = numTurns;
        }

        void print(){
            std::cout << "Turns:                " << numTurns << std::endl;
            std::cout << "Cards Played:         " << numCardsPlayed << std::endl;
            std::cout << "% of Draw Twos:       " << (numDrawTwos + 0.0) / numCardsPlayed << std::endl;
            std::cout << "% of Reverses:        " << (numReverses + 0.0) / numCardsPlayed << std::endl;
            std::cout << "% of Skips:           " << (numSkips + 0.0) / numCardsPlayed << std::endl;
            std::cout << "% of Wilds:           " << (numWilds + 0.0) / numCardsPlayed << std::endl;
            std::cout << "% of Wild Draw Fours: " << (numWildDrawFours + 0.0) / numCardsPlayed << std::endl;
        }
};

class Game{
    private:
        int numPlayers;
        Deck deck;
        int numTurns;
        Stats cardStats = Stats();
        std::vector<Hand> players;
        std::vector<Hand>::iterator currPlayer;
        int playerIndex;

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
            playerIndex = 0;

            const char *lastCard = deck.topCard();
            //if the first card up is wild
            //more complex logic could be impemented later
            if(lastCard[CARD_SUIT] == 'W'){
                lastCard = "B*";
            }
            char context[3];

            while(!finished()){
                //std::cout << "Last card: " << lastCard << std::endl;
                const char *cardPlayed = (*currPlayer).playCard(lastCard, context);

                //Checking there was a cardPlayed (Not NULL)
                if(cardPlayed){
                    deck.playCard(cardPlayed);
                    lastCard = cardPlayed;

                    //TO DO: Implement logic for utility cards;
                    //std::cout << cardPlayed << std::endl;
                    cardStats.noteCard(cardPlayed);

                    if(cardPlayed[CARD_SUIT] == 'W'){
                        //generic color cards for if a wild is played
                        //std::cout << "context: " << context << std::endl;
                        lastCard = context;
                    }
                } else {
                    //Currently no playable cards
                    //Apply drawing logic
                    //std::cout << "Draw" << std::endl;
#ifndef DRAW_UNTIL_PLAYABLE
                    //draw one card and end turn
                    (*currPlayer).addCard(deck.drawCard());
                    currPlayer = players.begin() + playerIndex;
#else
                    //draw until a card is playable then play it
                    const char *drawnCard;
                    while(1){
                       drawnCard = deck.drawCard(); 
                       if(canPlayCard(lastCard, drawnCard)){
                            deck.playCard(drawnCard);
                            lastCard = drawnCard;
                            cardStats.noteCard(drawnCard);
                            break;
                       } 
                        
                       //add card to player's hand
                       (*currPlayer).addCard(drawnCard);
                       currPlayer = players.begin() + playerIndex;
                    }
#endif
                }

                currPlayer++;
                playerIndex++;
                numTurns++;
                if(currPlayer == players.end()){
                    currPlayer = players.begin();
                    playerIndex = 0;
                }
            }

            cardStats.setNumTurns(numTurns);
        }

        void printInfo(){
            //deck.print();
            cardStats.print();
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
