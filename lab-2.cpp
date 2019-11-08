#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <conio.h>
 
int main()
{
    
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;  
    if (Process32First(hSnapshot, &pe)) 
    {
        printf("Name obraz                PID   Kol-vo nitei      Prioritet         PID family");
        printf("========================= ===== ================= ================= ============");
        do  
        {
            if (!pe.th32ProcessID) continue;    
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, pe.th32ProcessID);   
            char cPriority[18];
            switch (GetPriorityClass(hProcess)) 
            {
                case HIGH_PRIORITY_CLASS:
                    strcpy(cPriority, "Vishe srednego");
                    break;
                case IDLE_PRIORITY_CLASS:
                    strcpy(cPriority, "Nizkii");
                    break;
                case NORMAL_PRIORITY_CLASS:
                    strcpy(cPriority, "Srednii");
                    break;
                case REALTIME_PRIORITY_CLASS:
                    strcpy(cPriority, "Realnogo vremeni");
                    break;
                default:
                    strcpy(cPriority, "Visokii");
            }
            CloseHandle(hProcess);
            
            printf("%25s%6d%18d%18s%12d\n", pe.szExeFile, pe.th32ProcessID,
                   pe.cntThreads, cPriority, pe.th32ParentProcessID);
            
        }
        while (Process32Next(hSnapshot, &pe));  
    }
    else printf("Ne poluchilos' poluchit' informaciu");
    CloseHandle(hSnapshot);
    system("PAUSE");
    return 0;
}
