#include <cstdlib>
#include <cstdio>
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
    int numGames = 0;
    int noMoreCards = 0;
    double numPlayers;
    public:
        Stats(int numPlayers){
            this->numPlayers = numPlayers;
        }

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

        void outOfCards(){
            noMoreCards++;
        }

        void setNumTurns(int numTurns){
            this->numTurns = numTurns;
        }

        void addStats(Stats toAdd){
            numCardsPlayed += toAdd.numCardsPlayed;
            numDrawTwos += toAdd.numDrawTwos;
            numReverses += toAdd.numReverses;
            numSkips += toAdd.numSkips;
            numWilds += toAdd.numWilds;
            numWildDrawFours += toAdd.numWildDrawFours;
            numTurns += toAdd.numTurns;
            noMoreCards += toAdd.noMoreCards;
            numPlayers = (this->numPlayers + toAdd.numPlayers) / 2;
            numGames++;
        }

        void print(){
            std::cout << "Number of Games:       " << numGames << std::endl;
            std::cout << "Stalemate Games:       " << noMoreCards << std::endl;
            std::cout << "Turns per Player:      " << (numTurns + 0.0) / (numGames * numPlayers) << std::endl;
            std::cout << "Cards Played per Game: " << (numCardsPlayed + 0.0) / numGames << std::endl;
            std::cout << "% of Draw Twos:        " << ((numDrawTwos + 0.0) / numCardsPlayed) * 100.0 << std::endl;
            std::cout << "% of Reverses:         " << ((numReverses + 0.0) / numCardsPlayed) * 100.0 << std::endl;
            std::cout << "% of Skips:            " << ((numSkips + 0.0) / numCardsPlayed) * 100.0 << std::endl;
            std::cout << "% of Wilds:            " << ((numWilds + 0.0) / numCardsPlayed) * 100.0 << std::endl;
            std::cout << "% of Wild Draw Fours:  " << ((numWildDrawFours + 0.0) / numCardsPlayed) * 100.0 << std::endl;
        }
};

class Game{
    private:
        int numPlayers;
        Deck deck;
        int numTurns;
        Stats cardStats;
        std::vector<Hand> players;
        std::vector<Hand>::iterator currPlayer;
        int playerIndex;

    public:
        Game(int numPlayers) : cardStats(numPlayers) {
            this->numPlayers = numPlayers;
            numTurns = 0;
            deck = Deck();
        }

