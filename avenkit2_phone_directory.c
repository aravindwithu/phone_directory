#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/file.h>

//Declaration of struct array and charecter array size
#define arraySIZE 500
#define charSIZE 50

//Declaration of struct
struct Directory
{
    char firstName[charSIZE];
    char lastName[charSIZE];
    char phNum[charSIZE];
};

//Declaration of struct array called phDirectory
struct Directory phDirectory[arraySIZE];

/*
Insert function gets two arguments index and data, index to identify the position or index of the array where the 
new contact tob inserted. data is the combination of first name, last name, phone number or each line from the import file to insert the contact,
to respective index in phone directory. 
*/
void Insert(int index, char data[]){
	char *str = "\0"; 
	str = strtok(data, " ");
	int cnt = 0;
	while (str != NULL)
    {
        if(cnt == 0){ 
        	strcpy(phDirectory[index].firstName, str);
    	}
 
        else if(cnt == 1){
        	strcpy(phDirectory[index].lastName, str);
    	}

        else if(cnt == 2){
        	strcpy(phDirectory[index].phNum, str);
    	}

        str = strtok(NULL, " ");
        cnt++;
    }
    free(str);
}

/*
Display function will print the contact details for the given start index and end index. 
*/
void Display (int startIndex, int endIndex){
	int index = startIndex;
    while(index < endIndex)
    {
      printf("%s %s %s\n", phDirectory[index].firstName, phDirectory[index].lastName, phDirectory[index].phNum);
      index++;
    }
}

/*
Swap function to swap the contacts at given two indices. 
Swap is done using index of the phone directory array.
*/
void Swap(int index1, int index2){
    struct Directory temp;
    temp = phDirectory[index1];
    phDirectory[index1] = phDirectory[index2];
    phDirectory[index2] = temp;
}

char *Concat(int index, bool concatAll){
	char *str = (char *) malloc(sizeof(char) * (charSIZE * 3));
	strcpy(str, phDirectory[index].firstName); 
	strcat(str, phDirectory[index].lastName); 
	if(concatAll){
	strcat(str, phDirectory[index].phNum);}
	return str;
}

/*
Partition function, helper to quick sort 
where a partitioned portion is sorted with respective to pivot index and partition index. 
Uses Swap function to swap to contacts to arrange the contact 
and Concat helper fuction to concate the irst name, last name and phone number in the contact.
based on first name, last name and phone number ascending order
Here strcmp(String Compare) along with strcat() in Concat function is used to compare the contacts to determne the order.
*/
int Partition(int startIndex, int endIndex){
	int pivotIndex = endIndex;
	int partitionIndex = startIndex;
	int cmpVal = 0;
	int i = 0;
	for(i = startIndex; i < endIndex; i++){
		char *str1 = Concat(i, true);
		char *str2 = Concat(pivotIndex, true);
		cmpVal = strcmp(str1, str2);
		free(str1); free(str2);
		if (cmpVal <= 0){
			 Swap(i, partitionIndex);
			 partitionIndex++;
		}
	}
	Swap(partitionIndex,endIndex);
	return partitionIndex;
}

/*
Quicksort function, randomized quick sort function implemented in recursive manner. 
Partition is the helper function called in recursive manner to sort the data.
*/
void Quicksort(int startIndex, int endIndex){
	if (startIndex <= endIndex){
		srand(time(NULL));
		int randIndex = startIndex + rand() % (endIndex-startIndex+1);
		Swap(randIndex, endIndex);
		int partitionIndex = Partition(startIndex, endIndex);
		Quicksort(partitionIndex+1, endIndex);
		Quicksort(startIndex, partitionIndex-1);
	}
}


/*
Binarysearch function, to identify and return the contact required to be searched. 
Binarysearch is done in recursive manner.
In case of very large end index the mid calculation can be done by offseting the endindex with start index, divide the same by 2 and then add by start index.
both simple average mid index calculation and above mentioned methode will yield same mid index.
*/
int Binarysearch(int startIndex, int endIndex, char findName[]){
	if(startIndex <= endIndex){
		int midIndex = floor(startIndex + (endIndex - startIndex)/2);
		char *str1 = Concat(midIndex, false);		
		int cmpVal = strcmp(findName, str1);
		free(str1);
		if (cmpVal == 0){
			return midIndex;
		}
		else if(cmpVal  < 0){
			return Binarysearch(startIndex, midIndex-1, findName);
		}
		else{
			return Binarysearch(midIndex+1, endIndex, findName);
		}
	}
	return -1;
}

 /*LoadFile function retrives each line of contact from given file 
 and insert the same in phone directory arrau using insert methode.
 */
