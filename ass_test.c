#include "ass_test.h"

struct order{
	int number;
	int pizzas;
};

//array with structs of all customers
struct order *ords;
struct timespec t;
int *times;
int ovenNum=Noven;
int cookNum=Ncook;
pthread_mutex_t c,o,st,mt;
pthread_cond_t condc,condo;
int summaryTime=0;
int maxTime=0;
void * prep(void * ar){

	int* num=(int*)ar;
	struct order * preperator=&ords[*num];
	preperator->number=*num;
	clock_gettime(CLOCK_REALTIME,&t);
	times[*num]=t.tv_sec;
	//START PREPERATORS
	//lock
	pthread_mutex_lock(&c);

	while(cookNum==0){
	printf("id: %d\n",preperator->number);
	pthread_cond_wait(&condc,&c);
	}
	
printf("STARTING PREPERATOR %d\n",cookNum);
cookNum-=1;
	//unlock
	pthread_mutex_unlock(&c);
	sleep(Tprep*preperator->pizzas);
	

	
	
	
	//START OVENS
	pthread_mutex_lock(&o);
	while(ovenNum==0){
		printf("id: %d\n",preperator->number);
		pthread_cond_wait(&condo,&o);
	}
	
printf("STARTING OVEN %d\n",ovenNum);
ovenNum-=1;
	pthread_mutex_unlock(&o);
	sleep(Tbake*preperator->pizzas);
	
	pthread_mutex_lock(&o);
	ovenNum+=1;
	pthread_cond_signal(&condo);
	pthread_mutex_unlock(&o);
	//END OVENS
	//here the increasment because cooker works even through baking
	pthread_mutex_lock(&c);
	cookNum+=1;
	pthread_cond_signal(&condc);
	pthread_mutex_unlock(&c);
	//END PREPERATORS
	//Final print
clock_gettime(CLOCK_REALTIME,&t);
	times[*num]=t.tv_sec-times[*num];
	printf("Order whith number %d prepared in %d minutes.\n",
	preperator->number,times[*num]);
//ADDING TO SUMMARY TIME
pthread_mutex_lock(&st);
summaryTime+=times[*num];
pthread_mutex_unlock(&st);
//UPDATING MAX TIME
if(preperator->number==0){
pthread_mutex_lock(&mt);
	maxTime=times[*num];
pthread_mutex_unlock(&mt);
}
else if(times[*num]>maxTime){
pthread_mutex_lock(&mt);
maxTime=times[*num];
pthread_mutex_unlock(&mt);
}
	pthread_exit(NULL);
}

int main(int argc,char **argv)
{

	//checing oparators
	if(argc<3){
	printf("not waited values given\n");
	exit(-1);
	}
	//init args
	int cust =atoi(argv[1]);	
	int seed = atoi(argv[2]);
	//Thread ID
	pthread_t th[cust];
	int *ids=malloc(sizeof(int)*cust);
	ords=malloc(sizeof(struct order)*cust);
	times=malloc(sizeof(int)*cust);
	//array with structs of all customers
	//global struct order ords[cust];
	//creating threads
	for(int i=0;i<cust;i++){
	ids[i]=i;
	printf("waiting for next order \n");
	sleep(Tol+ rand_r(&seed)%(Toh-Tol+1));
	ords[i].pizzas=Nol+ rand_r(&seed)%(Noh-Nol+1);
	pthread_create(&th[i],NULL,prep,&ids[i]);


	}
	
	
	
	//wait to end
	for(int i=0;i<cust;i++){
		pthread_join(th[i],NULL);
	}
	printf("MESOS OROS XRONOU %d\n",summaryTime/cust);
	printf("MEGISTOS XRONOS %d\n",maxTime);
	free(ids);
	free(times);
	free(ords);

	return 0;
}
