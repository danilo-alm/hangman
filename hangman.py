import sys

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

            if has_hints:
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

    while (tries > 0 and len(word_letters) > 0):

        print()
        if has_hints:
            print(hint)

        # Print tries left
        print("Tries: " + str(tries))

        # Print used letters
        if len(used_letters) > 0:
            print("You have used the letters: " + ", ".join(used_letters))
        else:
            print("You haven't used any letters yet")

        # Print word
        w = [letter if letter in used_letters else '-' for letter in word]
        print(" ".join(w))

        # Ask user for input
        while True:
            letter = input("Letter: ").lower()

            if (len(letter) == 1 and letter.isalpha() == True):
                break

        # Check if letter is valid
        if (letter in used_letters):
            print("You already used that letter")
        else:
            used_letters.add(letter)
            if letter in word_letters:
                print("Correct!")
                word_letters.remove(letter)
            else:
                print("Wrong!")
                tries -= 1

    if tries > 0 and len(word_letters) == 0:
        print(f"\nCongratulations! You guessed \"{word}\" with {tries} tries left!")
    else:
        print(f"\n{tries} tries! You lose! The word was {word}")

    return None


if __name__ == "__main__":

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
        try:
            tries = input("How many tries do you want to guess the word? ")
            # Check if user typed a valid number
            if not tries.isdigit():
                continue

            play(int(tries), is_offline, has_hints)

            play_again = input("Play again? (Y/n): ").lower()
            if play_again not in ["y", "yes", ""]:
                print("Thanks for playing!")
                exit(0)

        except KeyboardInterrupt:
            print("\nExiting...")
            exit(0)