int LoadFile(char fileName[]){
	int index = 0;
	FILE *file = fopen(fileName, "r");
	if(!file){
		fprintf(stderr, "Can't onpen the file %s", fileName);
	}
	char buf[1000] = "\0";
    
	while(fgets(buf, 1000, file)){
		if(buf[strlen(buf)-1] == '\n'){
		buf[strlen(buf)-1] = '\0';}
		Insert(index, buf);
		index++;
	}
	//printf("The file %s is succesfully imported.\n\n", fileName);
	fclose(file); 
	return index;
}

/*
Delete function, to delete the given contact. 
*/
void Delete(int index){
	//Swap(index,endIndex);
	strcpy(phDirectory[index].lastName, "\0");
	strcpy(phDirectory[index].firstName, "\0"); 
	strcpy(phDirectory[index].phNum, "\0");
	//*phDirectory[index] = NULL;
}

int main(int argc, char *argv[])
{	
	char fileName[charSIZE]  = "\0";
	if(argv[1] == NULL){
	printf("Please provide the input file name.\n");	
	scanf("%s", fileName);	
	}
	else{
		strcpy(fileName, argv[1]);
	}
    
    // To load the file using the given file name
	int index = 0; 
	index = LoadFile(fileName);
    
	printf("\n");
    // while loop to simulate different functions of the phone directory c programe,
	bool isLoop = true;
	while(isLoop){
		int choice = 0;
		int findIndex = -1;
		char data[charSIZE*3] = "\0";
		char firstName[charSIZE] = "\0"; 
	        char lastName[charSIZE] = "\0";
		char *str = "\0";
		printf("1) Sort_contact\n2) Search\n3) Delete\n4) Insert\n");
		printf("Enter the choice -> \n");
		
		scanf("%d", &choice);
		
		switch (choice){

			case 1: // This case will sort the contact using quick sort algorithm
				Quicksort(0, index-1);
				Display(0,index);
				printf("\n");
			break;

			/*This case will search the contact using binary search algorithm to retrieve information about given name.
			quick sort must be done before binary search is done.
			*/
			case 2:
				printf("Enter the contact name -> ");
				scanf("%s %s",firstName, lastName);
				strcpy(data, firstName); 
				strcat(data, lastName);
				strcat(data, ",");					

				Quicksort(0, index-1);
				findIndex = Binarysearch(0, index, data);

			    if (findIndex != -1){
			    	Display(findIndex, findIndex+1);
			    }
			    else{
			    	printf("Name not found.\n");
			    }
			    printf("\n");
			break;

			/*This case will identify the index of the given name using binary search 
			and then using the given index the delete operation is done.
			Once the identified index is deleted then a quick sort is performed to retain the sort order.
			will work for both <firstname lastname> or <firstname lastname phnumber> as input but search and delete the contact with <firstname+lastname>
			*/
			case 3: 
				memset(data, 0, charSIZE*3);
				printf("Enter the contact name -> ");

				fgets (data, (charSIZE*3), stdin);
                                scanf ("%[^\n]%*c", data);
                                str = strtok(data, " ");
                                int cnt = 0;
                                while (str != NULL)
                                 {
                                        if(cnt == 0){
						strcpy(data, str);	
                                         }

                                        else if(cnt == 1){
                                                 strcat(data, str);
                                         }

                                        str = strtok(NULL, " ");
                                        cnt++;
				 }
				free(str);
				if(data[strlen(data)-1] != ','){
				strcat(data,",");}
				Quicksort(0, index-1);
				findIndex = Binarysearch(0, index, data);

			    if (findIndex != -1){
			    	Swap(findIndex,  index-1);
				Delete(index-1);
			    	index--;
			    	Quicksort(0, index-1);
				Display(0,index);
				printf("\n");
			    }
			    else{
			    	printf("Name not found.\n");
			    }
			    printf("\n");
			break;

			/*
			This case will insert the provided contact data.
			*/
			case 4:
				printf("Enter the contact -> ");
				fgets (data, (charSIZE*3), stdin);
				scanf ("%[^\n]%*c", data);
				Insert(index, data);
				index++;
				Quicksort(0, index-1);
				Display(0,index);
				printf("\n");
			break;
			/*
			For any other input case the programe exits.
			*/
			default:
				isLoop = false;
			break;
		}
	}
	/* 
	Removes memorey occupied PhDirectory number array during programe exit.
	*/
	while(index<0){
		Delete(index-1);
		index--;
	}
	return 0;
}


