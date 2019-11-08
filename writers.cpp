//
//  main.cpp
//  reader-writer
//
//  Created by Anton Korol 
//  Copyright © 2019 Anton Korol. All rights reserved.
//
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <mutex>
#define MAX_WRITERS 2
#define MAX_READERS 8
using namespace std;
void writer(char *name);
void reader(char *name);
DWORD uThrWriterID[MAX_WRITERS], uThrReaderID[MAX_READERS];


HANDLE a,b,c;

HANDLE hThrWritersArray[MAX_WRITERS], hThrReadersArray[MAX_READERS];
int flag=0,readcount=0;
mutex mtx;
int main()
{	
 
	a=CreateSemaphore(NULL,1,1,NULL);
	b=CreateSemaphore(NULL,1,1,NULL);
	c=CreateSemaphore(NULL,1,1,NULL);
	
	
	
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
    Sleep(rand()%500);
}
	getch();
	CloseHandle(a);
    CloseHandle(b);
    CloseHandle(c);
    for ( int i = 0; i < MAX_WRITERS; i++ )
        CloseHandle(hThrWritersArray[i]);
 
 	for ( int i = 0; i < MAX_READERS; i++ )
   		CloseHandle(hThrReadersArray[i]);
	return 0;
}

void reader(char *name)
{	while(true)
	{ 	if(flag==1) continue;
		WaitForSingleObject(b,INFINITE);
		if(readcount==0){
		
		WaitForSingleObject(a,INFINITE);

	}
		readcount++;

		ReleaseSemaphore(b,1,NULL);
		

		WaitForSingleObject(c,INFINITE);

		cout<<"There are "<<readcount<<" readers in work."<<endl; 

		ReleaseSemaphore(c,1,NULL);
		Sleep(rand()%500);
		WaitForSingleObject(b,INFINITE);
			readcount--;
			if(readcount==0)
			{
				ReleaseSemaphore(a,1,NULL);

			}
		ReleaseSemaphore(b,1,NULL);
		Sleep(rand()%500);
		}
	
	

}

void writer(char *name)
{	
	while(true)
	{	flag=1;
		WaitForSingleObject(a,INFINITE);
		flag=0;
		Sleep(rand()%500);
		mtx.lock();
		cout<<name<<" is working."<<endl;

		mtx.unlock();

		ReleaseSemaphore(a,1,NULL);
		Sleep(rand()%500);
		
	}
	
	
	
	

}


