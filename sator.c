/*
# (c)2005 Giacomo Miceli <jamez@jamez.it>

S A T O R
A r e p O
T e n e T
O p e r A
R O T A S

This is a dictionary based word square generator inspired on the Sator Square. 
The word squares produced by this program do not respect the 4-way palindrome 
of the famous Sator Arepo square, but they are two dimensional. In other words 
you can read the square from top-to-bottom and from left to right.

Make sure to specify a good dictionary in the config section or simply use the one provided. 

Description of the algorithm:

-take a word, count its chars, create the output matrix
-scan the dictionary creating a tree structure that only considers words of that 
 length and places them on the right branch according to the letter.
-while there are still letters in the input word
	.take the current letter index
	.while there are still words beginning with that word
		~try to insert that word in the matrix
		~call the function recursively with the next letter index

*/

#include <stdio.h>
#include <ctype.h>

/****Config section****/
#define ALPHABET_SIZE 26
#define MAX_WORD_SIZE 30
char dictionary[] = "dctionary.txt";
/**End config section**/

struct tree_node{
    char c;
    struct tree_node *next_letter[ALPHABET_SIZE];
};

typedef struct tree_node *tree;

struct list_node{
char word[MAX_WORD_SIZE];
struct list_node *next;
};
typedef struct list_node *list;

tree buildtree(tree, char *);

list getprelist(char *pre, int pos, tree built,int pos2, char *result, list good_words){

    //Create a list of words in the tree which begin with the pre-string
    int index, flag=0, i=0;;
    list link;
    if (pos < strlen(pre)){
    	index = pre[pos] - 97;
    	if (built->next_letter[index] != NULL){
    		good_words = getprelist(pre, ++pos, built->next_letter[index], pos2, result, good_words);
    		return good_words;
    		}
    	else
    		return NULL;
    	return good_words;
    	}
    else
    {
        while(i<ALPHABET_SIZE){
        	if (built->next_letter[i] != NULL){
        		flag = 1;
        		result[pos2] = built->next_letter[i]->c;
        		good_words = getprelist(pre, pos, built->next_letter[i], ++pos2, result, good_words);
          		result[--pos2] = ' ';
        		}
        	i++;
        	}
        list new_word = NULL;

        if (flag == 0){
        	new_word = malloc(sizeof(struct list_node));
        	int preapp = 0, postapp = 0;
        	while (pre[preapp]!='\0'){
        		new_word->word[preapp] = pre[preapp];
        		preapp++;
        		}
        	while (result[postapp]!='\0'){
        		new_word->word[preapp] = result[postapp];
        		postapp++;
        		preapp++;
        		}
        	new_word->word[preapp]='\0';
        	new_word->next=good_words;
        	return new_word;

        	}
        else
        	return good_words;
        }
}


main(){

    int index=0, input_length, scanning = 0, ii;
    char *input, word[MAX_WORD_SIZE];
    tree temp, built;
    list good_words;
    built = malloc(sizeof(struct tree_node));
    built->c = '$';
    good_words = malloc(sizeof(struct list_node));
    for (ii = 0; ii < ALPHABET_SIZE; ii++)
    	built->next_letter[ii] = NULL;
    //take a word
    printf("type a word\n");
    input = (char *)malloc(MAX_WORD_SIZE);
    scanf ("%s", input);
    input_length=strlen(input);

    //open the dictionary
    FILE *fp;
    fp = fopen(dictionary, "r");
    char c;
    while ((c = getc(fp)) != EOF){
    	if ((c != '/') && (c != '\n'))
    		{
    		if (isalnum(c)){
    			word[index] = tolower(c);
    			index++;
    			}
    		}
    	else{
    		if (strlen(word) == input_length){
    			built = buildtree(built, &word);
    			scanning++;
    			}


    		for (index = 0; index < MAX_WORD_SIZE; index++)
    			word[index] = '\0';
    		index = 0;
    		while (c != '\n')
    			c = getc(fp);
    		}
    	}

    good_words = malloc(sizeof(struct list_node));
    int position=0;
    char prel[10];
    char *res = (char *)malloc(MAX_WORD_SIZE);
    char pre[10] = "c";

    if (good_words == NULL)
    	printf("NULL\n");
    else{
        printf("%s\n", good_words->word);
        while (good_words->next != NULL){
            printf("%s\n", good_words->word);
            good_words = good_words->next;
        }
    }

    int x = 0, y = 0;
    char matrix[input_length][input_length];
    while (x < input_length){
        matrix[0][x] = input[x];
        x++;
    }

    matrix[0][x] = '\0';
    x = 0;
    get_matrix(built, input_length, 1, matrix);

}

