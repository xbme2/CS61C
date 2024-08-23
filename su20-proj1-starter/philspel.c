/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philspel.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;
const int WORD_SIZE = 96;
/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in
 * the grading process.
 */
int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2255, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  Void *s can be safely cast
 * to a char * (null terminated string) and is already done for you here
 * for convenience.
 */
unsigned int stringHash(void *s)
{
  char *string = (char *)s;
  unsigned int sum = 0;
  while (*string++ != '\0')
  {
    sum = (unsigned int)*string + sum % 2255;
  }
  return sum % 2255;
}

/*
 * This should return a nonzero value if the two strings are identical
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2)
{
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  if (strlen(string1) != strlen(string2))
    return 0;
  while (*string1 != '\0' && *string2 != '\0')
  {
    if (*string1 != *string2)
      return 0;
    string1++;
    string2++;
  }
  return 1;
}

/*
 * This function should read in every word from the dictionary and
 * store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(1)
 * to cleanly exit the program.
 *
 * Since the format is one word at a time, with new lines in between,
 * you can safely use fscanf() to read in the strings until you want to handle
 * arbitrarily long dictionary chacaters.
 */
void readDictionary(char *dictName)
{
  FILE *fp = fopen(dictName, "r");
  char *buffer = malloc(WORD_SIZE + 1);
  if (fp == NULL)
  {
    fprintf(stderr, "can not open this file!");
    exit(1);
  }
  while (fgets(buffer, WORD_SIZE, fp) != NULL)
  {
    *(buffer + strlen(buffer) - 1) = '\0';
    // printf("%s",buffer);
    char *word = malloc(WORD_SIZE + 1);
    strcpy(word, buffer);
    if (findData(dictionary, word) == NULL)
      insertData(dictionary, word, word);
    memset(buffer, 0, WORD_SIZE + 1);
  }

  fclose(fp);
  free(buffer);
  // test
  //  printf("it is a test\n");
  //  for (size_t i = 0; i < dictionary->size; i++)
  //  {
  //    if(dictionary->data[i]) printf("%s\n",(char*)dictionary->data[i]->data);
  //  }
}

/*
 * This should process standard input (stdin) and copy it to standard
 * output (stdout) as specified in the spec (e.g., if a standard
 * dictionary was used and the string "this is a taest of  this-proGram"
 * was given to stdin, the output to stdout should be
 * "this is a teast [sic] of  this-proGram").  All words should be checked
 * against the dictionary as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not found by appending " [sic]" after the error.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the
 * final 20% of your grade, you cannot assume words have a bounded length.
 */
void processInput()
{
  char *sentence = (char *)malloc((1024) * sizeof(char));
  char *case1 = (char *)malloc((WORD_SIZE + 1) * sizeof(char));
  char *case2 = (char *)malloc((WORD_SIZE + 1) * sizeof(char));
  char *case3 = (char *)malloc((WORD_SIZE + 1) * sizeof(char));
  while (fgets(sentence, 1024, stdin) != NULL) // unix only allow fgets
  {
    size_t index = 0;
    // fprintf(stdout,"lenth of inputStr is %ld\n",strlen(sentence));
    for (size_t i = 0; i < strlen(sentence); i++)
    {
      if (isalpha(sentence[i]))
      {
        case1[index] = sentence[i];
        case2[index] = sentence[i];
        case3[index] = tolower(sentence[i]);
        if (index != 0)
          case2[index] = tolower(case2[index]);
        index++;
      }
      else
      {
        if (index == 0)
        {
          fprintf(stdout, "%c", sentence[i]);
          continue;
        }
        else
        {
          case1[index] = case2[index] = case3[index] = '\0';
          index = 0;
          if (findData(dictionary, case1) || findData(dictionary, case2) || findData(dictionary, case3))
          {
            fprintf(stdout, "%s%c", case1, sentence[i]);
            continue;
          }
          else
          {
            fprintf(stdout, "%s [sic]%c", case1, sentence[i]);
          }
        }
      }
    }
    case1[index] = case2[index] = case3[index] = '\0';
    if (findData(dictionary, case1) || findData(dictionary, case2) || findData(dictionary, case3))
    {
      fprintf(stdout, "%s", case1);
    }
    else if (strlen(case1) > 0)
    {
      fprintf(stdout, "%s [sic]", case1);
    }
  }
  free(sentence);
  free(case1);
  free(case2);
  free(case3);
}