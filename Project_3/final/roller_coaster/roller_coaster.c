#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define N 10

int wait_pas = 0;

pthread_mutex_t mtx_pass;    //gia th xrhsh tou cond_pass
pthread_mutex_t mtx_main;    //gia th xrhsh tou cond_main
pthread_mutex_t mtx_roll;    //gia th xrhsh tou cond_roll

pthread_cond_t cond_pass;    //h oura anamonhs twn passengers
pthread_cond_t cond_main;    //h oura anamonhs ths main
pthread_cond_t cond_roll;    //h oura anamonhs tou roller_coaster


void *passenger();
void *roller_coaster();

int main(int argc, char *argv[]) {
	int check, k, i;
	int number;
	char command[10];      //gia tis edoles tou xrhsth
	pthread_t p_rc, *p_pass;
	
	k = 0;
	
	/*arxikopoihsh twn mutexes kai twn conditions*/
	check = pthread_mutex_init(&mtx_pass,NULL);
	check = pthread_mutex_init(&mtx_main,NULL);
	check = pthread_mutex_init(&mtx_roll,NULL);
	
	check = pthread_cond_init(&cond_pass, NULL);
	check = pthread_cond_init(&cond_main, NULL);
	check = pthread_cond_init(&cond_roll, NULL);
	
	/*dhmiourgia roller_coaster*/
	check = pthread_create(&p_rc, NULL, &roller_coaster, NULL);
	if(check!=0) {
		printf("Problem to create roller_coaster thread\n");
		
		return(7);
    }
    
    /*epanalhyh gia na dwsei o xrhsths oses fores thelei passengers*/
    while(1) {
		//zhtame dedomena apo xrhsth
		printf("For new passengers press 'new' and the number of passengers\n");
		printf("Else for exit press 'exit'\n");
		printf("wait_pas: %d\n",wait_pas);
		scanf(" %9s",command);
		
		if(strcmp(command,"new")==0){
			scanf("%d",&number);
			wait_pas = wait_pas + number;    //sunolikos arithmos twn epivatwn meta thn eisagwgh twn kainouriwn 
			
			/*desmeush epivatwn*/
			p_pass = (pthread_t *)malloc(sizeof(pthread_t)*number);
			if(p_pass==NULL){                                        
				printf("Problem with memory allocation\n");

				return(2);
			}
			/*Dhmiourgia toswn thread osoi einai k oi epivates*/
			for(i=0;i<number;i++){
				
				/*blockarei gia na exei kathe thread/epivaths diaforetiko k*/
				check = pthread_mutex_lock(&mtx_pass);
				k++; //o arithmos tou kathe thread/epivath
				
				check = pthread_create(&p_pass[i], NULL, &passenger, &k);
				if(check!=0) {
					printf("Problem to create %d thread\n", i);
					free(p_pass);
					
					return(7);
				}
			}
			free(p_pass);
		}
		else if(strcmp(command,"exit")==0){
			printf("Roller coaster has closed for today!\n"); 
			break;
		}
		else{ 
			printf("Try again!\n");
			continue;
		}
		printf("wait_pas: %d\n",wait_pas);
		/*dinoume shma sto trenaki na ksekinhsei*/
		pthread_mutex_lock(&mtx_roll);
		pthread_cond_signal(&cond_roll);
		pthread_mutex_unlock(&mtx_roll);
		
		/*perimenei h main mexri na teleiwsoun th volta tous oi epivates pou exei dwsei o xrhsths*/
		pthread_mutex_lock(&mtx_main);
		pthread_cond_wait(&cond_main, &mtx_main);
		pthread_mutex_unlock(&mtx_main);
	}
    
    return (0);
}

void *roller_coaster(){
	int i;
	int count;
	
	count = 0;
	
	/*mia epanalhyh gia kathe volta mexri na teleiwsoun oi epivates*/
	while(1) {
		/*perimenei na parei tous epibates apo th main*/
		pthread_mutex_lock(&mtx_roll);
		pthread_cond_wait(&cond_roll, &mtx_roll);
		pthread_mutex_unlock(&mtx_roll);
		
		/*eisagwgh epivatwn sto roller_coaster mono an sublhrwnoun N atoma sthn omada*/
		while(wait_pas>=N){
			printf("The roller coaster is ready for new ride!\n");
			/*dinoume shma sth N omada epivatwn gia na bei sto trenaki*/
			for(i=0; i<N; i++) {
				count++;
				pthread_cond_signal(&cond_pass);
			}
			/*kratame posoi epivates den anevhkan sthn koursa tou roller_coaster*/
			wait_pas = wait_pas - N;
			sleep(1);
			printf("The roller coaster begin!\n");
			sleep(2);
			printf("Passengers abord the roller_coaster!\n");
			sleep(1);
		}
		/*eidopoioume th main oti teleiwsan oi epivates pou boroun na anevoun se mia koursa*/
		pthread_cond_signal(&cond_main);
	}
	
	return (NULL);
}


void *passenger(void *num_p){
	int k;

	/*************krisimos kwdikas**********/
	k = *(int *)(num_p);
	/*bainoun sthn oura oi epivates mexri na tous steilei shma to trenaki*/
	pthread_cond_wait(&cond_pass, &mtx_pass);
	
	pthread_mutex_unlock(&mtx_pass);
	printf("i am passenger %d\n", k);
	
	return (NULL);
}