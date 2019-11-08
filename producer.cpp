//
//  main.cpp
//  producer-consumer
//
//  Created by Anton Korol 
//  Copyright © 2019 Anton Korol. All rights reserved.
//
#include "queue.h"
#include <mutex>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#define MAX_PRODUCERS 6
#define MAX_CONSUMERS 9
#define MAX 25
#define READ_OUTPUT
 
using namespace std;
 
DWORD uThrProdID[MAX_PRODUCERS], uThrConsID[MAX_CONSUMERS];
 
HANDLE eventA,eventB,eventC,eventD,eventE;
HANDLE hThrProdArray[MAX_PRODUCERS], hThrCons[MAX_CONSUMERS];
 
void ProducerThread(char name[11]);
void ConsumerThread(char name[11]);
class queue;

int tail = 0;
int head = 0;
int countEl = 0;
mutex mt;
//void FlushBuf(void);
//void PutChar(int sym);
//int GetChar(void);
queue q=new queue(MAX);

int main()
{ 
    q=new queue(MAX);
    
 
    eventA = CreateEvent(NULL, FALSE, FALSE, NULL);
    eventB= CreateEvent(NULL, FALSE, TRUE, NULL);
    eventC = CreateEvent(NULL, FALSE, TRUE, NULL);
 	eventD = CreateEvent(NULL, FALSE, TRUE, NULL);
 	eventE = CreateEvent(NULL, FALSE, TRUE, NULL);
    for ( int i = 0; i < MAX_PRODUCERS; i++ )
    {
        char* prod = new char[11];
        sprintf(prod, "Producer %d", i+1);
        hThrProdArray[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ProducerThread, prod, 0, &uThrProdID[i]);
    }
 for(int i=0;i<MAX_CONSUMERS;i++)
 {
 	char* cons=new char[11];
 	sprintf(cons,"Consumer %d",i+1);
    hThrCons[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConsumerThread, cons, 0, &uThrConsID[i]);
}

    getch();
 
 
    CloseHandle(hEventBuf);
    CloseHandle(hEventRead);
    CloseHandle(hEventWrite);
 
    for ( int i = 0; i < MAX_PRODUCERS; i++ )
        CloseHandle(hThrProdArray[i]);
 
 	for ( int i = 0; i < MAX_CONSUMERS; i++ )
   		CloseHandle(hThrCons[i]);
    return 0;
}
 
void ProducerThread(char name[11])
{
    while(1)
    {
        WaitForSingleObject(eventA, INFINITE);
        Sleep(rand()%500);
        if (q.size() ==MAX)
        {	WaitForSingleObject(eventC, INFINITE);
            
        }
        WaitForSingleObject(eventE, INFINITE);
        if(q.size()==1)
        {
        	q.enqueue(int(name[9]));
        	SetEvent(eventE);
		}
		else
		{SetEvent(eventE);
			q.enqueue(int(name[9]));
		}
        mt.lock();
        cout <<  name << ": There are " << q.size() << " symbols in buf" << endl << endl;
        mt.unlock();
		Sleep(2500);
		if(q.size()==1)
		setEvent(eventD);
        SetEvent(eventA);
    }
}
 
void ConsumerThread(char name[11])
{
    WaitForSingleObject(hEventWrite, INFINITE);
    while(1)
    {
        Sleep(rand()%50);
        if (q.size() == 0)
        {
            SetEvent(hEventRead);
            WaitForSingleObject(hEventWrite, INFINITE);
        }

		mt.lock();
       cout <<name<<" :I've read one symbol. There are " << q.size() << " symbols in buf now. " << endl << endl;
        //printf("%s :I've read one symbol. There are %d symbols in buf now. \n\n",name,count);
		mt.unlock();
		Sleep(2500);
        SetEvent(hEventRead);
    }
}

//class queue
//{
//	int *arr;		// array to store queue elements
//	int capacity;	// maximum capacity of the queue
//	int front;		// front points to front element in the queue (if any)
//	int rear;		// rear points to last element in the queue
//	int count;		// current size of the queue
//
//public:
////	queue(int size = SIZE); 	// constructor
////	~queue();   				// destructor
////
////	void dequeue();
////	void enqueue(int x);
////	int peek();
////	int size();
////	bool isEmpty();
////	bool isFull();
//
//
//// Constructor to initialize queue
//queue(int size)
//{
//	arr = new int[size];
//	capacity = size;
//	front = 0;
//	rear = -1;
//	count = 0;
//}
//
//// Destructor to free memory allocated to the queue
//~queue()
//{
//	delete arr;
//}
//
//// Utility function to remove front element from the queue
//void dequeue()
//{
//	// check for queue underflow
//	if (isEmpty())
//	{
//		cout << "UnderFlow\nProgram Terminated\n";
//		exit(EXIT_FAILURE);
//	}
//
//	cout << "Removing " << arr[front] << '\n';
//
//	front = (front + 1) % capacity;
//	count--;
//}
//
//// Utility function to add an item to the queue
//void enqueue(int item)
//{
//	// check for queue overflow
//	if (isFull())
//	{
//		cout << "OverFlow\nProgram Terminated\n";
//		exit(EXIT_FAILURE);
//	}
//
//	cout << "Inserting " << item << '\n';
//
//	rear = (rear + 1) % capacity;
//	arr[rear] = item;
//	count++;
//}
//
//// Utility function to return front element in the queue
//int peek()
//{
//	if (isEmpty())
//	{
//		cout << "UnderFlow\nProgram Terminated\n";
//		exit(EXIT_FAILURE);
//	}
//	return arr[front];
//}
//
//// Utility function to return the size of the queue
//int size()
//{
//	return count;
//}
//
//// Utility function to check if the queue is empty or not
//bool isEmpty()
//{
//	return (size() == 0);
//}
//
//// Utility function to check if the queue is full or not
//bool isFull()
//{
//	return (size() == capacity);
//}
//
//};


 
//void FlushBuf(void)
//{
//    tail = 0;
//    head = 0;
//    count = 0;
//}
// 
//void PutChar(int sym)
//{
//    WaitForSingleObject(hEventBuf, INFINITE);
//    if (count < SIZE_BUF)
//    {
//        cycleBuf[tail] = sym;
//        count++;
//        tail++;
//        if (tail == SIZE_BUF) tail = 0;
//    }
//    SetEvent(hEventBuf);
//}
// 
//int GetChar(void)
//{
//    int sym = 0;
//    if (count > 0)
//    {
//        sym = cycleBuf[head];
//        count--;
//        head++;
//        if (head == SIZE_BUF) head = 0;
//    }
//    return sym;
//}
