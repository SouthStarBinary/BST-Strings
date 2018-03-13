/*
	Jean Paul Lobon
	
        Program description: program takes data as strings, inputs them into a BST and then prints them out inorder
        Option -c makes the strings inputted case sensitive
        Option -o: followed by the name of a file will output the inorder stings into that file. If this option is not selected 
        then the results will be printed to the screen.

        **I certify that this program is entirely my own work and none of it
          is the work of any other person
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

  int debug = 0;
/*structure called TREENODE represents every node in the tree. Key holds the a string that was inputted. The left and right child pointers point to that 
 * nodes left and right children. Int count is for keeping track of multiple duplicates inputted.
 *
 */
typedef struct TREENODE { 
       char * key;
       struct TREENODE *leftChild;
       struct TREENODE *rightChild;
       int count;
}TREENODE;

/*
 *Some headers for functions 
 */

TREENODE* getStdin (char * input, TREENODE* node);

int compareTo(char * str1, char * str2);


TREENODE* treeInsert (char input[100], TREENODE* node);

TREENODE* newNODE (char ar[]);


/*treeInsert recursive function transverses the tree downward to insert last inputted string. Starts from the root. 
 *This tree inserts strings insensitively.
 */

TREENODE* treeInsert (char input[100], TREENODE* node) {
       
       if (node == NULL) {            
             return newNODE(input);
       }
            
       if (compareToSensitive(input, node->key) < 0) {
             node->leftChild = treeInsert(input, node->leftChild);
       }
       else if (compareToSensitive(input, node->key) > 0) {
             node->rightChild = treeInsert(input, node->rightChild);
       }
       else {
             node->count++;
       } 
       return node;

}
/*Some function as above however this insert treats strings case sensitively
 *
 */
TREENODE* treeInsertInsensitive (char input[100], TREENODE* node) { 

      if (node == NULL) { 
          return newNODE(input);
      }
      
      if (compareTo(input, node->key) < 0) { 
          node->leftChild = treeInsertInsensitive(input, node->leftChild);
      } 
      else if (compareTo(input, node->key) > 0) { 
          node->rightChild = treeInsertInsensitive(input, node->rightChild);
     }
      else { 
            node->count++;
      }
      return node;
}

/*Function that creates a new node. This is called from both treeInsert functions base cases.
 */

TREENODE* newNODE (char ar[]) { 
             TREENODE *new = (TREENODE *) malloc(sizeof(TREENODE));
             new->key = (char *) malloc(strlen(ar)*sizeof(char) );
//             new->key = ar;             
             strcpy(new->key,ar);
             new->leftChild = NULL;
             new->rightChild = NULL;
             new->count = 0;
             return new;
}

/*Prints the BST inorder to the screen
 */
void printTree (TREENODE *node) {
      //printf("Before the if\n");
       if (node != NULL) { 
             //printf("In the if");
             printTree(node->leftChild);
             printf("%s \n",node->key);
             while (node->count > 0) {
                  node->count--;
                  printf("%s\n",node->key);
             }
             printTree(node->rightChild);
       }
}

/*Puts the result of the tree, inorder, into a selected file.
 */

void printTreeToFile (TREENODE *node, FILE *outputFile) {

       if (node != NULL) { 
            printTreeToFile(node->leftChild,outputFile);
            fputs(node->key,outputFile);
            //fputs("\n",outputFile);
            printTreeToFile(node->rightChild,outputFile);
       }
}

/*Performs a postorder transversal of the tree and frees a nodes malloc'd string and then the node itself
 */

void freeTree (TREENODE *node) { 
       if (node != NULL) { 
           freeTree(node->leftChild);
           freeTree(node->rightChild);
           free(node->key);
           free(node);
       }
}

/*Compares two strings (new string inserted and a node's string) case sensitive.
 */

int compareToSensitive (char * str1, char * str2) {
       
        int i = 0;
        int result = 0;
        while(str1[i] - str2[i] == 0 && (strlen(str1) > i || strlen(str2) > i)) {
              i++;

        }

	if (str1[i] - str2[i] > 0) { 

              result = str1[i] - str2[i];     
        }
        else if (str1[i] - str2[i] < 0) { 

             result = str1[i] - str2[i];
        }
        else { 
             result = str1[i] - str2[i];
        }
        return result;
}

/*Same function as above however is case insensitive.
 */

int compareTo (char * str1, char * str2) {
         
        int i;
        char copy1[100];
        char copy2[100];
        strcpy(copy1,str1);
        for (i = 0; copy1[i]; i++) { 
            copy1[i] = tolower(copy1[i]);
        }
        strcpy(copy2,str2);
        for (i = 0; copy2[i]; i++) { 
            copy2[i] = tolower(copy2[i]);
        }
        //printf("%s and %s\n",copy1,copy2);
        
        return compareToSensitive(copy1,copy2);
}



