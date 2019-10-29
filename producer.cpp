#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#define MAX_PRODUCERS 6
#define MAX_CONSUMERS 6
#define SIZE_BUF 25
//#define READ_OUTPUT
 
using namespace std;
 
DWORD uThrProdID[MAX_PRODUCERS], uThrConsID[MAX_CONSUMERS];
 
HANDLE hEventWrite, hEventRead, hEventBuf;
HANDLE hThrProdArray[MAX_PRODUCERS], hThrCons[MAX_CONSUMERS];
 
void ProducerThread(char name[11]);
void ConsumerThread(char name[11]);
 
int cycleBuf[SIZE_BUF];
int tail = 0;
int head = 0;
int count = 0;
 
void FlushBuf(void);
void PutChar(int sym);
int GetChar(void);
 
FILE *fw;
 
#ifdef READ_OUTPUT
FILE *fr;
#endif
 
int main()
{
    fw = fopen("bufw.txt", "w");
#ifdef READ_OUTPUT
    fr = fopen("bufr.txt", "w");
#endif
    FlushBuf();
 
    hEventWrite = CreateEvent(NULL, FALSE, FALSE, NULL);
    hEventBuf = CreateEvent(NULL, FALSE, TRUE, NULL);
    hEventRead = CreateEvent(NULL, FALSE, TRUE, NULL);
 
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
//hThrCons = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConsumerThread, NULL, 0, &uThrConsID);
    getch();
 
    fclose(fw);
#ifdef READ_OUTPUT
    fclose(fr);
#endif
 
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
        WaitForSingleObject(hEventRead, INFINITE);
        Sleep(rand()%500);
        if (count < SIZE_BUF)
        {
            for ( int i = 0; i < 3; i++ )
            {
                PutChar(name[9]);
                putc(name[9], fw);
            }
            putc(13, fw);
        }
        cout <<  name << ": There are " << count << " symbols in buf" << endl << endl;
        Sleep(2500);
        SetEvent(hEventWrite);
    }
}
 
void ConsumerThread(char name[11])
{
    WaitForSingleObject(hEventWrite, INFINITE);
    while(1)
    {
        Sleep(rand()%50);
        if (count == 0)
        {
            SetEvent(hEventRead);
            WaitForSingleObject(hEventWrite, INFINITE);
        }
#ifdef READ_OUTPUT
        putc(GetChar(), fr);
#else
        GetChar();
#endif
       cout <<name<<" :I've read one symbol. There are " << count << " symbols in buf now. " << endl << endl;
        //printf("%s :I've read one symbol. There are %d symbols in buf now. \n\n",name,count);
		Sleep(2500);
        SetEvent(hEventRead);
    }
}
 
void FlushBuf(void)
{
    tail = 0;
    head = 0;
    count = 0;
}
 
void PutChar(int sym)
{
    WaitForSingleObject(hEventBuf, INFINITE);
    if (count < SIZE_BUF)
    {
        cycleBuf[tail] = sym;
        count++;
        tail++;
        if (tail == SIZE_BUF) tail = 0;
    }
    SetEvent(hEventBuf);
}
 
int GetChar(void)
{
    int sym = 0;
    if (count > 0)
    {
        sym = cycleBuf[head];
        count--;
        head++;
        if (head == SIZE_BUF) head = 0;
    }
    return sym;
}
