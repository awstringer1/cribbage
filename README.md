# Cribbage Hands

This repository contains C++ code for randomly generating and counting points in hands of cribbage. I coded this up because I was interested in seeing the distribution of points in cribbage hands.

Here is a plot of the proportions of each score seen in 1,000,000 runs. The highest score is 29 (!), and about 65% of scores were either 2, 4, or 6. This makes sense; 15's and pairs are the most common type of score, and in order to get an odd number, you need to get a run of three (or five!) or have a jack with the same suit as the starter card. Both of these situations are less common than the two-point situations.

![Cribbage Scores, 1,000,000 hands](https://github.com/awstringer1/cribbage/blob/master/score-run-plot.png)

Some simulating shows the 29 point hand is legit. It occurs when you have 3 fives in your hand, the starter card is a five, and the fourth card in your hand is a jack of the same suit as the starter card. This gives you

- 4 15's, got from the 4 possible combinations of three fives (8 points)
- 4 more 15's, got from the 4 possible ways to pair a five with the jack (8 points)
- Four of a kind, which counts as 6 pairs, so 12 points
- Nobs: a jack of the same suit as the starter card is worth 1 point

If you've played cribbage, you might notice a couple things about this plot. First, about 8% of the hands achieve zero points- how often does that actually happen in a real game? Second, the average is around 4 points (4.5, actually), and since you need 121 points to win, that would be an average of about 27 turns per game (okay, I know it's less because the game actually goes until the max of player 1 or player 2's points is >= 121, but it won't be *that* much less), which seems pretty long. What's the problem?

The problem is in an actual cribbage game we don't deal 4 cards- we deal 6 cards, and then (attempt to) pick the best sub hand out of these, without looking at the starter. Coding this functionality into the game and re-running the simulation gives the following:

![Cribbage Scores, 1,000,000 hands, drawing best 4-card sub-hand from 6 drawn cards](https::/github.com/awstringer1/cribbage/blob/master/score-run-best-subhand-plot.png)

This looks better- almost no zeroes, and an average score of 7.255, which corresponds to a 17-turn game on average (ignoring the max(player1,player2) thing, for now). This seems more realistic. 
