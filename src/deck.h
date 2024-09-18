#ifndef DECK_H
#define DECK_H

#include "util.h"

class Hand{
    private:
        std::vector<const char *> cards = std::vector<const char *>();
    public:
        Hand(){}

        int numCards(){
            return cards.size();
        }

        char * playCard(){
            return NULL;
        }

        void addCard(const char *card){
           cards.push_back(card); 
        }

        void print(){
            std::vector<const char *>::iterator vi;
            for(vi = cards.begin(); vi != cards.end(); vi++){
                std::cout << *vi << ", ";
            }
            std::cout << std::endl;
        }
};

class Deck{
    private: 
        const char *deck[DECK_SIZE] = DECK;
        const char *discard[DECK_SIZE];
        int currCard = 0;
        int topDiscard = -1;

        void resetDeck(){
            int i;
            for(i = topDiscard - 1; i >= 0; i--){
                deck[i] = discard[i];    
                discard[i] = NULL;
            }

            discard[0] = discard[topDiscard];
            discard[topDiscard] = NULL;
            topDiscard = 0;
            currCard = 0;
            
            //shuffle();  
        }

    public:
        Deck(){
            int i;
            for(i = 0; i < DECK_SIZE; i++){
                discard[i] = NULL;
            }
        }

        std::vector<Hand> deal(int numPlayers){
            std::vector<Hand> hands = std::vector<Hand>();

            if((numPlayers * NUM_STARTING_CARDS) + 1 > DECK_SIZE){
                std::cerr << "ERROR: Not enough cards for all the players" << std::endl;
                return hands;
            }

            int i;
            for(i = 0; i < numPlayers; i++){
                hands.push_back(Hand());
            }

            std::vector<Hand>::iterator vi;
            for(i = 0; i < NUM_STARTING_CARDS; i++){
                for(vi = hands.begin(); vi != hands.end(); vi++){
                    (*vi).addCard(deck[currCard]);
                    deck[currCard] = NULL;
                    currCard++;
                }
            }

            return hands;
        }

        void shuffle(){
            int i, j, k;
            const char *temp;
            for(i = 0; i < NUM_SHUFFLE_SWAPS; i++){
                j = rand() % DECK_SIZE;
                k = rand() % DECK_SIZE;

                if(j == k) continue;
                if(!deck[j] || !deck[k]) continue;

                temp = deck[j];
                deck[j] = deck[k];
                deck[k] = temp;
            }
        }

        void setup(){
            playCard(deck[currCard]);
            deck[currCard] = NULL;
            currCard++;
        }

        const char *drawCard(){
            if(!(currCard < DECK_SIZE) || !deck[currCard]){
               resetDeck(); 
            }

            if(!deck[currCard]){
                std::cerr << "ERROR: No cards available to draw" << std::endl;
            }

            const char *card = deck[currCard];
            deck[currCard] = NULL;
            currCard++;
            return card;
        }

        const char *topCard(){
            if(topDiscard < 0 || topDiscard >= DECK_SIZE){
                std::cerr << "ERROR: Invalid index of discard pile" << std::endl; 
                return NULL;
            }
            return discard[topDiscard];
        }

        void playCard(const char *card){
           discard[++topDiscard] = card;
        }

        void print(){
            int i = 0;
            std::cout << "Deck:" << std::endl;
            for(i = 0; i < DECK_SIZE; i++){
                if(deck[i]){
                    std::cout << "Card " << i << ": " << deck[i] << std::endl;
                }
            }
            std::cout << "Discard:" << std::endl;
            for(i = 0; i < DECK_SIZE; i++){
                if(discard[i]){
                    std::cout << "Card " << i << ": " << discard[i] << std::endl;
                }
            }
        }
};

#endif