int get_matrix(tree built, int input_length ,int start, char matrix[input_length][input_length]){
/*
 * Builds the matrix
 *if start = strlen input print this matrix
 *create pre-string using start-string
 *create the list of every word that begins with pre-string
 *if our list is null returns 0
 *while our list isn't over
	 *insert current word in position and
	 *call again the function with the updated matrix and start++
	 *(if get_matrix(...)==0) return 0
 *return
 */

    int x=0, y=0, index=0;
    if (start == input_length){
        //If we reached the last letter to be processed, print the matrix
        while (x < input_length * input_length){
        	printf("%c", matrix[0][x]);
        	x++;
        	if (x % input_length == 0)
        		printf ("\n");
        }
        printf ("\n\n");
        return 1;
        }

    else{
        //else create the matrix
        list good_words;
        good_words = malloc(sizeof(struct list_node));
        int position = 0;
        char *res = (char *)malloc(MAX_WORD_SIZE);
        char pre[MAX_WORD_SIZE];

        int indwhile = 0;
        //build the pre-string
        while (indwhile <= start){
        	pre[indwhile] = matrix[indwhile][start];
        	indwhile++;
        	}
        pre[start] = '\0';
        index++;
        good_words = getprelist(pre, position, built, position, res, NULL);
        if (good_words==NULL)
        	return 0;
        else{
            start++;
            char newmatrix[input_length][input_length];
            memcpy(newmatrix, matrix, sizeof(char) * input_length * input_length);
            strcpy(newmatrix[start-1], good_words->word);
            get_matrix(built, input_length, start, newmatrix);
            while (good_words->next != NULL){
            	memcpy(newmatrix, matrix, sizeof(char) * input_length * input_length);
            	strcpy(newmatrix[start-1], good_words->word);
            	get_matrix(built, input_length, start, newmatrix);
            	good_words = good_words->next;
            	}
            }
        }
    }

tree buildtree (tree built, char *word){
    //Inserts in the tree a given word. In this tree, every word is represented by a different path from root to leaf.
    if (strlen(word) == 0)
    	return built;
    int c, i=1, ii;
    char trim[MAX_WORD_SIZE];
    c = word[0]-97;

    if (built->next_letter[c] == NULL){
    	built->next_letter[c] = malloc(sizeof(struct tree_node));
    	for (ii = 0; ii < ALPHABET_SIZE; ii++)
    		built->next_letter[c]->next_letter[ii] = NULL;
    	built->next_letter[c]->c = word[0];
    }

    while (word[i] != '\0'){
    	trim[i-1] = word[i];
    	i++;
    	}
    trim[i-1]='\0';

    built->next_letter[c]=buildtree(built->next_letter[c],trim);
    return built;
    }


int visittree(tree built){
    //just 4 debug> visits and prints the content of the tree
    int i = 0, flag = 0;
    printf ("%c\n", built->c);
    while(i < ALPHABET_SIZE){
    	if (built->next_letter[i] != NULL){
    		visittree(built->next_letter[i]);
    		flag = 1;
    		}
    	i++;
    	}
    if(flag == 0) printf ("///\n");
        return;
    }
