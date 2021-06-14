#include "lib.h"
#include "types.h"

//static int readcount;

void Philosopher(int id, sem_t* count, sem_t forks[5]){
	printf("Philosopher %d: think\n", id);
	sleep(128);
	printf("Philosopher %d: want to eat\n", id);
	sem_wait(count);
	sem_wait(&forks[id]);
	printf("Philosopher %d: pickup fork:%d \n", id, id);
	sem_wait(&forks[(id + 1) % 5]);
	printf("Philosopher %d: pickup fork:%d \n", id, (id + 1) % 5);
	printf("Philosopher %d: eat\n", id);
	sleep(128);
	printf("Philosopher %d: put down fork\n", id);
	sem_post(&forks[id]);
	sem_post(&forks[(id + 1) % 5]);	
	sem_post(count);

}

void producer(int id, sem_t* empty, sem_t* full ,sem_t* mutex){
	while(1){
        	sleep(128);
		printf("Producer %d: produce\n", id);
		sem_wait(empty);
		sem_wait(mutex);
		printf("Producer %d: put product into buffer \n", id);
		sleep(128);
		sem_post(mutex);
		sem_post(full);
	}
}
void consumer(sem_t* empty, sem_t* full ,sem_t* mutex){
	while(1){
		sleep(128);
		printf("Consumer : want to consume\n");
		sem_wait(full);
		sem_wait(mutex);
		printf("Consumer : consume\n");
		sleep(128);
		sem_post(mutex);
		sem_post(empty);
	}
}


/*
void reader(int id, sem_t* mutex, sem_t* writelock, int* readcount){
	sleep(128);
	sem_wait(mutex);
        printf(" readera:%d\n",*readcount);                              
                           
  	if(*readcount==1)                  
      		sem_wait(writelock);                                                                 
  	sem_post(mutex);
    	printf("Reader %d: read, total %d reader\n", id, *readcount);
	sleep(128);
  	sem_wait(mutex);
	printf(" readerb:%d\n",*readcount);      
  	(*readcount)--;
  	if(readcount==0)
      		sem_post(writelock);
   	sem_post(mutex);
}
void writer(int id, sem_t* writelock){
	sleep(128);
  	sem_wait(writelock);
	printf("Writer %d: write\n", id);
  	sleep(128);
   	sem_post(writelock);
}
*/
int uEntry(void) {
	// For lab4.1
	// Test 'scanf' 
	int dec = 0;
	int hex = 0;
	char str[6];
	char cha = 0;
	int ret = 0;
	while(1){
		printf("Input:\" Test %%c Test %%6s %%d %%x\"\n");
		ret = scanf(" Test %c Test %6s %d %x", &cha, str, &dec, &hex);
		printf("Ret: %d; %c, %s, %d, %x.\n", ret, cha, str, dec, hex);
		if (ret == 4)
			break;
	}
	// For lab4.2
	// Test 'Semaphore'
	int i = 4;

	sem_t sem;
	printf("Father Process: Semaphore Initializing.\n");
	ret = sem_init(&sem, 2);
	if (ret == -1) {
		printf("Father Process: Semaphore Initializing Failed.\n");
		exit();
	}

	ret = fork();
	if (ret == 0) {
		while( i != 0) {
			i --;
			printf("Child Process: Semaphore Waiting.\n");
			sem_wait(&sem);
			printf("Child Process: In Critical Area.\n");
		}
		printf("Child Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		exit();
	}
	else if (ret != -1) {
		while( i != 0) {
			i --;
			printf("Father Process: Sleeping.\n");
			sleep(128);
			printf("Father Process: Semaphore Posting.\n");
			sem_post(&sem);
		}
		printf("Father Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		exit();
	}
	// For lab4.3
	// TODO: You need to design and test the philosopher problem.
	// Note that you can create your own functions.
	// Requirements are demonstrated in the guide.
	sem_t count;
	int ret = sem_init(&count, 4);
	if (ret == -1) {
		printf("Father Process: Semaphore count Initializing Failed.\n");
		exit();
	}
        sem_t forks[5];
	for(int i=0;i<5;i++){
		ret = sem_init(&(forks[i]), 1);
		if (ret == -1) {
		printf("Father Process: Semaphore forks Initializing Failed.\n");
		exit();
		}		
	}
	int i=0;
	for(;i<5;i++){
		ret = fork();
		if (ret == 0) {
			Philosopher(i, &count, forks);
			break;
	   }
	}
	if(i==5){
		while(1);
		printf("Father Process: Semaphore Destroying.\n");
		sem_destroy(&count);
		for(int i=0;i<5;i++){
			sem_destroy(&forks[i]);
		}
		exit();
        }
        else  exit();
	
        
        //for lab4.4		
	sem_t empty;      
	sem_t full;   
	sem_t mutex=1;
	int ret = sem_init(&empty, 2);
	if (ret == -1) {
		printf("Father Process: Semaphore count Initializing Failed.\n");
		exit();
	}
	ret = sem_init(&full, 0);
	if (ret == -1) {
		printf("Father Process: Semaphore count Initializing Failed.\n");
		exit();
	}
	ret = sem_init(&mutex, 1);
	if (ret == -1) {
		printf("Father Process: Semaphore count Initializing Failed.\n");
		exit();
	}
	int i=0;
	for(;i<5;i++){
		ret = fork();
		if (ret == 0) {
			if(i<4){
				producer(i+1, &empty, &full, &mutex);
			}
			else{
				 consumer(&empty, &full, &mutex);
			}
			break;
	   	}
	}
	if(i==5){
		while(1);
		printf("Father Process: Semaphore Destroying.\n");
		sem_destroy(&empty);
		sem_destroy(&full);
		sem_destroy(&mutex);
		exit();
        }
        else  exit();
        


	/*
	sem_t writelock, mutex;
        sem_t readcount;
	readcount=0;
	int ret = sem_init(&writelock, 1);
	if (ret == -1) {
		printf("Father Process: Semaphore count Initializing Failed.\n");
		exit();
	}
	ret = sem_init(&mutex, 1);
	if (ret == -1) {
		printf("Father Process: Semaphore count Initializing Failed.\n");
		exit();
	}
        int i=0;
	for(;i<6;i++){
		ret = fork();
		if (ret == 0) {
			if(i<3){
				reader(i+1, &mutex, &writelock, &readcount);
			}
			else{
				 writer(i-2, &writelock);
			}
			break;
	   	}
	}
	if(i==6){
		while(1);
		printf("Father Process: Semaphore Destroying.\n");
		sem_destroy(&writelock);
		sem_destroy(&mutex);
		exit();
        }
        else  exit();
	*/
	return 0;
}
