# Cribbage Hands

This repository contains C++ code for randomly generating and counting points in hands of cribbage. I coded this up because I was interested in seeing the distribution of points in cribbage hands.

Here is a plot of the proportions of each score seen in 1,000,000 runs. The highest score is 29 (!), and about 65% of scores were either 2, 4, or 6. This makes sense; 15's and pairs are the most common type of score, and in order to get an odd number, you need to get a run of three (or five!) or have a jack with the same suit as the starter card. Both of these situations are less common than the two-point situations.

![Cribbage Scores, 1,000,000 hands](https://github.com/awstringer1/cribbage/blob/master/score-run-plot.png)

Some simulating shows the 29 point hand is legit. It occurs when you have 3 fives in your hand, the top card is a five, and the fourth card in your hand is a jack of the same suit as the top card. This gives you

- 4 15's, got from the 4 possible combinations of three fives (8 points)
- 4 more 15's, got from the 4 possible ways to pair a five with the jack (8 points)
- Four of a kind, which counts as 6 pairs, so 12 points
- Nobs: a jack of the same suit as the top card is worth 1 point

 