int main(int argc, char **argv)
{
        extern char *optarg;
        extern int optind;
        int c, err = 0; 
        int cflag=0, oflag=0;//flags which options are being used
        FILE* inputFile = NULL;
        FILE* outputFile = NULL;
        char input[100];
        char * inputFileName;
        char * outputFileName = "";
	static char usage[] = "usage: %s [-c] [-o output_file_name] [input_file_name]\n";//command usage

        /* 
          While loop that gets each options if it is used then assings it to
          the appropriate value.
       */
	while ((c = getopt(argc, argv, "co:")) != -1)
		switch (c) {
	         	case 'c':
               	               cflag = 1;
                               break;
                
                        case 'o':
                              oflag = 1;
                              outputFileName = optarg;
                              break;

	                case '?':
                             err = 1;
                             printf("Error: not an option. %s.\n",usage);
                             exit(0);
			     break;
	        }
   


        if ((optind + 1) > argc) {
             printf("Enter text:\n\n");
        }
        else {
        while (optind <  argc) { 
               optind++;
        }
        if ((inputFile = fopen(argv[optind - 1],"r")) == NULL) {
                printf("ERROR: Input file not found\n");
                exit(101);
           }

        }
        
        
        
        /*The following are all the following options that could be selected from the command line
         *  
         * ALL inputFile != NULL STATEMENTS PRINT THE TREE OUT TO THE SCREEN
         *cflag and oflags represent if the options -c or -o were selected 
         */
        
        if (inputFile != NULL && cflag == 0 && oflag == 0) { 
            TREENODE *root = NULL;
            char string[100];
            fgets(string, 100, inputFile);
            root = treeInsert(string,root);
            while (fgets(string, 100, inputFile) && string[0] != '\n') { 
                treeInsertInsensitive(string, root);
            }
            printTree(root);
            freeTree(root);
         }       
  
        if (inputFile != NULL && cflag == 1 && oflag == 0) { 
            TREENODE *root = NULL;
            char string[100];
            fgets(string, 100, inputFile);
            root = treeInsert(string,root);
            while (fgets(string, 100, inputFile) && string[0] != '\n') { 
                treeInsert(string, root);
            } 
            printTree(root);
            freeTree(root);
        }

        if (inputFile != NULL && cflag == 0 && oflag == 1) { 
            outputFile = fopen(outputFileName,"w");
            TREENODE *root = NULL;
            char string[100];
            fgets(string, 100, inputFile);
            root = treeInsert(string,root);
            while (fgets(string, 100, inputFile) && string[0] != '\n') { 
                treeInsertInsensitive(string,root);
            }
            printTreeToFile(root,outputFile);
            freeTree(root);
        }

        if (inputFile != NULL && cflag == 1 && oflag == 1) {
            outputFile =  fopen(outputFileName,"w");
            TREENODE *root = NULL;
            char string[100];
            fgets(string, 100, inputFile);
            root = treeInsert(string,root);
            while (fgets(string, 100, inputFile) && string[0] != '\n') { 
                treeInsert(string,root);
            }
            printTreeToFile(root,outputFile);
            freeTree(root);
        }
        
        /*ALL inputFile == NULL STATEMENTS OUTPUT THE TREE INTO A SELECTED FILE
         */      
        if (inputFile == NULL && cflag == 0 && oflag == 0) { 
            char string[100];
            TREENODE *root = NULL;
            fgets(string, 100, stdin);
            root = treeInsertInsensitive(string,root);
            do {
                fgets(string, 100, stdin); 
                treeInsertInsensitive(string,root);
            }while(string[0] != '\n');
            printTree(root);
            freeTree(root);
        }

        if (inputFile == NULL && cflag == 1 && oflag == 0) { 
            char string[100];
            TREENODE *root = NULL;
            fgets(string, 100, stdin);
            root = treeInsert(string,root);
            do { 
                fgets(string, 100, stdin);
                treeInsert(string,root);
            }while(string[0] != '\n');
            printTree(root);
            freeTree(root);
        }

        if (inputFile == NULL && cflag == 0 && oflag == 1) { 
            outputFile = fopen(outputFileName , "w");
            char string[100];
            TREENODE *root = NULL;
            fgets(string,100,stdin);
            root = treeInsertInsensitive(string,root);
            do { 
                fgets(string, 100, stdin);
                treeInsertInsensitive(string,root);
            }while (string[0] != '\n');
            printTreeToFile(root,outputFile);
            fclose(outputFile);
            //fclose(inputFile);
            freeTree(root);
            
        } 

        if (inputFile == NULL && cflag == 1 && oflag == 1) { 
            outputFile = fopen(outputFileName, "w");
            char string[100];
            TREENODE *root = NULL;
            fgets(string,100,stdin);
            root = treeInsert(string,root);
             do {
                fgets(string, 100, stdin);
                treeInsert(string,root);
            }while (string[0] != '\n');
            printTreeToFile(root,outputFile);
            fclose(outputFile);
            //fclose(inputFile);
            freeTree(root);
        }

	return(0); 
  }
