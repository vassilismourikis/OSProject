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
pthread_mutex_t c,o,st,mt,scl;
pthread_cond_t condc,condo;
int summaryTime=0;
int maxTime=0;

int main(int argc,char **argv)
{
	int return_code;
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
	//printf("waiting for next order \n");
	sleep(Tol+ rand_r(&seed)%(Toh-Tol+1));
	ords[i].pizzas=Nol+ rand_r(&seed)%(Noh-Nol+1);
	return_code=pthread_create(&th[i],NULL,prep,&ids[i]);
//CHECK
	if (return_code != 0) {
            printf("ERROR: return code from pthread_create() is %d\n", return_code);
            exit(-1);
        }
//CHECK
	}
	
	
	
	//wait to end
	void* status;
	for(int i=0;i<cust;i++){
		return_code=pthread_join(th[i],&status);
//CHECK
		if (return_code != 0) {
            		printf("ERROR: return code from pthread_join() is %d\n", return_code);
            		exit(-1);
        	}
//CHECK
	}
	printf("MESOS OROS XRONOU %d\n",summaryTime/cust);
	printf("MEGISTOS XRONOS %d\n",maxTime);
	free(ids);
	free(times);
	free(ords);

	return 0;
}

void * prep(void * ar){
	int return_code;
	int* num=(int*)ar;
	struct order * preperator=&ords[*num];
	preperator->number=*num;
	return_code=clock_gettime(CLOCK_REALTIME,&t);
//CHECK
 	if(return_code==-1){
        	printf("ERROR: Failure getting real time.");
        	pthread_exit(&return_code);
    	}
//CHECK
	times[*num]=t.tv_sec;
	//START PREPERATORS
	//lock
	return_code=pthread_mutex_lock(&c);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_lock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
	while(cookNum==0){
	//printf("waiting for cooker id: %d\n",preperator->number);
	return_code=pthread_cond_wait(&condc,&c);
//CHECK
	if (return_code != 0) {
    		printf("ERROR: return code from pthread_cond_wait() is %d\n", return_code);
    		pthread_exit(&return_code);
    	}
//CHECK
	}
	
//printf("STARTING PREPERATOR %d\n",cookNum);
cookNum-=1;
	//unlock
	return_code=pthread_mutex_unlock(&c);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
	sleep(Tprep*preperator->pizzas);
	

	
	
	
	//START OVENS
	return_code=pthread_mutex_lock(&o);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_lock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
	while(ovenNum==0){
		//printf("waiting for oven id: %d\n",preperator->number);
		pthread_cond_wait(&condo,&o);
//CHECK
		if (return_code != 0) {
    		printf("ERROR: return code from pthread_cond_wait() is %d\n", return_code);
    		pthread_exit(&return_code);
    		}
//CHECK
	}
	
//printf("STARTING OVEN %d\n",ovenNum);
ovenNum-=1;
	return_code=pthread_mutex_unlock(&o);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
	sleep(Tbake*preperator->pizzas);
	
	return_code=pthread_mutex_lock(&o);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_lock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
	ovenNum+=1;
	return_code=pthread_cond_signal(&condo);
	if (return_code != 0) {
    		printf("ERROR: return code from pthread_cond_signal() is %d\n", return_code);
    		pthread_exit(&return_code);
    	}

	return_code=pthread_mutex_unlock(&o);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
	//END OVENS
	
	return_code=pthread_mutex_lock(&c);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_lock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
	//here the increasment because cooker works even through baking
	cookNum+=1;
	pthread_cond_signal(&condc);
	if (return_code != 0) {
    		printf("ERROR: return code from pthread_cond_signal() is %d\n", return_code);
    		pthread_exit(&return_code);
    	}
	return_code=pthread_mutex_unlock(&c);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
	//END PREPERATORS
	//Final print
return_code=clock_gettime(CLOCK_REALTIME,&t);
//CHECK
	if(return_code==-1){
        	printf("ERROR: Failure getting real time.");
        	pthread_exit(&return_code);
    	}
//CHECK
	times[*num]=t.tv_sec-times[*num];
//LOCKING SCREEN
return_code=pthread_mutex_lock(&scl);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_lock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
printf("Order whith number %d prepared in %d minutes.\n",
	preperator->number,times[*num]);
return_code=pthread_mutex_unlock(&scl);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
//UNLOCKING SCREEN
//ADDING TO SUMMARY TIME
return_code=pthread_mutex_lock(&st);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_lock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
summaryTime+=times[*num];
return_code=pthread_mutex_unlock(&st);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
//UPDATING MAX TIME
if(preperator->number==0){
return_code=pthread_mutex_lock(&mt);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_lock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
	maxTime=times[*num];
return_code=pthread_mutex_unlock(&mt);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
}
else if(times[*num]>maxTime){
return_code=pthread_mutex_lock(&mt);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_lock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
maxTime=times[*num];
return_code=pthread_mutex_unlock(&mt);
//CHECK
	if (return_code != 0) {
  		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", return_code);
  		pthread_exit(&return_code);
  	}
//CHECK
}
	pthread_exit(NULL);
}


