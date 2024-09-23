#ifndef DECK_H
#define DECK_H

#include "util.h"
#include "house_rules.h"

bool canPlayCard(const char *prevCard, const char *card);

class Hand{
    private:
        std::vector<const char *> cards = std::vector<const char *>();
        int cardValues[NUM_CARD_POINTS] = CARD_POINTS;
    public:
        Hand(){}

        int numCards();
        int getCardPoints(const char *card);
        const char *playCard(const char *prevCard, char context[3]);
        void addCard(const char *card);
        void print();
};

class Deck{
    private: 
        const char *deck[DECK_SIZE] = DECK;
        const char *discard[DECK_SIZE];
        int currCard = 0;
        int topDiscard = -1;

        void resetDeck();

    public:
        Deck();

        std::vector<Hand> deal(int numPlayers);
        void shuffle();
        void setup();
        const char *drawCard();
        const char *topCard();
        void playCard(const char *card);
        void print();
};

#endif
