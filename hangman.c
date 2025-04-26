#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ALPHABET_LEN 26
#define MAX_WORD_LEN 100

short get_letter_index(char letter);
bool check_letter(char letter, short letters[]);
void clear_buffer();
char* get_random_word(const char *filename);

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Usage: %s <tries>\n", argv[0]);
        return 1;
    }

    char *word = get_random_word("words.txt");
    if (word == NULL) {
        fprintf(stderr, "Error reading words file.\n");
        return 1;
    }

    short word_letters[ALPHABET_LEN] = { 0 };
    short used_letters[ALPHABET_LEN] = { 0 };
    unsigned short tries = argc == 2 ? (unsigned short) atol(argv[1]) : 5;

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

        printf("\n\nTries remaining: %d\nLetter: ", tries);
        char c = getchar();
        if (c != '\n') clear_buffer();

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

char* get_random_word(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    char buffer[MAX_WORD_LEN];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file))
        count++;

    if (count == 0) {
        fclose(file);
        return NULL;
    }

    int target = rand() % count;
    rewind(file);

    int i = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {
        if (i++ == target)
        {
            buffer[strcspn(buffer, "\n")] = '\0';
            char *word = malloc(strlen(buffer) + 1);
            strcpy(word, buffer);
            fclose(file);
            return word;
        }
    }

    fclose(file);
    return NULL;
}