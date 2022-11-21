#include <stdio.h>
#include <string.h>

char convertToMorse(char phrase[], char morsePhrase[])
{
    char text[36] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    char morse[36][7] = {".-", "-...", "-.-.", "-..", ".", "..-", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", "...--", "....-", ".....", "-....", "--....", "---..", "----.", "-----"};

    for (int i = 0; i < strlen(phrase); ++i)
    {
        for (int counter = 0; counter < 36; counter++)
        {
            if (phrase[i] == text[counter])
            {
                char *morseLetter = morse[counter];
                strcat(morsePhrase, morseLetter);
                strcat(morsePhrase, "/");
            }
        }
    }
    return (morsePhrase[200]);
}