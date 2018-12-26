/* 
 * Program to randomly sample cribbage hands and count their points
 */

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>

static std::string filepath = "/Users/alexstringer/phd/projects/learn-cpp/cribbage/score-run-best-subhand.csv";

// Define all the possible cards
static char values[13] = {'A','2','3','4','5','6','7','8','9','0','J','Q','K'};
static char suits[4] = {'H','C','D','S'};

// Class to represent a card
// A card has a value and a suit
class Card {
private:
  // Initialize
  void set_values(char setsuit,char setvalue) {
    // Check if values are valid
    bool valid_suit = false, valid_value = false;
    int i;
    for (i=0;i<13;i++) {
      if (setvalue == values[i]) {
        valid_value = true;
      }
    }
    for (i=0;i<4;i++) {
      if (setsuit == suits[i]) {
        valid_suit = true;
      }
    }
    
    if (!valid_suit) {
      std::cout << "Invalid suit: " << setsuit << "\n";
      exit(1);
    }
    if (!valid_value) {
      std::cout << "Invalid value: " << setvalue << "\n";
      exit(1);
    }
    
    suit = setsuit;
    value = setvalue;
  }
public:
  char suit;
  char value;
  
  // Default constructor
  Card() {
    
  }
  // Parametrized contructor
  Card(char setsuit,char setvalue) {
    set_values(setsuit,setvalue);
  }
  
  // Get the integer value of the card
  int int_value() {
    switch(value) {
    case 'A': 
      return 1;
      break;
    case '2': 
      return 2;
      break;
    case '3': 
      return 3;
      break;
    case '4': 
      return 4;
      break;
    case '5': 
      return 5;
      break;
    case '6': 
      return 6;
      break;
    case '7': 
      return 7;
      break;
    case '8': 
      return 8;
      break;
    case '9': 
      return 9;
      break;
    case '0': 
      return 10;
      break;
    case 'J': 
      return 10;
      break;
    case 'Q': 
      return 10;
      break;
    case 'K': 
      return 10;
      break;
    default: return 0;  
    }
  }
  
  // Print the card
  void print() {
    if (value == '0') {
      std::cout << '1' << value << suit;
    }
    else {
      std::cout << value << suit;
    }
  }
  
  // Compare equality to another card
  bool compare(Card other_card) {
    if (suit != other_card.suit) {
      return false;
    }
    if (value != other_card.value) {
      return false;
    }
    return true;
  }
  
  // Compare ordering to another card, by value
  int order_by_value(Card other_card) {
    if (int_value() > other_card.int_value()) {
      return 1;
    }
    else if (int_value() < other_card.int_value()) {
      return -1;
    }
    else if ( (value == '0') & (other_card.value != '0')) {
      return -1;
    }
    else if ( (value == 'J') & (other_card.value == 'Q' | other_card.value == 'K')) {
      return -1;
    }
    else if ( (value == 'Q') & (other_card.value == 'K')) {
      return -1;
    }
    else if ( (value == 'K') & (other_card.value != 'K')) {
      return 1;
    }
    else if ( (value == 'Q') & (other_card.value == 'J' | other_card.value == '0')) {
      return 1;
    }
    else if ( (value == 'J') & (other_card.value == '0')) {
      return 1;
    }
    else {
      return 0;
    }
  }
  
  // Method to test if supplied card is immediately greater than current card,
  // i.e. if current card is adjacent to and less than other card
  bool adjacent_and_less_than(Card other_card) {
    if (other_card.int_value() == int_value() + 1) {
      return true;
    }
    switch(other_card.value) {
    case 'J':
      if (value == '0') {
        return true;
      }
      else {
        return false;
      }
      break;
    case 'Q':
      if (value == 'K') {
        return true;
      }
      else {
        return false;
      }
      break;
    case 'K':
      return false;
      break;
    default: return false;
    }
  }
};

// Class to represent a hand
// A hand is an array of four cards, with some associated member methods
class Hand {
private:
  // Functions for counting hands, with and without the top card

  
  int count_no_topcard() {
    return 0;
  }

public:
  Card cards[4];
  
  // Default constructor
  Hand() {
    set_values_random();
  };
  
