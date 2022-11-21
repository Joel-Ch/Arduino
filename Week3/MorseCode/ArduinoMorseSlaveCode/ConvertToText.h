#include <stdio.h>
#include <string.h>

char convertToText(char morsePhrase[], char Phrase[])
{
    const char text[36] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    char morse[36][7] = {".-", "-...", "-.-.", "-..", ".", "..-", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", "...--", "....-", ".....", "-....", "--....", "---..", "----.", "-----"};

    char *morseWord = strtok(morsePhrase, "/");
    /* walk through other tokens */
    while (morseWord != NULL)
    {
        for (int counter = 0; counter < 36; counter++)
        {
            if (strcmp(morseWord, morse[counter]) == 0)
            {
                char letter[2];
                letter[0] = text[counter];
                letter[1] = '\0';

                strcat(Phrase, letter);
            }
        }
        morseWord = strtok(NULL, "/");
    }
    return (Phrase[500]);
}