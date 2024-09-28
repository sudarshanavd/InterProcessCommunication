#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define SHARED_MEMORY_SIZE sizeof(int)
#define SHARED_MEMORY_NAME "sharedMemoryExample"

int main(int argc,char *argv[]){
    HANDLE hMapFile;
    int* pBuf;
    int m=atoi(argv[1]);
    FILE *fp;

    fp = fopen ( "producer_list.txt", "w" ) ;
    if ( fp == NULL )
    {
        puts ( "Cannot open file" ) ;
        exit ( 1 ) ;
    }

    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        SHARED_MEMORY_SIZE,
        SHARED_MEMORY_NAME );

    if (hMapFile == NULL){
        printf("could not create file mapping object (%d).\n",GetLastError());
        return 1;
    }

    pBuf = (int*)MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        SHARED_MEMORY_SIZE
    );
    if (pBuf == NULL){
        printf("could not map view of file (%d). \n",GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    *pBuf = -999;
    srand(time(NULL));
    for (int i = 0; i < m; i++){
        if (*pBuf == -999){
        int number = rand() % 100 + 1;
        *pBuf = number;
        fprintf( fp,"%d) Producer: Created number %d\n",i+1,number);
        } 
        while(*pBuf != -999);
    }

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}