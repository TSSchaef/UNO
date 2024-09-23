#include "deck.h"
#include "util.h"

bool canPlayCard(const char *prevCard, const char *card){
    //can always play wilds
    if(card[CARD_SUIT] == 'W'){
        return true;
    }

    //Can play on matching colors
    if(card[CARD_SUIT] == prevCard[CARD_SUIT]){
        return true;
    }

    //Can play on any matching card type
    if(card[CARD_TYPE] == prevCard[CARD_TYPE]){
        return true;
    }

    return false;
}

int Hand::numCards(){
    return cards.size();
}

int Hand::getCardPoints(const char *card){
    int value = 0;
    if(card[CARD_SUIT] == 'W'){
        if(card[CARD_TYPE] == '0'){
           value = cardValues[13];
        }else {
           value = cardValues[14];
        }
    } else {
        switch(card[CARD_TYPE]){
            case '0':
                value = cardValues[0];
                break;
            case '1':
                value = cardValues[1];
                break;
            case '2':
                value = cardValues[2];
                break;
            case '3':
                value = cardValues[3];
                break;
            case '4':
                value = cardValues[4];
                break;
            case '5':
                value = cardValues[5];
                break;
            case '6':
                value = cardValues[6];
                break;
            case '7':
                value = cardValues[7];
                break;
            case '8':
                value = cardValues[8];
                break;
            case '9':
                value = cardValues[9];
                break;
            case 'R':
                value = cardValues[10];
                break;
            case 'S':
                value = cardValues[11];
                break;
            case 'D':
                value = cardValues[12];
                break;
            default:
                break;
        }
    }
    return value;
}


const char * Hand::playCard(const char *prevCard, char context[3]){
    std::vector<const char *>::iterator vi;
    std::vector<const char *>::iterator maxi;
    int maxScore = -1;
    
    int numPlayable = 0;
    for(vi = cards.begin(); vi != cards.end(); vi++){
        if(canPlayCard(prevCard, *vi)){
            numPlayable++;
            if(getCardPoints(*vi)  > maxScore){
                maxScore = getCardPoints(*vi);
                maxi = vi;
            }
        }
    }

    //House rule can only play wild draw 4's if no other card can be played
    #ifdef RESTRICT_WILD_FOURS
    if(numPlayable > 1 && (*maxi)[CARD_SUIT] == 'W' && (*maxi)[CARD_TYPE] == '4'){
        int tempScore = -1;
        for(vi = cards.begin(); vi != cards.end(); vi++){
            //look for cards that aren't W4's and take the best out of those.
            //If no cards meet these criteria (2 W4's in hand) the previous 
            //best will be taken
            if(canPlayCard(prevCard, *vi) && ((*vi)[CARD_SUIT] != 'W' || (*vi)[CARD_TYPE] != '4')){
                if(getCardPoints(*vi)  > tempScore){
                    tempScore = getCardPoints(*vi);
                    maxi = vi;
                }
            }
        }
    }
    #endif

    if(maxScore > -1){
        const char *bestCard = *maxi; 
        //if playing wild pick most popular color in hand
        //and set context to pass on
        if(bestCard[CARD_SUIT] == 'W'){
            int r, g, b, y;        
            r = g = b = y = 0;
            for(vi = cards.begin(); vi != cards.end(); vi++){
                switch((*vi)[CARD_SUIT]){
                    case 'R':
                        r++;
                        break;
                    case 'G':
                        g++;
                        break;
                    case 'B':
                        b++;
                        break;
                    case 'Y':
                        y++;
                        break;
                    default:
                        break;
                }
            }

            if(r > g && r > b && r > y){
               context[CARD_SUIT] = 'R'; 
            } else if (g > b && g > y) {
               context[CARD_SUIT] = 'G'; 
            } else if (y > b){
               context[CARD_SUIT] = 'Y'; 
            } else {
               context[CARD_SUIT] = 'B'; 
            }
            context[CARD_TYPE] = '*';
            context[2] = '\0';
        }

        cards.erase(maxi); 
        return bestCard;
    }

    //No cards are able to be played
    //TO DO: Implement drawing mechanic
    return NULL;
}

void Hand::addCard(const char *card){
    cards.push_back(card); 
}

void Hand::print(){
    std::vector<const char *>::iterator vi;
    for(vi = cards.begin(); vi != cards.end(); vi++){
        std::cout << *vi << ", ";
    }
    std::cout << std::endl;
}


void Deck::resetDeck(){
    int i;
    for(i = topDiscard - 1; i >= 0; i--){
        deck[i] = discard[i];    
        discard[i] = NULL;
    }

    discard[0] = discard[topDiscard];
    discard[topDiscard] = NULL;
    topDiscard = 0;
    currCard = 0;

    shuffle();  
}

Deck::Deck(){
    int i;
    for(i = 0; i < DECK_SIZE; i++){
        discard[i] = NULL;
    }
}

std::vector<Hand> Deck::deal(int numPlayers){
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

void Deck::shuffle(){
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

void Deck::setup(){
    playCard(deck[currCard]);
    deck[currCard] = NULL;
    currCard++;
}

const char *Deck::drawCard(){
    if(!(currCard < DECK_SIZE) || !deck[currCard]){
        resetDeck(); 
    }

    if(!deck[currCard]){
        /*std::cerr << "ERROR: No cards available to draw" << std::endl;
        int i;
        bool noCards = true;
        for(i = 0; i < DECK_SIZE; i++){
           if(deck[i]){
                std::cerr << "ERROR: Deck contains card, data structure error" << std::endl;
                noCards = false;
                break;
           }
        }

        for(i = 0; i < DECK_SIZE; i++){
           if(discard[i]){
                std::cerr << "ERROR: Discard contains card, data structure error" << std::endl;
                noCards = false;
                break;
           }
        }

        if(noCards){
            std::cerr << "ERROR: Deck contains no cards, all cards in players hands" << std::endl;
        }*/

        return NULL;
    }

    const char *card = deck[currCard];
    deck[currCard] = NULL;
    currCard++;
    return card;
}

const char *Deck::topCard(){
    if(topDiscard < 0 || topDiscard >= DECK_SIZE){
        std::cerr << "ERROR: Invalid index of discard pile" << std::endl; 
        return NULL;
    }
    return discard[topDiscard];
}

void Deck::playCard(const char *card){
    discard[++topDiscard] = card;
}

void Deck::print(){
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

