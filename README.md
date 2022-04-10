# Hangman

A CLI hangman game\

By default, it will use the `requests` module to fetch random words and hints. You can disable the use of `requests` module by running it with the `offline` command-line argument and disable the hints with the `no-hints` command-line argument.\
To disable `requests` module AND play with no hints, for instance:
```
python hangman.py offline no-hints
```
Default usage:
```
python hangman.py
How many tries do you want to guess the word? 7

A feeler organ on the head of an insect, crab, or other animal.
Tries: 7
You haven't used any letters yet
- - - - - - -
Letter: a
Correct!

[...]

A feeler organ on the head of an insect, crab, or other animal.
Tries: 6
You have used the letters: e, f, n, a
a n - e n n a
Letter: t
Correct!

Congratulations! You guessed "antenna" with 6 tries left!
Play again? (Y/n):
```
Random words were gotten from [Random Word API](https://random-word-api.herokuapp.com/home) and their meanings from [Free Dictionary API](https://dictionaryapi.dev/)