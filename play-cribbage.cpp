/*
 * Actually play a game of cribbage
 *
*/

#include <iostream>
#include <iterator>
#include <vector>

#include "cribbage-tools.h"

int main() {

  srand(time(NULL));
  /* The game proceeds as follows:
   * - Generate two players; initialize their scores to 0
   * - Deal two hands, without replacement, of 6 cards each
   * - Discard two cards per hand; form the crib from the discarded cards
   * - Perform the pegging phase
   * - Perform the counting phase
   * - Switch dealers and repeat
   */

  // Create the players
  Player player1, player2;

  Hand thecrib = deal(&player1,&player2,true);
  std::cout << "Player 1 hand: ";
  player1.get_hand().print();
  std::cout << "Beginning pegging phase" << std::endl;
  pegging_phase(&player1,&player2,true);

}
