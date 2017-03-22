#include<stdio.h>
#include<unistd.h>
#define BUFFER_SIZE 1024

int main()
{
    int i;
    char separationMark='.';
    char buff[BUFFER_SIZE];
    int count;
    while((count=read(0,buff,BUFFER_SIZE))!=0){
        for(i=0;i<count;i++){
            if(i<count-2){
                write(1,&buff[i],1);
                write(1,&separationMark,1);
            }else if(i<count){
                 write(1,&buff[i],1);
            }
        }
    }
return 0;
}
