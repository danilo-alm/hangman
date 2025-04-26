#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_LEN 26

short get_letter_index(char letter);
bool check_letter(char letter, short letters[]);
void clear_buffer();

int main(void)
{
    char word[] = "testando";

    short word_letters[ALPHABET_LEN] = { 0 };
    short used_letters[ALPHABET_LEN] = { 0 };
    short tries = 5;

    int i = 0;
    for (char c = word[i]; c != '\0'; c = word[++i]) {
        short idx = get_letter_index(c);
        word_letters[idx] = 1;
    }
    short letters_remaining = i;

    while (tries > 0 && letters_remaining > 0)
    {
        printf("\n");
        int i = 0;
        for (char c = word[i]; c != '\0'; c = word[++i])
        {
            if (check_letter(c, used_letters))
            {
                printf("%c ", c);
            }
            else
            {
                printf("_ ");
            }
        }

        printf("Tries remaining: %d\n", tries);
        printf("\nLetter: ");
        char c = getchar();
        clear_buffer();

        if (check_letter(c, used_letters))
        {
            printf("Letter already used\n");
            continue;
        }

        short idx = get_letter_index(c);
        if (idx == -1)
        {
            printf("Invalid letter\n");
            continue;
        }

        used_letters[idx] = 1;
        if (word_letters[idx] == 1)
        {
            printf("Letter found!\n");
            letters_remaining--;
        }
        else
        {
            printf("Nope\n");
            tries--;
        }
    }

    if (letters_remaining == 0)
    {
        printf("You won!\n");
    }
    else
    {
        printf("You lost!\n");
    }
    
    return 0;
}

short get_letter_index(char letter)
{
    if (letter >= 'a' && letter <= 'z')
    {
        return letter - 'a';
    }
    else if (letter >= 'A' && letter <= 'Z')
    {
        return letter - 'A';
    }
    return -1;
}

bool check_letter(char letter, short letters[])
{
    short idx = get_letter_index(letter);
    return idx != -1 && letters[idx] == 1;
}

void clear_buffer()
{
    while (getchar() != '\n');
}
