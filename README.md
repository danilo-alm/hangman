# Hangman

A simple terminal-based Hangman game implemented in C. The game selects a random word from a file and lets the user guess letters, tracking incorrect guesses.

## Features

* Random word selection from a file
* Case-insensitive input
* Displays correctly guessed letters and tracks previously used ones
* 5 incorrect attempts allowed before game over

## Usage

### Compile

```bash
gcc -o hangman hangman.c
```

### Run

```bash
./hangman [words_file]
```

* `words_file` (optional): A text file containing one word per line. Defaults to `words.txt` if not provided.

### Example

```bash
./hangman words.txt
```

## Word File Format

* One word per line
* Only alphabetic characters (no digits or symbols)
* All letters should be in the standard Latin alphabet (A–Z or a–z)

**Example `words.txt`:**

```
apple
banana
cherry
grape
```

## Controls

* Type one letter and press `Enter` to guess
* Letters already guessed are ignored
* The game ends when you guess all letters or run out of tries

## Notes

* Input is not case-sensitive
* Only letters A–Z are accepted
