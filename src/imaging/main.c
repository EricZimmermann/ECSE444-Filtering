#include <stdio.h>
#include <stdlib.h>
#include <image.h>

#define MAXCHAR 100000


int main(int argc, char* argv[]){
    printf("Image processing \n");
    while((argc > 1) && (argv)){
        switch(argv[1][1]){
            case 'i':
                printf("This is for the input image, open an image in this step.\n");
                FILE *fp;
                char str[MAXCHAR];
                printf("%s", argv[2]);
                printf("\n");
                fp = fopen(argv[2], "r");
                if(fp == NULL){
                    printf("can't open file, please provide the full path\n");
                    return 1;
                }
                int i = 0;
                while (fgets(str, MAXCHAR, fp) != NULL){
                    printf("%s\n", str);
                    i++;
                }
                fclose(fp);
                printf("%d\n", i);
                break;
            case 'o':
                printf("This is for the output\n");
                break;
            case 's':
                printf("This is for the output\n");
                break;
            case 'p':
                printf("This is for the output\n");
                break;
            case 'r':
                printf("This is for the output\n");
                break;
        }
        argc--;
        argv++;
    }
    return 0;
}