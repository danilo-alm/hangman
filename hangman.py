import sys


def main():
    is_offline = False
    has_hints = True

    if "offline" in sys.argv:
        is_offline = True
    if "no-hints" in sys.argv:
        has_hints = False

    # Check if user has requests module to play online
    if not is_offline:
        import pkg_resources

        required = {'requests'}
        installed = {pkg.key for pkg in pkg_resources.working_set}
        missing = required - installed

        if missing:
            print("You need the `requests` module to play online\n"
            "Install it with \"python -m pip install requests\"")
            exit(1)

    while True:
        tries = input("How many tries do you want to guess the word? ")
        # Check if user typed a valid number
        if not tries.isdigit():
            continue

        try:
            play(int(tries), is_offline, has_hints)
        except KeyboardInterrupt:
            print("\n-----------------------------------")
            None

        play_again = input("Play again? (Y/n): ").lower()
        if play_again not in ["y", "yes", ""]:
            print("Thanks for playing!")
            exit(0)


def play(tries, is_offline, has_hints):

    if is_offline:
        from words import words_and_meanings
        from random import choice

        # Get random word and hint from words.py
        word = choice(list(words_and_meanings.keys())).lower()
        if has_hints:
            hint = words_and_meanings[word]

    else:
        import requests
        while True:
            # Get random word
            req = requests.get("https://random-words-api.herokuapp.com/w?n=1&swear=0")

            if req.status_code != 200:
                # Unable to continue game without a word
                print("\nError: coudn't get word.\nIf you're having trouble to play "
                "online, run the program with the `offline` command-line argument")
                return None

            word = req.json()[0].lower()

            # We will look the word the word in the dictionary even if the user has
            # opted not to see any hint, since that's a great way of checking
            # whether a word is valid

            # Get word meaning. It will work as a hint
            req = requests.get("https://api.dictionaryapi.dev/api/v2/entries/en/" + word)

            if req.status_code != 200:
                # Continue game without hints
                print("Couldn't get word hint")
                has_hints = False
                break

            json_request = req.json()

            # When no meaning is found, it returns just a dict
            # instead of a dict inside a list
            if isinstance(json_request, dict):
                # If no meaning was found, it's probably an invalid word
                # Get another word
                continue

            hint = req.json()[0]["meanings"][0]["definitions"][0]["definition"]

            break

    # When user guesses a letter, we'll remove it from `word_letters`
    word_letters = set(letter for letter in word)
    used_letters = set()
    letter_already_used = False

    while (tries > 0 and len(word_letters) > 0):

        # Keep track of number of lines we'll have to clear on each iteration
        # We'll increment that if we need to print more lines than expected
        lines_to_clear = (6 if has_hints else 5)

        print()
        if has_hints:
            print("HINT: " + hint)

        # Print tries left
        print(f"TRIES: {tries} tries left")

        # Print used letters
        if len(used_letters) > 0:
            print("Letters used: " + ", ".join(used_letters))
        else:
            print("You haven't used any letters yet")

        print()

        # Print word
        w = [letter if letter in used_letters else '-' for letter in word]
        print("WORD: " + " ".join(w))

        # Check if the letter that the user typed previously had already been used
        if letter_already_used:
            print("You have already used that letter.")
            lines_to_clear += 1
            letter_already_used = False

        # Ask user for input
        while True:
            letter = input("Letter: ").lower()
            lines_to_clear += 1
            if (len(letter) == 1 and letter.isalpha() == True):
                break

        # Check if letter is valid
        if not (letter in used_letters):
            used_letters.add(letter)
            if letter in word_letters:
                word_letters.remove(letter)
            else:
                tries -= 1
        else:
            letter_already_used = True

        # Clear what we wrote
        clear_last_lines(lines_to_clear)

    if tries > 0 and len(word_letters) == 0:
        print(f"\nCongratulations! You guessed \"{word}\" with {tries} tries left!")
    else:
        print(f"\n{tries} tries! You lose! The word was {word}")

    return None


# Clears `num_of_lines` lines from console
def clear_last_lines(num_of_lines: int) -> None:
    if num_of_lines != 0:
        # Move cursor back to previous line
        sys.stdout.write('\x1b[1A')

        # Clear line
        sys.stdout.write('\x1b[2K')
    
        return clear_last_lines(num_of_lines - 1)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nExiting...")
        exit(0)
