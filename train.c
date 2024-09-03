#include <dirent.h> 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define h 10
#define w 20
//neural number
#define nn 20
//neural number result
#define nnr 20

void check_error(int tag, double l1[], double l2[],double n1[],double nr[]);
void save_net(char *name, double *arr, int length);

void printarr(int* n){
    for(int i = 0;i<h;i++){
        for(int j = 0;j<w;j++){
            /*printf("%i",n[i*w+j]);*/
        }
        /*printf("\n");*/
    }
}

int run_neur(int tag,int n[], double l1[], double l2[],double n1[],double nr[]){
    int bias = 10;

    /*printf("running n1\n");*/

    for(int i=0;i<nn;i++){
        for(int j=0;j<h*w;j++){
            n1[i] += n[j]*l1[j+(w*h*i)]; 
        }

        n1[i] = 1/(1+exp(-n1[i]+bias));
    }
    /*printf("running nr\n");*/
    for(int i=0;i<nnr;i++){
        for(int j=0;j<(nn);j++){
            nr[i] += n1[j]*l2[j+(nn*i)]; 
        }
        nr[i] = 1/(1+exp(-nr[i]+bias));
    }

    check_error(tag,l1,l2,n1,nr);

    for(int i=0;i<10;i++){
        printf("%f\n",nr[i]);
    }
    printf("\n");

    int max = 0;
    for(int i=0;i<10;i++){
        if(nr[i] > nr[max])
            max = i;
    }
    save_net("l1",l1,w*h*nn);
    save_net("l2",l1,nnr*nn);
    return max;
}

void check_error(int tag, double l1[], double l2[],double n1[],double nr[]){
    double error[nnr];
    for(int i=0;i<nnr;i++){
        if(i == tag){
            error[i] = 1-(nr[i] * nr[i]);
        }else{
            error[i] = 0-(nr[i] * nr[i]);
        }
        /*printf("error = %f\n",error[i]);*/
    }
    for(int i=0;i<nnr;i++){
        for(int j=0;j<nn;j++){
            int index=  i*nn + j;
            /*double bef = l2[index];*/
            l2[index] += error[i];
            /*printf("before=%f, error=%f, after=%f, %i.%i\n",bef,error[i],l2[index],i,j);*/
        }
    }
}

void save_net(char *name, double *arr, int length){
    char buff[w*h*nn];
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
            char buff[256];
            sprintf(buff,"images/%s",dir->d_name);

            memcpy(*filenames, buff,256);
            (filenames)++;
        }
        closedir(d);
    }
    return 1;
}

int getTag(char *name){
    while(*name != '_')
        name++;
    name++;
    int n_i = *name;
    n_i -= 48;
    return n_i;
}

int main(){

    int n[w*h] = {0};
    double l1[w*h*nn] = {0};
    double l2[nn*nnr] = {0};
    double n1[nn] = {0};
    double nr[nnr] = {0};

    int i=0;
    while(i < w*h*nn){
        l1[i] = ((rand()/1E9)*4);
        i++;
    }

    i=0;
    while(i < w*h*nnr){
        l2[i] = ((rand()/1E9)*4);
        i++;
    }

    char imagelist[10][100];
    get_image_list(imagelist);


    for(int img=0;img<1;img++){
        FILE* f = fopen(imagelist[img], "r");
        if(f == NULL){
            continue;
        }

        int tag = getTag(imagelist[img]);
        /*printf("tag = %s\n",imagelist[img]);*/
        printf("tag = %i\n",tag);
        /*printf("\n");*/

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

        int res;
        for(int i=0;i<20;i++){
            res = run_neur(tag, n, l1, l2, n1, nr);
        }
        printf("res = %i\n",res);
        printf("\n");
    }

    return 1;
}
