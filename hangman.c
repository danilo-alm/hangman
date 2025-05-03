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
void print_usage(char* argv[]);

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        print_usage(argv);
        return 1;    
    }

    srand(time(NULL));

    char *word = get_random_word(argc == 2 ? argv[1] : "words.txt");
    if (word == NULL) {
        fprintf(stderr, "Error reading words file.\n");
        print_usage(argv);
        return 1;
    }

    short word_letters[ALPHABET_LEN] = { 0 };
    short used_letters[ALPHABET_LEN] = { 0 };
    short letters_remaining = 0;
    short tries = 5;

    int i = 0;
    for (char c = word[i]; c != '\0'; c = word[++i]) {
        short idx = get_letter_index(c);
        if (idx == -1)
        {
            fprintf(stderr, "Invalid character in word: %c\n", c);
            free(word);
            return 1;
        }

        if (word_letters[idx] == 0) {
            word_letters[idx] = 1;
            letters_remaining++;
        }
    }

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
        printf("\nYou won!\n");
    }
    else
    {
        printf("\nYou lost! The word was %s\n", word);
    }
    
    free(word);
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

void print_usage(char* argv[])
{
    printf("Usage: %s <words_file>\n", argv[0]);
}