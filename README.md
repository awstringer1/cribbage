# Cribbage Hands

This repository contains C++ code for randomly generating and counting points in hands of cribbage. I coded this up because I was interested in seeing the distribution of points in cribbage hands.

Here is a plot of the proportions of each score seen in 1,000,000 runs. The highest score is 29 (!), and about 65% of scores were either 2, 4, or 6. This makes sense; 15's and pairs are the most common type of score, and in order to get an odd number, you need to get a run of three (or five!) or have a jack with the same suit as the starter card. Both of these situations are less common than the two-point situations.

![Cribbage Scores, 1,000,000 hands](https://github.com/awstringer1/cribbage/blob/master/score-run-plot.png)
