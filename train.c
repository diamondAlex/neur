#include <stdio.h>
#include <string.h>
#include <errno.h>
#define w 400
#define h 300

void printarr(int* n){
    for(int i = 0;i<h;i++){
        for(int j = 0;j<w;j++){
            printf("%i",n[i*w+j]);
        }
        printf("\n");
    }
}
int main(){
    FILE* f = fopen("test.img", "r");
    if(f == NULL){
        printf("error ; %s\n", strerror(errno));
    }

    int n[w*h] = {0};
    char c;
    int i=0;
    while((c = getc(f)) != EOF){
        if(c == '0'){
            n[i] = 0;
        }else{
            n[i] = 1;
        }
        i++;
    }
    printarr(n);
}
