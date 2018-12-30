// Unit tests for cribbage code.

#include "cribbage-tools.h"
#include "gtest/gtest.h"
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <regex>
#include <boost/algorithm/string.hpp>

// Test card construction
TEST(CardConstruction,GoodCards) {
  Card card1 = Card('S','A');
  EXPECT_EQ(card1.value,'A');
  EXPECT_EQ(card1.suit,'S');
}

int main() {
  return 0;
}
