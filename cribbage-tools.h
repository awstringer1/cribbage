/*
 * Program to randomly sample cribbage hands and count their points
 */

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <regex>
#include <boost/algorithm/string.hpp>
//#include <boost/algorithm/string_regex.hpp>


static std::string filepath = "/Users/alexstringer/phd/projects/learn-cpp/cribbage/score-run-best-subhand.csv";

// Define all the possible cards
static char values[13] = {'A','2','3','4','5','6','7','8','9','0','J','Q','K'};
static char suits[4] = {'H','C','D','S'};

// Class to represent a card
// A card has a value and a suit
class Card {
private:
  // Check valid values
  void valid_values(char suit,char value) {
    bool valid_suit = false, valid_value = false;
    int i;
    for (i=0;i<13;i++) {
      if (value == values[i]) {
        valid_value = true;
      }
    }
    for (i=0;i<4;i++) {
      if (suit == suits[i]) {
        valid_suit = true;
      }
    }

    if (!valid_suit) {
      std::cout << "Invalid suit: " << suit << "\n";
      exit(1);
    }
    if (!valid_value) {
      std::cout << "Invalid value: " << value << "\n";
      exit(1);
    }
  }
  // Initialize
  void set_values(char setsuit,char setvalue) {
    // Check if values are valid
    valid_values(setsuit,setvalue);

    suit = setsuit;
    value = setvalue;
  }
  // Can also provide a string, and set values by parsing suit and value
  void set_values(std::string input) {
    //std::cout << input << "\n";
    // Regex for matching the suit and value
    // Note that std::regex_search returns one match at a time, so the below
    // code returns only the first match from the input for each regex (suit, value).
    // I don't see any reason to guard against this.
    std::smatch sm_suit, sm_value;
    std::regex re_suit("(H|S|D|C)");
    std::regex re_value("(0|[2-9]|A|J|Q|K)");
    std::regex_search(input,sm_suit,re_suit);
    std::regex_search(input,sm_value,re_value);

    std::string setvalue = sm_value[0];
    std::string setsuit = sm_suit[0];

    if (sm_value.empty()) {
      std::cout << "Parsing value from input " << input << " unsuccessful.\n";
      exit(1);
    }
    if (sm_suit.empty()) {
      std::cout << "Parsing suit from input " << input << " unsuccessful.\n";
      exit(1);
    }



    char *setsuit_c = &setsuit[0];
    // If the value is a 10, make it a 0
    char *setvalue_c = &setvalue[0];
    if (setvalue.compare(std::string ("10")) == 0) {
      *setvalue_c = '0';
    }
    else {
      setvalue_c = &setvalue[0]; // Redundant, I know.
    }
    // Check if values are valid
    valid_values(*setsuit_c,*setvalue_c);
    suit = *setsuit_c;
    value = *setvalue_c;


  }
public:
  char suit;
  char value;

  // Default constructor
  Card() {

  }
  // Parametrized contructors
  Card(char setsuit,char setvalue) {
    set_values(setsuit,setvalue);
  }

  Card(std::string input) {
    set_values(input);
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

  // Parametrized constructors
  Hand(Card card1,Card card2,Card card3,Card card4) {
    set_values_deterministic(card1,card2,card3,card4);
  }

  Hand(Card cards[]) {
    set_values_deterministic(cards[0],cards[1],cards[2],cards[3]);
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

// Class to represent a player
// A player has a score, and a Hand
class Player {
private:
  Hand currenthand;
public:
  int score = 0;

  void set_hand(Hand thehand) {
    currenthand = thehand;
  }

  Hand get_hand() {
    return currenthand;
  }

  // Is the current player the dealer?
  bool is_dealer = false;
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

// Request discard input from the user
// Display 6 cards to the user, and ask them to discard 2
// Return the hand containing the four remaining cards
Hand request_discard(std::vector<Card> cards) {
  // Make sure 6 cards provided
  if (cards.size() != 6) {
    std::cout << "Please provide a std::vector containing 6 cards. Your input contained " << cards.size() << " cards.\n";
    exit(1);
  }
  // Display the cards
  int i;
  std::cout << "You are dealt the following 6 cards: ";
  for (i=0;i<6;i++) {
    cards[i].print(); std::cout << " ";
  }
  std::cout << "\nPlease choose two to discard. Type your choices in the format shown, separated by a space.\n";

  // Request input from the user
  std::string user_input;
  std::getline(std::cin,user_input);

  // Parse the input into Card objects
  std::vector<std::string> user_input_split;
  boost::split(user_input_split,user_input,boost::is_any_of(" "));
  Card discard1 = Card(user_input_split[0]), discard2 = Card(user_input_split[1]);
  std::cout << "Discarding ";
  discard1.print(); std::cout << " and "; discard2.print();
  std::cout << " into the crib." << std::endl;

  // Return the hand after discarding
  Card goodcards[4];
  int k=0;
  for (i=0;i<6;i++) {
    if (cards[i].compare(discard1) | cards[i].compare(discard2)) {
      continue;
    }
    else {
      goodcards[k] = cards[i];
      k++;
    }
  }

  return Hand(goodcards);


}

// Deal. Draw 12 cards without replacement, split into two 6-card arrays; get the
// two best four-card subhands from this; assign them to the supplied players. The
// four discarded cards make up the crib; return these as a Hand
Hand deal(Player *player1, Player *player2,bool interactive = false) {
  // Draw 12 cards
  std::vector<Card> cards = draw_cards(12);
  // Split them into 2 vectors
  int i;
  std::vector<Card> player1cards, player2cards;
  std::cout << "12 cards drawn: ";
  for (i=0;i<6;i++) {
    cards[i].print(); std::cout << " ";
    cards[i+6].print(); std::cout << " ";
    player1cards.push_back(cards[i]);
    player2cards.push_back(cards[i+6]);
  }
  std::cout << std::endl;

  // Get the best hands, assign to players
  if (interactive) {
    player1->set_hand(request_discard(player1cards));
  }
  else {
    player1->set_hand(best_hand(player1cards));
  }
  player2->set_hand(best_hand(player2cards));

  // for (i=0; i<6; i++) {
  //   player1cards[i].print(); std::cout << " ";
  // }
  // std::cout << "\n";
  // for (i=0; i<6; i++) {
  //   player2cards[i].print(); std::cout << " ";
  // }
  // std::cout << "\n";

  // Return the crib
  std::vector<Card> thecrib;
  for (i=0;i<6;i++) {
    // Check if it was included in a player's hand; if not, add to the crib
    if (!(player1->get_hand().in_hand(player1cards[i]))) {
      // std::cout << " Card "; player1cards[i].print(); std::cout << " is not in player 1's hand.\n";
      thecrib.push_back(player1cards[i]);
    }
    if (!(player2->get_hand().in_hand(player2cards[i]))) {
      // std::cout << " Card "; player2cards[i].print(); std::cout << " is not in player 2's hand.\n";
      thecrib.push_back(player2cards[i]);
    }
  }

  // Make sure that worked
  if (thecrib.size() != 4) {
    // std::cout << "Crib dealing did not work, crib contains " << thecrib.size() << " cards.\n";
    exit(1);
  }

  return Hand(thecrib[0],thecrib[1],thecrib[2],thecrib[3]);
}

/* The old main() function from testing
int main() {

  // Set the random seed
  srand(time(NULL));

  // Code for creating the standard drawing simulation
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
*/