  // Parametrized constructor
  Hand(Card card1,Card card2,Card card3,Card card4) {
    set_values_deterministic(card1,card2,card3,card4);
  }
  
  // Set the values of the cards, deterministically. Mostly for testing
  void set_values_deterministic(Card card1,Card card2,Card card3,Card card4) {
    cards[0] = card1;
    cards[1] = card2;
    cards[2] = card3;
    cards[3] = card4;
  }
  
  // Randomly draw a new card
  static Card random_card() {
    int randint;
    char randsuit, randvalue;
    
    randint = rand() % 13;
    randvalue = values[randint];
    randint = rand() % 4;
    randsuit = suits[randint];
    
    return Card(randsuit,randvalue);
  }
  // Randomly draw a new hand, without replacement
  void set_values_random() {
    // First draw a hand
    int i;
    for (i=0;i<4;i++) {
      cards[i] = random_card();
    }
    
    // Now check the pairs to make sure they aren't equal
    bool someequal = true;
    while (someequal) {
      if (cards[0].compare(cards[1])) {
        cards[1] = random_card();
        continue;
      }
      if (cards[0].compare(cards[2])) {
        cards[2] = random_card();
        continue;
      }
      if (cards[0].compare(cards[3])) {
        cards[3] = random_card();
        continue;
      }
      if (cards[1].compare(cards[2])) {
        cards[2] = random_card();
        continue;
      }
      if (cards[1].compare(cards[3])) {
        cards[3] = random_card();
        continue;
      }
      if (cards[2].compare(cards[3])) {
        cards[3] = random_card();
        continue;
      }
      
      someequal = false;
    }
  }
  
  // Check if a Card is in the hand
  bool in_hand(Card crd) {
    int i;
    for (i=0;i<4;i++) {
      if (cards[i].compare(crd)) {
        return true;
      }
    }
    return false;
  }
  
  // Print out the hand
  void print() {
    std::cout << "Hand is as follows: ";
    int i;
    for (i=0;i<4;i++) {
      cards[i].print();
      std::cout << " ";
    }
    std::cout << "\n";
  }
  
  // Method for sorting an array of cards
  static void swap(Card *xp, Card *yp) 
  { 
    Card temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
  }
  
  bool is_sorted() {
    int i;
    for (i=0;i<3;i++) {
      if (cards[i].order_by_value(cards[i+1]) == 1) {
        return false;
      }
    }
    return true;
  }
  
  static void bubble_sort(Card cards[],int n = 4) {
    bool sorted = false, madeswap = false;
    int i;
    while (!sorted) {
      for (i=0;i<(n-1);i++) {
        if (cards[i].order_by_value(cards[i+1]) == 1) {
          // Swap them
          swap(&cards[i],&cards[i+1]);
          madeswap = true;
        } 
      }
      sorted = !madeswap;
      madeswap = false;
    }
  }
  
  // Count the hand. Two options: provide a top card, or don't.
  // 
  // Don't provide a top card: used in the dealing phase, where you get 6
  // cards and need to decide 2 to discard. 
  int count()  {
    int i, j, k, score=0;
    // Pairs, and 2-card 15's
    for(i=0;i<4;i++) {
      for (j=i+1;j<4;j++) {
        if (cards[i].value == cards[j].value) {
          score+=2;
        }
        if (cards[i].int_value() + cards[j].int_value() == 15) {
          score+=2;
        }
      }
    }
    
    // 3 card 15's. Note triples count as two pairs so they are counted separately, above
    // Do runs separately
    for (i=0;i<4;i++) {
      for (j=i+1;j<4;j++) {
        for(k=j+1;k<4;k++) {
          if (cards[i].int_value() + cards[j].int_value() + cards[k].int_value() == 15) {
            score+=2;
          }
        }
      }
    }
    
    // Four card 15's
    if (cards[0].int_value() + cards[1].int_value() + cards[2].int_value() + cards[3].int_value() == 15) {
      score+=2;
    }
    
    // Runs
    bubble_sort(cards);
    // Now find sequences
    // Just do it manually; there are too many special cases and we need to avoid double counting
    if (cards[0].adjacent_and_less_than(cards[1])) {
      if (cards[1].adjacent_and_less_than(cards[2])) {
        // Now we have a sequence of 3. Check for 4
        if (cards[2].adjacent_and_less_than(cards[3])) {
          score+=4;
        }
        else {
          score+=3;
        }
      }
    }
    // Now check the sequences that do not include card 1
    // Make sure not to double count though, so first check that card[0] is NOT
    // in a sequence with card[1]
    if (!cards[0].adjacent_and_less_than(cards[1])) {
      if (cards[1].adjacent_and_less_than(cards[2])) {
        if (cards[2].adjacent_and_less_than(cards[3])) {
          score+=3;        
        }
      }
    }
    
    // Flushes
    if ( (cards[0].suit == cards[1].suit) &
         (cards[0].suit == cards[2].suit) &
         (cards[0].suit == cards[3].suit) ) {
      score+=4;
    }
    
    return score;
  }
  
