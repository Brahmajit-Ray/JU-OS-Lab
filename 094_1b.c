/*

*Name-Brahmajit Ray
 Roll-002211001094
 5th August 11:35

*Question:Program to GET and SET the Environment variable and to know use of getenv and setenv system calls.Now, Set two new environment variables and display them.

*Compliation Command:gcc 094_1b.c -std=c99
*Output Command:./a.out

*Sample Output

USER:be2294
HOME:/home/usr/student/ug/yr22/be2294
HOST:(null)
ARCH:(null)
DISPLAY:(null)
PRINTER:(null)
PATH:/usr/lib64/qt-3.3/bin:/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/home/usr/student/ug/yr22/be2294/.local/bin:/home/usr/student/ug/yr22/be2294/bin
HELLO:OS_LAB
WORLD:OS_Lab2

*/

#include <stdlib.h>
#include <stdio.h>

int main(){
        char* arr[]={"USER","HOME","HOST","ARCH","DISPLAY","PRINTER","PATH"};

        setenv("HELLO","OS_LAB",1);
        setenv("WORLD","OS_Lab2",1);

        for(int i=0;i<7;i++){
                const char* name=arr[i];
                printf("%s:%s\n",arr[i],getenv(arr[i]));
        }

        printf("%s:%s\n","HELLO",getenv("HELLO"));
        printf("%s:%s\n","WORLD",getenv("WORLD"));
        return 0;
}