        Stats getStats(){
            return cardStats;
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
            bool goingForward = true;

            while(!finished()){
                //std::cout << "Last card: " << lastCard << std::endl;
                const char *cardPlayed = (*currPlayer).playCard(lastCard, context);

                //Checking there was a cardPlayed (Not NULL)
                if(cardPlayed){
                    deck.playCard(cardPlayed);
                    lastCard = cardPlayed;
                    cardStats.noteCard(cardPlayed);

                    //TO DO: Implement logic for utility cards;
                    //Reverse
                    if(cardPlayed[CARD_TYPE] == 'R'){
                        goingForward = !goingForward; 
                    }
                    
                    //Skip
                    if(cardPlayed[CARD_TYPE] == 'S'){
                        if(goingForward){
                            currPlayer++;
                            playerIndex++;
                            if(currPlayer == players.end()){
                                currPlayer = players.begin();
                                playerIndex = 0;
                            }
                        } else {
                            if(currPlayer == players.begin()){
                                currPlayer = players.end();
                                playerIndex = players.size();
                            }
                            currPlayer--;
                            playerIndex--;
                        }
                    }

                    //Draw Two
                    if(cardPlayed[CARD_TYPE] == 'R'){
                        if(goingForward){
                            currPlayer++;
                            playerIndex++;
                            if(currPlayer == players.end()){
                                currPlayer = players.begin();
                                playerIndex = 0;
                            }
                        } else {
                            if(currPlayer == players.begin()){
                                currPlayer = players.end();
                                playerIndex = players.size();
                            }
                            currPlayer--;
                            playerIndex--;
                        }
                        //Skip to the next player, on the intermediate player
                        //(the one skipped) draw 2
                        //TODO add a preprocessing variable to allow stackable
                        //+2's as a house rule

                        const char* card = deck.drawCard();
                        if(!card){
                           cardStats.outOfCards();
                           break;
                        }
                       (*currPlayer).addCard(card);
                        
                        card = deck.drawCard();
                        if(!card){
                           cardStats.outOfCards();
                           break;
                        }

                       (*currPlayer).addCard(card);
                       currPlayer = players.begin() + playerIndex;
                    }

                    if(cardPlayed[CARD_SUIT] == 'W'){
                        //generic color cards for if a wild is played
                        //std::cout << "context: " << context << std::endl;
                        lastCard = context;

                        //implement wild draw 4 
                        if(cardPlayed[CARD_TYPE] == '4'){

                            if(goingForward){
                                currPlayer++;
                                playerIndex++;
                                if(currPlayer == players.end()){
                                    currPlayer = players.begin();
                                    playerIndex = 0;
                                }
                            } else {
                                if(currPlayer == players.begin()){
                                    currPlayer = players.end();
                                    playerIndex = players.size();
                                }
                                currPlayer--;
                                playerIndex--;
                            }
                            
                            int j;
                            bool drew4 = true;
                            for(j = 0; j < 4; j++){
                                const char* card = deck.drawCard();
                                if(!card){
                                    cardStats.outOfCards();
                                    drew4 = false;
                                    break;
                                }
                                (*currPlayer).addCard(card);
                            }
                            if(!drew4){
                               break; 
                            }
                            currPlayer = players.begin() + playerIndex;
                        }
                    }
                } else {
                    //Currently no playable cards
                    //Apply drawing logic
                    //std::cout << "Draw" << std::endl;
#ifndef DRAW_UNTIL_PLAYABLE
                    //draw one card and end turn
                    const char* card = deck.drawCard();
                    if(!card){
                        cardStats.outOfCards();
                        break;
                    }
                    (*currPlayer).addCard(card);
                    currPlayer = players.begin() + playerIndex;
#else
                    //draw until a card is playable then play it
                    const char *drawnCard;
                    bool outOfCards = false;
                    while(1){
                       drawnCard = deck.drawCard(); 

                       if(!drawnCard){
                           cardStats.outOfCards();
                           outOfCards = true;
                           break;
                       }

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
                    if(outOfCards) break;
#endif
                }


                numTurns++;
                if(goingForward){
                    currPlayer++;
                    playerIndex++;
                    if(currPlayer == players.end()){
                        currPlayer = players.begin();
                        playerIndex = 0;
                    }
                } else {
                    if(currPlayer == players.begin()){
                        currPlayer = players.end();
                        playerIndex = players.size();
                    }
                    currPlayer--;
                    playerIndex--;
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
    int numPlayers = 5;
    int numGames = 100000;
    /*std::cout << "Enter the number of players: " << std::endl;

    std::cin >> numPlayers;

    if(numPlayers <= 1){
        std::cerr << "ERROR: Invalid number of players" << std::endl;
        return -1;
    }
    
    std::cout << "Enter the number of games: " << std::endl;

    std::cin >> numGames;

    if(numGames < 1){
        std::cerr << "ERROR: Invalid number of games" << std::endl;
        return -1;
    }*/
   
    Stats statsAccumulator = Stats(numPlayers);
    while(numGames > 0){
        Game game = Game(numPlayers);
        game.run();
        statsAccumulator.addStats(game.getStats());
        numGames--;
        if(numGames % 10 == 0){
            printf("\rGames Remaining:       %d", numGames);
            fflush(stdout);
        }
    }
    printf("\r                               \r");
    
    statsAccumulator.print();

    return 0;
}
