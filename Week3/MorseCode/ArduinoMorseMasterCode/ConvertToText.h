#include <stdio.h>
#include <string.h>

void convertToText(char morsePhrase[], char Phrase[])
{
    const char text[36] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    char morse[36][7] = {".-", "-...", "-.-.", "-..", ".", "..-", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", "...--", "....-", ".....", "-....", "--....", "---..", "----.", "-----"};

    char *morseWord = strtok(morsePhrase, "/");
    //checks for the next slash and splits the string before it, to enable checking of individual letters
    while (morseWord != NULL)
    {
        for (int counter = 0; counter < 36; counter++)
        {//go through each letter and see if its equal
            if (strcmp(morseWord, morse[counter]) == 0)
            {//if equal add it to the end of "Phrase"
                char letter[2];
                letter[0] = text[counter];
                letter[1] = '\0';
                // converting from part of an array to a full string (adding a null character)
                strcat(Phrase, letter);
            }
        }
        morseWord = strtok(NULL, "/");
        // checks for the next slash to select the next word in the message
    }
}