# Read in the cribbage score file and plot the score distribution
# 
library(tidyverse)

# Read in the cribbage scores
scores <- readr::read_csv(file = "/Users/alexstringer/phd/projects/learn-cpp/cribbage/score-run.csv")
scores_besthand <- readr::read_csv(file = "/Users/alexstringer/phd/projects/learn-cpp/cribbage/score-run-best-subhand.csv")
# Summary statistics
summary(scores$score)

# Histogram
scores %>%
  ggplot(aes(x = score)) +
  theme_light() +
  geom_bar(aes(y = ..count../sum(..count..)),colour = "black",fill = "royalblue") +
  labs(title = "Distribution of cribbage hand scores",
       subtitle = str_c(scales::comma(nrow(scores))," hands"),
       x = "Score",
       y = "% of hands") +
  scale_y_continuous(labels = scales::percent_format()) +
  scale_x_continuous(breaks = 0:30)

# Save it
ggsave(filename = "/Users/alexstringer/phd/projects/learn-cpp/cribbage/score-run-plot.png")

# Best subhand
scores_besthand %>%
  ggplot(aes(x = score)) +
  theme_light() +
  geom_bar(aes(y = ..count../sum(..count..)),colour = "black",fill = "royalblue") +
  labs(title = "Distribution of cribbage hand scores, best 4-card subhand from 6-card draws",
       subtitle = str_c(scales::comma(nrow(scores))," hands"),
       x = "Score",
       y = "% of hands") +
  scale_y_continuous(labels = scales::percent_format()) +
  scale_x_continuous(breaks = 0:30)

# Save it
ggsave(filename = "/Users/alexstringer/phd/projects/learn-cpp/cribbage/score-run-best-subhand-plot.png")