  // Provide a top card:
  // Takes an argument, topcard, indicating the value of the card on top of the
  // deck. The reason for this is that in a given round of cribbage, we'll count
  // three hands, all using the same top card.
  int count(Card topcard) {
    int i, j, k, score=0;
    // Pairs, and 2-card 15's
    for(i=0;i<4;i++) {
      // Check the top card
      if (cards[i].value == topcard.value) {
        score+=2;
      }
      if (cards[i].int_value() + topcard.int_value() == 15) {
        score+=2;
      }
      // Check the other cards
      for (j=i+1;j<4;j++) {
        if (cards[i].value == cards[j].value) {
          score+=2;
        }
        if (cards[i].int_value() + cards[j].int_value() == 15) {
          score+=2;
        }
      }
    }
    
    // 3 card 15's. Note triples count as two pairs so they are counted separately, above
    // Do runs separately
    for (i=0;i<4;i++) {
      for (j=i+1;j<4;j++) {
        // Combinations with the top card
        if (cards[i].int_value() + cards[j].int_value() + topcard.int_value() == 15) {
          score+=2;
        }
        for(k=j+1;k<4;k++) {
          if (cards[i].int_value() + cards[j].int_value() + cards[k].int_value() == 15) {
            score+=2;
          }
        }
      }
    }
    
    // Four card 15's
    if (cards[0].int_value() + cards[1].int_value() + cards[2].int_value() + cards[3].int_value() == 15) {
      score+=2;
    }
    if (topcard.int_value() + cards[1].int_value() + cards[2].int_value() + cards[3].int_value() == 15) {
      score+=2;
    }
    if (cards[0].int_value() + topcard.int_value() + cards[2].int_value() + cards[3].int_value() == 15) {
      score+=2;
    }
    if (cards[0].int_value() + cards[1].int_value() + topcard.int_value() + cards[3].int_value() == 15) {
      score+=2;
    }
    if (cards[0].int_value() + cards[1].int_value() + cards[2].int_value() + topcard.int_value() == 15) {
      score+=2;
    }
    
    // Five-card fifteens
    if (cards[0].int_value() + cards[1].int_value() + cards[2].int_value() + cards[3].int_value() + topcard.int_value() == 15) {
      score+=2;
    }
    
    // Runs
    // Create a new array of cards containing the top card
    Card allcards[5];
    for (i=0;i<4;i++) {
      allcards[i] = cards[i];
    }
    allcards[4] = topcard;
    bubble_sort(allcards,5);
    // Now find sequences
    // Just do it manually; there are too many special cases and we need to avoid double counting
    if (allcards[0].adjacent_and_less_than(allcards[1])) {
      if (allcards[1].adjacent_and_less_than(allcards[2])) {
        // Now we have a sequence of 3. Check for 4, or 5
        if (allcards[2].adjacent_and_less_than(allcards[3])) {
          // Now it's a sequence of 4...
          if (allcards[3].adjacent_and_less_than(allcards[4])) {
            score+=5;
          }
          else {
            score+=4;
          }
        }
        else {
          score+=3;
        }
      }
    }
    // Now check the sequences that do not include card 1
    // Make sure not to double count though, so first check that card[0] is NOT
    // in a sequence with card[1]
    if (!allcards[0].adjacent_and_less_than(allcards[1])) {
      if (allcards[1].adjacent_and_less_than(allcards[2])) {
        if (allcards[2].adjacent_and_less_than(allcards[3])) {
          // Now it's a sequence of 3...
          if (allcards[3].adjacent_and_less_than(allcards[4])) {
            score+=4;
          }
          else {
            score+=3;
          }
        }
      }
    }
    // Last possibility is a sequence of 3 formed by hte last 3 cards
    if (!allcards[0].adjacent_and_less_than(allcards[1])) {
      if (!allcards[1].adjacent_and_less_than(allcards[2])) {
        if (allcards[2].adjacent_and_less_than(allcards[3])) {
          if (allcards[3].adjacent_and_less_than(allcards[4])) {
            score+=3;
          }
        }
      }
    }
    
    // Flushes
    if ( (cards[0].suit == cards[1].suit) &
         (cards[0].suit == cards[2].suit) &
         (cards[0].suit == cards[3].suit) ) {
      // If the four cards in the hand are the same suit, check if the top card
      // is also the same suit. If yes, 5 points, if no, then 4
      if (cards[0].suit == topcard.suit) {
        score+=5;
      }
      else {
        score+=4;
      }
    }
    
    // Finally, "Nobs"- check if hand has a jack of the same suit as the top card
    for (i=0;i<4;i++) {
      if (cards[i].value == 'J') {
        if (cards[i].suit == topcard.suit) {
          score++;
        }
      }
    }
    
    return score;
  }
};

