#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define N 10

int i_blue, i_red;
int un_count_b, un_count_r;
int count_b, count_r;
int check_blue,check_red;
pthread_mutex_t mtx;

void *red(void *num_r);
void *blue(void *num_b);

int main(int argc, char *argv[]){
	char c;
	int i;
	int check;
	int *array_r, *array_b;
	pthread_t *p_red, *p_blue;
	
	pthread_mutex_init(&mtx,NULL);
	
	synch(main);
	
	synch(red);
	
	synch(blue);
	
	while(1) {
		i_blue = 0;
		i_red = 0;
		count_b = 0;
		count_r = 0;
		un_count_b = 0;
		un_count_r = 0;
		check_blue = 0;
		check_red = 0;
		
		printf("Press 0 to insert a red car, 1 for blue car or 'q' to exit\n");
		do{
			c = getchar();
			if(c=='0'){
				//red
				i_red++;
			}
			else if(c=='1'){
				//blue
				i_blue++;
			}
			else if(c=='q') {
				return(0);
			}
		}while(c!='\n');
		
		if(i_red==0 && i_blue==0){
			return(0);
		}
		printf("Α new day dawned\n");
		
		p_red = (pthread_t*)malloc(sizeof(pthread_t)*i_red);
		if(p_red==NULL){
			printf("Problem with memory allocation\n");
			return(2);
		}
		p_blue = (pthread_t*)malloc(sizeof(pthread_t)*i_blue);
		if(p_blue==NULL){
			printf("Problem with memory allocation\n");
			return(2);
		}
		array_r = (int*)malloc(sizeof(int)*i_red);
		if(array_r==NULL){
			printf("Problem with memory allocation\n");
			return(2);
		}
		array_b = (int*)malloc(sizeof(int)*i_blue);
		if(array_b==NULL){
			printf("Problem with memory allocation\n");
			return(2);
		}
		
		printf("red %d, blue %d\n",i_red, i_blue);
		
		for(i=0; i<i_red; i++){
			array_r[i] = i + 1;
			check = pthread_create(&p_red[i], NULL, &red, &array_r[i]);
			if(check!=0) {
				printf("Problem to create %d thread\n", i);
				return(7);
			}
		}
		
		for(i=0; i<i_blue; i++){
			array_b[i] = i + 1;
			check = pthread_create(&p_blue[i], NULL, &blue, &array_b[i]);
			if(check!=0) {
				printf("Problem to create %d thread\n", i);
				return(7);
			}
		}
		synch_begin(main);
		
		synch_wait();
		
		synch_end(main);
		
		printf("End of the day\n");
	}
	
	return(0);
}

void *red(void *num_r){
	int number;
	int i;
	number = *(int *)(num_r);
	
	synch_begin(red);
	
	if(i_blue==0){
		check_red++;
		if(check_red == i_red) {
			notify();
		}
		if(i_red!=1){
			synch_wait();
		}
	}
	else{
		synch_wait();
	}
	if(count_r%N==0){
		count_r++;
		for(i=1; i<N && i<=(i_red-count_r); i++) {
			synch_notify();
		}
	}
	else {
		count_r++;
	}
	
	synch_end(red);
	
	printf("red %d\n",number);
	
	pthread_mutex_lock(&mtx);
	
	un_count_r++;
	if(un_count_r%N == 0 || i_red == un_count_r) {
		printf("count_red %d\n",un_count_r);
		if(i_blue != un_count_b) {
			synch_begin(blue);
			
			synch_notify();
			
			synch_end(blue);
		}
		else {
			if(i_red == un_count_r) {
				synch_begin(main);
				
				synch_notify();
				
				synch_end(main);
			}
			else {
				synch_begin(red);
				
				synch_notify();
				
				synch_end(red);
			}
		}
	}
	pthread_mutex_unlock(&mtx);
	
	return(NULL);
}

void *blue(void *num_b){
	int number;
	int i;
	number = *(int *)(num_b);
	
	synch_begin(blue);
	
	check_blue++;
	if(check_blue == i_blue) {
		synch_notify();
	}
	if(i_blue!=1){
		synch_wait();
	}
	if(count_b%N==0){
		count_b++;
		for(i=1; i<N && i<=(i_blue-count_b); i++) {
			synch_notify();
		}
	}
	else {
		count_b++;
	}
	
	synch_end(blue);
	
	printf("blue %d\n",number);
	
	pthread_mutex_lock(&mtx);
	
	un_count_b++;
	if(un_count_b%N == 0 || i_blue == un_count_b) {
		printf("count_blue %d\n",un_count_b);
		if(i_red != un_count_r) {
			synch_begin(red);
			
			synch_notify();
			
			synch_end(red);
		}
		else {
			if(i_blue == un_count_b) {
				synch_begin(main);
				
				synch_notify();
				
				synch_end(main);
			}
			else {
				synch_begin(blue);
				
				synch_notify();
				
				synch_end(blue);
			}
		}
	}
	pthread_mutex_unlock(&mtx);
	
	return(NULL);
}