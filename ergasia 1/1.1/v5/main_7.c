#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>

#define N 10
// in h thesh poy THA diabasoyme
// out h thesh poy Diabasame
int in1=0;
int in2=0;
int out1=0;
int out2=0;

int end_flag1=0;
int end_flag2=0;
int end_flag3=0;

char array1[N];
char array2[N];

void * p1_main(void *arg);
void * p2_main(void *arg);
void * p3_main(void *arg);

int main(int argc, char *argv[]){
	int check;
	pthread_t p1,p2,p3;
	
	check = pthread_create(&p1,NULL,&p1_main,NULL);
	if(check!=0){
		//yparxei thema
		printf("fault pthread 1\n");
		return(1);
	}
	check = pthread_create(&p2,NULL,&p2_main,NULL);
	if(check!=0){
		//yparxei thema
		printf("fault pthread 2\n");
		return(1);
	}
	check = pthread_create(&p3,NULL,&p3_main,NULL);
	if(check!=0){
		//yparxei thema
		printf("fault pthread 3\n");
		return(1);
	}
	
	while(end_flag3!=1){}
	
	return(0);
}

void *p1_main(void *arg){
	char l1,l2;
	
	do{//bgazoume ta perita kena sthn arxh
		l1=getchar();
		if(l1==EOF){
			end_flag1 = 1;
			return(NULL);
		}
	}
	while(l1=='\n' || l1==' ');
	
	
	while(end_flag1!=1){
		l2=getchar();
		if(l2==EOF){
			end_flag1 = 1;
		}
		
		while((in1+1)%N==out1){}//synthiki anamonhs
		
		if(l1!='\n' && l1!=' '){
			array1[in1] = l1;
			in1 = (in1+1)%N;
			l1 = l2;
			continue;
		}
		else{
					
			while(l2=='\n' || l2==' '){
				l2 = getchar();
				if(l2==EOF){
					end_flag1 = 1;
					break;
				}
			}
			if(end_flag1 != 1) {
				array1[in1] = ' ';
				in1 = (in1+1)%N;
			}
			l1 = l2;
		}
	}
	
	return(NULL);
}

void * p2_main(void *arg){
	
	while(1){
		if(end_flag1==1 && in1 == out1){
			end_flag2 = 1;
			break;
		}
		
		while(in1==out1 && end_flag1!=1) {}//einai out giati ayth einai h thesh poy theloyme na diabasoyme
		
		while((in2+1)%N==out2){}
		
		if(in1==out1 && end_flag1==1) {
			continue;
		}
		
		array2[in2] = toupper(array1[out1]);
		out1 = (out1+1)%N;
		in2 = (in2+1)%N;
	}
	
	return(NULL);
}

void * p3_main(void *arg){
	int k;

		while(1){
			if(end_flag2==1 && in2==out2) {
				end_flag3 = 1;
				break;
			}
				
			for(k=0; k<30 && (in2!=out2 ||(end_flag2!=1)); k++){
				
				while(in2==out2 && end_flag2!=1) {}
				
				putchar(array2[out2]);
				out2 = (out2+1)%N;
				
				fflush(NULL);
				
			}
				putchar('\n');
			
		}

	
	return(NULL);
}