// Draw any number of cards, without replacement
std::vector<Card> draw_cards(int n) {
  if (n > 52) {
    std::cout << "Can't draw " << n << " distinct cards from a 52 card deck!\n";
    exit(1);
  }
  std::vector<Card> cards;
  
  // Randomly draw cards
  int i, j;
  for (i=0;i<n;i++) {
    cards.push_back(Hand::random_card());
  }
  // Check if any are duplicate; remove duplicates
  bool somethesame = true;
  while(somethesame) {
    somethesame = false;
    // Check if any are the same
    for (i=0;i<n;i++) {
      for (j=i+1;j<n;j++) {
        if (cards[i].compare(cards[j])) {
          cards[j] = Hand::random_card();
          somethesame = true;
        }
      }
    }
  }
  
  return cards;
}

// Take a vector of six cards, return the Hand with the highest score

Hand best_hand(std::vector<Card> cards) {
  int curscore, bestscore = 0;
  int i, j, k, l;
  int n = cards.size();
  Hand curhand, besthand;
  
  for (i=0;i<n;i++) {
    for (j=i+1;j<n;j++) {
      for (k=j+1;k<n;k++) {
        for (l=k+1;l<n;l++) {
          curhand = Hand(cards[i],cards[j],cards[k],cards[l]);
          curscore = curhand.count();
          if (curscore > bestscore) {
            bestscore = curscore;
            besthand = curhand;
          }
        }
      }
    }
  }
  
  return besthand;
}

int main() {
  
  // Set the random seed
  srand(time(NULL));
  
  /* Code for creating the standard drawing simulation
  // Open the output file
  std::ofstream outputfile;
  outputfile.open(filepath);
  outputfile << "score\n";
  
  // Generate and count a bunch of hands
  int i, n = 1000000;
  
  for (i=0;i<n;i++) {
    // Get a random hand
    Hand thehand;
    
    // Generate the top of deck card
    Card topcard;
    topcard = Hand::random_card();
    while(thehand.in_hand(topcard)) {
      topcard = Hand::random_card();
    }
    
    // Count the hand
    outputfile << thehand.count(topcard) << "\n";
  }
  */
  
  // Simulate the actual turn- draw 6 cards, take the best sub-hand, record its points
  // Open the output file
  std::ofstream outputfile;
  outputfile.open(filepath);
  outputfile << "score\n"; // Write the header
  
  // Generate and count a bunch of hands
  int i, n = 1000000;
  
  for (i=0;i<n;i++) {
    // Get a vector of six cards
    std::vector<Card> drawncards = draw_cards(6);
    // Get the best hand
    Hand besthand = best_hand(drawncards);
    // Draw the top card
    Card topcard;
    topcard = Hand::random_card();
    while(besthand.in_hand(topcard)) {
      topcard = Hand::random_card();
    }
    // Write the score to the file
    outputfile << besthand.count(topcard) << "\n";
  }

}
