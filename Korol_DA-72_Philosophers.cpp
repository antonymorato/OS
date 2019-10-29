
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>
#define MAX_PHILOSOPHERS 5
using namespace std;
 
class CPhilosof;
CRITICAL_SECTION forkCriticalSection[MAX_PHILOSOPHERS];
HANDLE hPhilThread[MAX_PHILOSOPHERS];
DWORD uThreadIDs[MAX_PHILOSOPHERS];
CPhilosof* pfilos[MAX_PHILOSOPHERS];
 
class CPhilosof{
public:
    CPhilosof() { iHungry=0; }
    int iHungry;
    int iNum;
    
    bool eat()
    {
        if( (pfilos[(iNum+1)%5]->iHungry) <= iHungry && (pfilos[(iNum+4)%5]->iHungry) <= iHungry )   
        {
            if (TryEnterCriticalSection(&forkCriticalSection[iNum]))
            {
                
                if (TryEnterCriticalSection(&forkCriticalSection[(iNum+1)%5]))
                {
                   
 					printf("Philosof %d began to eat\n",iNum+1);
                    Sleep(rand()%3000+1000);
 
                    LeaveCriticalSection(&forkCriticalSection[(iNum+1)%5]);
                    
                    LeaveCriticalSection(&forkCriticalSection[iNum]);
                    printf("Philosof %d ended to eat\n",iNum+1);
                    Sleep(rand()%3000+1000);
                    return true;
                } 
                
                
                    
					LeaveCriticalSection(&forkCriticalSection[iNum]);
                  //  printf("Philosof %d is thinking\n",iNum+1);
					return false;
                
            }
        }
    };
};
 
 
void body(LPVOID lpParam)
{
    CPhilosof* iFilos;
    iFilos = (CPhilosof*)lpParam;
    srand(time(NULL));
    while (1)
    {	
        if (!(iFilos->eat()))
        {
            (iFilos->iHungry)++;
        
        
			Sleep(rand()%300+100);
        }
        else
        {
            iFilos->iHungry = 0;
           
			Sleep(rand()%300+100);
        }
    }
};
 
 
int main()
{
    srand(time(NULL));
 
    for ( int i = 0; i < 5; i++ )
    {
        InitializeCriticalSection( &forkCriticalSection[i] );
        pfilos[i] = new CPhilosof;
    }
    
    for ( int i = 0; i < 5; i++ )
    {
        pfilos[i]->iNum = i;
        hPhilThread[i] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)body,pfilos[i],0,&uThreadIDs[i]);
    }
 
    _getch();
    for ( int i = 0; i < 5; i++ )
        CloseHandle(hPhilThread[i]);
    return 0;
}
