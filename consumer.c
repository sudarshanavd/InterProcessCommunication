#include <stdio.h>
#include <windows.h>

#define SHARED_MEMORY_SIZE sizeof(int)
#define SHARED_MEMORY_NAME "sharedMemoryExample"

int main(int argc,char *argv[]){
    HANDLE hMapFile;
    int* pBuf;
    int n=atoi(argv[1]);
    FILE *fp;

    fp = fopen ( "consumer_list.txt", "w" ) ;
    if ( fp == NULL )
    {
        puts ( "Cannot open file" ) ;
        exit ( 1 ) ;
    }

    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
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

    for (int i = 0; i < n; i++){
        int number = *pBuf;
        if (*pBuf != -999){
        if (number % 2 == 0){
            fprintf( fp,"%d) Consumer: %d is EVEN\n",i+1,number);
        } else {
            fprintf( fp,"%d) Consumer: %d is ODD\n",i+1,number);
        }
        *pBuf=-999;
        }
        while(*pBuf == -999 && i < n-1);
    }
    
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}