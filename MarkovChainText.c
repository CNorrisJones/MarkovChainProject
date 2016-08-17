#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//WORD COUNT FUNCTION
//
//

int filewordcount(FILE* file) {
	int count = 0;
	char* temp;
	
	//Check if file actually exists and was read properly
	
	if (file == NULL){
		printf("File couldn't be accessed, wordnumber function failed!\n");
		return 0;
	}
	
	temp = malloc(180 * sizeof(char));
	
	//Run through File, upping count for each word (string) found
	while (fscanf(file, "%s", temp) != EOF) {
		count += 1;
		free(temp);
		temp = malloc(180 * sizeof(char));
	} 
	//Free up used memory, return the count result, which is the total number of words in file
	free(temp);
	return count;
}

//STRUCT FARM
//
//

struct wordstruct{
	char* wordname;
	int wordcount;
};

struct pairstruct{
	char* pairname;
	char* paira;
	char* pairb; 
	int paircount;
	float corpuspercentage;
};

//MAIN PROGRAM
//
//

int main(int argc, char** argv) {
	
	//VARIABLE FARM
	//
	//
	
	int count = 0;
	int wordnumber = 0;
	int comparecount = 0;
	char* tempword = malloc(180 * sizeof(char));
	char* temppair1 = malloc(360 * sizeof(char));
	char* temppair2 = malloc(180 * sizeof(char));
	char* tempcon = malloc(360 * sizeof(char));
	char* pair_to_break = malloc(360 * sizeof(char));
	char* broken_paira; 
	char* broken_pairb;
	float tempnumerator = 0.0;
	float tempdenominator = 0.0;
	int uniquewords = 0; 
	int totalpair = 0;
	
	//DETERMINE WORD COUNT
	//
	//
	
	//Open File in Question 
	
	FILE* test = fopen(argv[1], "r");
	
	//Check to see if argv[1] exists, abort out if no file is found 
	
	if (test == NULL){
		printf("File couldn't be accessed or doesn't exist!\n");
		free(tempword);
		free(temppair1);
		free(temppair2);
		free(tempcon);
		return 0; 
	}
		
	//Use Word Counter function to pull number of words from file
	
	wordnumber = filewordcount(test);
	
	//Allocate space for a number of arrays of wordstruct equal to the number of words in the file, then initialize each individual struct array as NULL
	
	struct wordstruct** wordlist = malloc(wordnumber * sizeof(struct wordstruct*));
	while (count != wordnumber) {
		wordlist[count] = NULL;
		count += 1;
	}
	
	count = 0;
	
	//DETERMINE UNIQUE WORD TOTALS
	//
	//
	
	//Close file so program can reopen and reset fscanf
	
	fclose(test);
	//Read through the document again, pulling each word either into a new word struct, or upping the count of an existing word struct
	
	FILE* document = fopen(argv[1], "r");
		
	//Use For Loop to take scan in each word in the document FILE* 
	//Create temporary struct to house word information then run a while loop to compare that word against each struct currently in the system
	//For Assignment 6, I am making a change for the unique word total to ignore the last word, so I can use it as the unique first word pair total to build my corpus
	
	for (count = 0; count != (wordnumber - 1); count += 1) {
		
		//Read in word from document
		fscanf(document, "%s", tempword);
		
		//Determine if word already exists in wordlist struct array. If it doesn't, replace NULL struct with new word, by setting up a temp struct and then replacing the NULL struct with it. 
		//If the word does already exist in array of structs, up the count for that word struct
		
		//make the while a function, maybe?
		//comparecount = 0;
		while (comparecount != (wordnumber - 1)) {
			if (wordlist[comparecount] == NULL) {
				struct wordstruct* tempstruct = malloc(sizeof(struct wordstruct));
				tempstruct->wordname = malloc(180 * sizeof(char));
				tempstruct->wordcount = 1;
				strcpy(tempstruct->wordname, tempword);
				wordlist[comparecount] = tempstruct;
				break;
			}
			else if (strcmp(tempword, wordlist[comparecount]->wordname) == 0) {
				wordlist[comparecount]->wordcount += 1;
				break;
			}
			comparecount += 1;
		}
		comparecount = 0;
	}
	
	count = 0;
	
	while (count != (wordnumber - 1)) {
		if (wordlist[count] != NULL) {
			uniquewords += 1;
		}
		count += 1;
	}
	
	count = 0;
	
	//DETERMINE UNIQUE PAIR TOTALS
	//
	//
	
	//Now to pretty much do it all again, but this time with word pairs. First, set up an array of pairstruct structs, equal to the number of words you have minus 1 (last word can't have a buddy)
	
	struct pairstruct** pairlist = malloc((wordnumber - 1) * sizeof(struct pairstruct*));
		while (count != (wordnumber - 1)) {
			pairlist[count] = NULL;
			count += 1;
		}
	
	count = 0;
	
	//Close File again, to re-open for another fscanf pass with word pairs
	
	fclose(document);
	
	document = fopen(argv[1], "r");
	
	//Fscanf Round 2. This time for every word in the text file read in the current word and the next word, assigning both to a temp char*, and concatenating them together
	//Then setting that concatenated string into a struct if that concat string doesn't exist in a struct, or if it does upping that struct's count by 1
	
	//Pre Set the first pair, so we don't have the for loop skipping over pairs by jumping twice each fscanf
	
	fscanf(document, "%s %s", temppair1, temppair2);
	
	//Run the For Loop, first concatenating the current 2 temppairs
	
	//Creating strtok delimiter, space, so I can break up the pairstruct->pairname I stupidly decided to concatenate together
	
	for (count = 0; count != (wordnumber - 1); count += 1) {
	
		//Put a Space between the words, set variable tempcon to the concatenation of temppair1 and temppair2
		strcat(temppair1, " ");
		strcpy(tempcon, strcat(temppair1, temppair2));
		
		//Determine if word pair already exists in pairlist struct array. If it doesn't, replace NULL struct with new word, by setting up a temp struct and then replacing the NULL struct with it. 
		//If the word does already exist in array of structs, up the count for that word struct
		//Basically rehashing the wordstruct code above, just changing variables to work with the pairstruct
		//For Assignment 6 I've also had to set up two new members for each pairstruct, that contain the first and second word in the pair
			//Created these two new members using strtok, by first creating a copy of the concatenated word pair, breaking that pair by a space delimiter into new separate char* arrays
			//Then copying those two new char* arrays into member paira and member pairb of the tempstruct before copying it over to the pairstruct itself
		
		while (comparecount != (wordnumber - 1)) {
			if (pairlist[comparecount] == NULL) {
				struct pairstruct* tempstruct = malloc(sizeof(struct pairstruct));
				tempstruct->pairname = malloc(360 * sizeof(char));
				tempstruct->paira = malloc(180 * sizeof(char));
				tempstruct->pairb = malloc(180 * sizeof(char));
				tempstruct->paircount = 1;
				tempstruct->corpuspercentage = 0.0;
				strcpy(tempstruct->pairname, tempcon);
				strcpy(pair_to_break, tempcon);
				broken_paira = strtok(pair_to_break, " ");
				broken_pairb = strtok(NULL, " ");
				strcpy(tempstruct->paira, broken_paira);
				strcpy(tempstruct->pairb, broken_pairb);
				pairlist[comparecount] = tempstruct;
				break;
			}
			else if (strcmp(tempcon, pairlist[comparecount]->pairname) == 0) {
				pairlist[comparecount]->paircount += 1;
				break;
			}
			comparecount += 1;
		}
		comparecount = 0;
		
		//Set up temppair1 to be temppair2, and fscanf to make temppair2 the next word in the document
		strcpy(temppair1, temppair2);
		fscanf(document, "%s", temppair2);
	}
		
	//BUILD THE CORPUS
	//
	//
	
	count = 0;
	comparecount = 0;
	
	while (count != (wordnumber - 1)) {
		if (NULL != pairlist[count]) {
			for (comparecount = 0; comparecount != uniquewords; comparecount += 1) {
				if (NULL != wordlist[comparecount]) {
					if (strcmp(wordlist[comparecount]->wordname, pairlist[count]->paira) == 0) {
						tempnumerator = pairlist[count]->paircount;
						tempdenominator = wordlist[comparecount]->wordcount;
						pairlist[count]->corpuspercentage = (tempnumerator / tempdenominator);
						break;
					}
				}
			}
		}
		count += 1;
	} 
	
	count = 0;
	
	while (count != (wordnumber - 1)) {
		if (pairlist[count] != NULL) {
			printf("Pair Name: %s\n", pairlist[count]->pairname);
			printf("Corpus Percentage: %lf\n", pairlist[count]->corpuspercentage);
			printf("\n");
		}
		count += 1;
	}
	
	//RANDOM STATE GENERATOR
	//
	//
	
	count = 0;
	
	while (count != (wordnumber - 1)) {
		if (pairlist[count] != NULL) {
			totalpair += 1;
		}
		
		count += 1;
	}
	
	//Close document again, re-open it so we can pull the first word and use that as our State Machine's initial state
	
	fclose(document);
	
	document = fopen(argv[1], "r");
	
	char* starting_word = malloc(180 * sizeof(char));
	int seed = time(NULL);
	srand(seed);
	int random_number = rand() % 100;
	float random_float = (random_number * 0.01);
	int finish_loop = 0;
	float corpus_comparison = 0.0;
	float corpus_addition = 0.0;
	
	fscanf(document, "%s", starting_word);
	
	//Set up a few nested loops that, based on the initial state word, will print off the next word based on the weighted probabilities from the corpus 
	
	count = 0;
	comparecount = 0;
	
	while (comparecount != 1000) {
		printf("%s ", starting_word);
		count = 0;
		finish_loop = 0;
		//First check to make sure that the word in question has a pair it can run off from
		//If it finds a pair that starts the same as starting word it sets the finish_loop variable to 1, which will be used later to continue the for loop
		while (count != (totalpair)) {
			if (strcmp(starting_word, pairlist[count]->paira) == 0) {
				finish_loop = 1;
				break;
			}
			count += 1;	
		}
		//Terminating First Loop if current word state has no word pairs to run with
		if (finish_loop == 0) {
			break;
		}
		//Setting up the random number that will be used to generate the next state word
		count = 0;
		random_number = rand() % 100;
		random_float = (random_number * 0.01);
		corpus_comparison = 0.0;
		//Now the actual business. Run a while loop that looks through the list of pairs, trying to find a pair that starts with the same word as the starting_word
		//Once found, it will turn the starting_word into the second part of the pair if the random float we've created is less than it's corpus percentage
		//If the corpus percentage is smaller, then we add it's corpus percentage onto 
		while (count != totalpair) {
			if (strcmp(starting_word, pairlist[count]->paira) == 0) {
				corpus_comparison += pairlist[count]->corpuspercentage;
				if (random_float <= corpus_comparison) {
					strcpy(starting_word, pairlist[count]->pairb);
					break;
				}
			}
			count += 1;
		}
		comparecount += 1;
	}
	
	printf(" \n");
	
	//CLEAN UP TIME
	//
	//
	
	//Close up Files, and Free all unfreed memory
	
	fclose(document);
	
	count = 0;
	
	free(tempword);
	free(temppair1);
	free(temppair2);
	free(tempcon);
	free(pair_to_break);
	free(starting_word);
	
	while (count != (wordnumber - 1)) {
		if (wordlist[count] != NULL) {
			free(wordlist[count]->wordname);
			free(wordlist[count]);
		}
		count += 1;
	}
	
	count = 0;
	
	while (count != (wordnumber - 1)) {
		if (pairlist[count] != NULL) {
			free(pairlist[count]->pairname);
			free(pairlist[count]->paira);
			free(pairlist[count]->pairb);
			free(pairlist[count]);
		}
		count += 1;
	}
	
	
	free(wordlist);
	free(pairlist);
	
	return 0;
}
