#include <dirent.h> 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define w 300
#define h 300

void check_error(int tag, double l1[], double l2[],double n1[],double nr[]);
void save_net(char *name, double *arr, int length);

void printarr(int* n){
    for(int i = 0;i<h;i++){
        for(int j = 0;j<w;j++){
            printf("%i",n[i*w+j]);
        }
        printf("\n");
    }
}

int run_neur(int n[], double l1[], double l2[],double n1[],double nr[]){
    int bias = 5;
    for(int i=0;i<16;i++){
        /*printf("%f\n",l1[i]);*/
    }
    printf("running n1\n");
    for(int i=0;i<w;i++){
        for(int j=0;j<w;j++){
            n1[i] += n[j]*l1[j+(w*i)]; 
        }

        n1[i] = 1/(1+exp(-n1[i]+bias));
    }
    for(int i=0;i<w;i++){
        for(int j=0;j<w;j++){
            nr[i] += n1[j]*l2[j+(w*i)]; 
        }
        nr[i] = 1/(1+exp(-nr[i]+bias));
    }

    printf("nr[0] = %f,",nr[0]);
    printf("nr[1] = %f\n",nr[1]);
    check_error(0,l1,l2,n1,nr);
    if(nr[0]>nr[1])
        return 1;
    else
        return 2;
    /*printf("saving\n");*/
    /*save_net("l1",l1,w*h);*/
    /*save_net("l2",l2,w*h);*/
}

void check_error(int tag, double l1[], double l2[],double n1[],double nr[]){
    double error[4];
    for(int i=0;i<4;i++){
        if(i == tag){
            error[i] = 1-nr[i];
        }else{
            error[i] = 0-nr[i];
        }
        /*printf("error = %f\n",error[i]);*/
    }
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            int index=  i*w+j;
            /*double bef = l2[index];*/
            l2[index] += error[i];
            /*printf("before=%f, error=%f, after=%f, %i.%i\n",bef,error[i],l2[index],i,j);*/
        }
    }
}

void save_net(char *name, double *arr, int length){
    char buff[100];
    sprintf(buff, "%s.neur",name);
    FILE* f = fopen(buff, "w");

    double* p_arr = arr;

    for(int i=0;i<length;i++){
        fwrite(p_arr,sizeof(double),1,f);
        p_arr++;
    }
    fclose(f);
}

void read_net(char* name){
    FILE* f = fopen(name,"r");
    if(!f){
        printf("couldn't open file");
        return;
    }
    double neur[100000];
    double* p;
    p = neur;
    int nbr;
    while((nbr=fread(p, sizeof(double), 1, f)) == 1){
        printf("*p = %f\n",*p);
        p++;
    }
}

int get_image_list(char filenames[][100]){
    DIR *d;
    struct dirent *dir;
    d = opendir("images");
    if(d){
        while((dir = readdir(d)) != NULL){
            if(!strcmp(dir->d_name,"..") || !strcmp(dir->d_name,"."))
                continue;
            char buff[100];
            sprintf(buff,"images/%s",dir->d_name);

            memcpy(*filenames, buff,256);
            (filenames)++;
        }
        closedir(d);
    }
    return 1;
}



int main(){

    int n[w*h] = {0};
    double l1[w*h] = {0};
    double l2[w*h] = {0};
    double n1[w] = {0};
    double nr[2] = {0};

    int i=0;


    while(i < w*h){
        l1[i] = ((rand()/1E9)*4);
        l2[i] = ((rand()/1E9)*4);
        i++;
    }

    char imagelist[10][100];
    printf("getting imgs\n");
    get_image_list(imagelist);


    FILE* f = fopen(imagelist[0], "r");
    if(f == NULL){
        printf("error ; %s\n", strerror(errno));
        return 0;
    }


    printf("get the file\n");

    i=0;
    char c;
    while((c = getc(f)) != EOF && i!=(w*h)){
        if(c == '0'){
            n[i] = 0;
        }else{
            n[i] = 1;
        }
        i++;
    }

    /*for(int i=0;i<10;i++){*/
    int res = run_neur(n, l1, l2, n1, nr);
    printf("res = %i\n",res);
    /*}*/
    /*printarr(n);*/
    return 1;
}
