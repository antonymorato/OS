//reader-writer

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>

#define MAX_WRITERS 2
#define MAX_READERS 4

void writer(char *name);
void reader(char *name);
DWORD uThrWriterID[MAX_WRITERS], uThrReaderID[MAX_READERS];
HANDLE semArr[2];
HANDLE semWrite,semRead,semForRead;
HANDLE hThrWritersArray[MAX_WRITERS], hThrReadersArray[MAX_READERS];
int readcount=0,writecount=0,container;
int main()
{	container=0;
	semWrite=CreateSemaphore(NULL,1,1,NULL);
	semRead=CreateSemaphore(NULL,1,MAX_READERS,NULL);
	semForRead=CreateSemaphore(NULL,1,1,NULL);
	
	semArr[0]=semRead;
	semArr[1]=semForRead;
	
	for ( int i = 0; i < MAX_WRITERS; i++ )
    {
        char* prod = new char[11];
        sprintf(prod, "Writer %d", i+1);
        hThrWritersArray[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)writer, prod, 0, &uThrWriterID[i]);
    }
 for(int i=0;i<MAX_READERS;i++)
 {
 	char* cons=new char[11];
 	sprintf(cons,"Reader %d",i+1);
    hThrReadersArray[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)reader, cons, 0, &uThrReaderID[i]);
}
	getch();
	CloseHandle(semWrite);
    CloseHandle(semRead);
    CloseHandle(semForRead);
    for ( int i = 0; i < MAX_WRITERS; i++ )
        CloseHandle(hThrWritersArray[i]);
 
 	for ( int i = 0; i < MAX_READERS; i++ )
   		CloseHandle(hThrReadersArray[i]);
	return 0;
}

void reader(char *name)
{	while(true)
	{ 
		WaitForMultipleObjects(2,semArr,TRUE,INFINITE);
		Sleep(rand()%1000);
		printf("%s\tValue:%d\n\n",name,container);
		Sleep(rand()%2500);
		ReleaseSemaphore(semRead,1,NULL);
		ReleaseSemaphore(semForRead,1,NULL);
		}
	
	
//	int num1=*(int*)prm;
//	int i=0,r;
//	for(i;i<iter;i++)
//	{
		
//		if (sem_wait(&orderM)==0) printf("%d Читатель %d в очереди__________Ч%d\n",i,num1,num1);	// Remember our order of arrival
//		sem_wait(&readresM);				 // We will manipulate the readers counter
//		if (readers == 0)				// If there are currently no readers (we came first)...
//			sem_wait(&accessM);				// ...requests exclusive access to the resource for readers
//		readers++;							 // Note that there is now one more reader
//		sem_post(&orderM);					 // Release order of arrival semaphore (we have been served)
//		sem_post(&readresM);				 // We are done accessing the number of readers for now
//
//		printf("%d Работает читатель %d________________Ч%d\n",i,num1,num1);				// Here the reader can read the resource at will
//		r=1+rand()%4;
//		sleep(r);
//		sem_wait(&readresM);				 // We will manipulate the readers counter
//		readers--;							 // We are leaving, there is one less reader
//		if (readers == 0)				// If there are no more readers currently reading...
//			sem_post(&accessM);				// ...release exclusive access to the resource
//		sem_post(&readresM);				 // We are done accessing the number of readers for now
//	}
}

void writer(char *name)
{
	while(true)
	{
		WaitForSingleObject(semWrite,INFINITE);
		Sleep(rand()%500);
		container+=5;
		printf("%s wrote,value:%d\n\n",name,container);
		Sleep(rand()%2500);
		ReleaseSemaphore(semWrite,1,NULL);
		
		
		
		
	}
	
	
	
	
//	int num2=*(int*)prm;
//	int j=0,r;
//	for(j;j<iter;j++)
//	{
//		if(sem_wait(&orderM)==0) printf("%d Писатель %d в очереди__________П%d\n",j,num2,num2); // Remember our order of arrival
//		sem_wait(&accessM);					// Request exclusive access to the resource
//		sem_post(&orderM);					 // Release order of arrival semaphore (we have been served)
//
//		printf("%d Работает писатель %d________________П%d\n",j,num2,num2);				 // Here the writer can modify the resource at will
//		r=1+rand()%4;
//		sleep(r);
//		sem_post(&accessM);					// Release exclusive access to the resource
//	}
}


