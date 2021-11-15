#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>


#define OFFSET 100
#define ROWS 100
#define COLS 100
#define ROOT 10



void ctrlC(int);
char* read_filename();
void fill_array(char*, int[ROWS][COLS]);
void waitt(unsigned int);
void print_array(int[ROWS][COLS]);
void* play(void*);


int arg1;
int arg2;
int world[ROWS][COLS];//the current array
int copy[ROWS][COLS];//the next generation array
pthread_t threads[ROWS];//the threads


void ctrlC(int signo){
  if (signo == SIGINT) 
   	exit(0);
}

char* read_filename(){
	char* filename = malloc(1);
	printf("Enter the name of the test file:\n");
	int c;
    int i = 0;
    /* Read characters until found an EOF or newline character. */
    while((c = getchar()) != '\n' && c != EOF){
	    filename[i++] = c;
	    filename = realloc(filename, i+1); // Add space for another character to be read.
    }
    filename[i] = '\0';  // Null terminate the string
	return filename;
}

void fill_array(char* filename, int array[ROWS][COLS]){
	FILE* file = fopen(filename, "r");
	if(file==NULL){
		return;
	}
	int i, j;
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			if(!feof(file)){
				fscanf(file, "%d", &array[i][j]);
			}
		}
	}
	fclose(file);
}


void print_array(int array[ROWS][COLS]){

	/*print the new one*/
	int i, j;
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			if(array[i][j]==0)
				printf(" ");
			else
				printf("x");
			printf(" ");	
		}
		printf("\n");
	}

	puts("\033[H\033[J");

}

int check_neighbors(int i, int j){
	int counter = 0;

	//up&left neighbor
	if(i-1>=0 && j-1>=0){
		if(world[i-1][j-1]==1){
			counter++;
			
		}
	}
	//up neighbor
	if(i-1>=0){
		if(world[i-1][j]==1){
			counter++;
		}
	}
	//up&right neighbor
	if(i-1>=0 && j+1>=0){
		if(world[i-1][j+1]==1)
		{
			counter++;
		}
	}
	//left neighbor
	if(j-1>=0){
		if(world[i][j-1]==1){
			counter++;
		}
	}
	//right neighbor
	if(j+1>=0){
		if(world[i][j+1]==1){
			counter++;
		}
	}
	//bottom&left neighbor
	if(i+1>=0 && j-1>=0){
		if(world[i+1][j-1]==1){
			counter++;
		}
	}
	//bottom neighbor
	if(i+1>=0){
		if(world[i+1][j]==1){
			counter++;
		}
	}
	//bottom&right neighbor
	if(i+1>=0 && j+1>=0){
		if(world[i+1][j+1]==1){
			counter++;
		}
	}
	return counter;
}

/*the function each thread will run*/
void* play(void* _){

	//sem_wait(&sem);
	int k, l;
	
	int neighbors=0;
	for(l=arg1; l<arg1+10; l++){
		for(k=arg2; k<arg2+10; k++){
			neighbors = 0;
			neighbors = check_neighbors(l,k);
			
			//printf("l:%d k:%d neighbors:%d\n", l, k, neighbors);
			if(world[l][k]==1){
				if(neighbors<2 || neighbors>3){
					copy[l][k] = 0;
				}
				if(neighbors==2 || neighbors==3){
					copy[l][k] = 1;
				}
			}
			else{
				if(neighbors==3){
					copy[l][k] = 1;
				}
			}
		}
	}


	
//	sem_post(&sem);
	pthread_exit(0);
}



int main(){	
	signal(SIGINT, ctrlC);
	

	char* filename = read_filename();
	fill_array(filename, world);
	/*copy array*/
	int i, j; 
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			copy[i][j] = world[i][j];
		}
	}
	while(1){

		//sem_init(&sem, 0, 1);
		int k=0;
		print_array(world);
		
		for(i=0; i<ROWS; i+=10){
			for(j=0; j<COLS; j+=10){
			
				arg1 = i;
				arg2 = j;
				pthread_create(&threads[k], NULL, play, NULL);
				pthread_join(threads[k], NULL);
				k++;
				
			}
		}

		for(i=0; i<ROWS; i++){
			for(j=0; j<COLS; j++){
				world[i][j] = copy[i][j];
			}
		}

		//sem_destroy(&sem);

	}

	

	return 0;

}