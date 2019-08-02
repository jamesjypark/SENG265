/*
 * UVic SENG 265, Fall 2018, A#1
 *
 * This will contain a solution to sengfmt. In order to complete the
 * task of formatting a file, it must open and read the file (hint: 
 * using fopen() and fgets() method) and format the text content base on the 
 * commands in the file. The program should output the formated content 
 * to the command line screen by default (hint: using printf() method).
 *
 * Supported commands include:
 * ?width width :  Each line following the command will be formatted such 
 *                 that there is never more than width characters in each line 
 * ?mrgn left   :  Each line following the command will be indented left spaces 
 *                 from the left-hand margin.
 * ?fmt on/off  :  This is used to turn formatting on and off. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_WIDTH 132

int main(int argc, char *argv[]) {
    FILE *fp;
    char buf[MAX_LINE_WIDTH];
    char testBuf[MAX_LINE_WIDTH];
    char tempLine[MAX_LINE_WIDTH];
    char *token;
    char *formatToken;

    int format = 0;
    int margin = 0;
    int width = 0;

    fp = fopen(argv[1],"r");
    memset(tempLine, 0, sizeof tempLine);
    while (fgets(buf, MAX_LINE_WIDTH, fp)!=NULL){
        /* If the line is format line. */
        strncpy(testBuf,buf,strlen(buf)+1);
        if(buf[0]=='?'){
            formatToken = strtok(buf, " ");
            switch(buf[1]){
                case 'f':
                    if (buf[2]=='m' && buf[3]=='t'){
                        formatToken = strtok(NULL, " ");
                        if(strcmp(formatToken,"on\n")==0){
                            /* If switching from off to on */
                            format = 1;
                        } else {
                            format = 0;
                        }
                    } else {
                        if (format == 1){
                            int j;
                            for (j = 0; j < margin; j++){
                                printf(" ");
                            }
                        }
                        printf("%s\n",testBuf);
                    }
                    break;
                case 'w':
                    if (buf[2]=='i' && buf[3]=='d' && buf[4]=='t' && buf[5]=='h'){
                        format = 1;
                        formatToken = strtok(NULL, " ");
                        width = atoi(formatToken);
                    } else {
                        if (format == 1){
                            int j;
                            for (j = 0; j < margin; j++){
                                printf(" ");
                            }
                        }
                        printf("%s\n",testBuf);
                    }
                    break;
                case 'm':
                    if (buf[2]=='r' && buf[3] == 'g' && buf[4] == 'n'){
                        formatToken = strtok(NULL, " ");
                        margin = atoi(formatToken);
                        if (width == 0){
                            margin = 0;
                        }
                    } else {
                        if (format == 1){
                            int j;
                            for (j = 0; j < margin; j++){
                                printf(" ");
                            }
                        }
                        printf("%s\n",testBuf);
                    }
                    break;
            }
        } else if(buf[0]=='\n'){
            if (format == 1){
                int j;
                for (j = 0; j < margin; j++){
                    printf(" ");
                }
            }
            printf("%s",tempLine);
            if (format == 1){
                printf("\n");
            }
            printf("\n");
            memset(tempLine, 0, sizeof tempLine);
        } else {
            /* If format is off */
            if (format == 0){
                printf("%s",buf);
            /* If format is on */
            } else {
                token = strtok(buf, " \n");
                while( token != NULL ) {
                    if (strcmp(token," ")){
                        if (strlen(token) + strlen(tempLine) + margin + 1<= width){
                            if (strcmp(tempLine,"")){
                                strcat(tempLine," ");
                            }
                            strcat(tempLine,token);
                        } else {
                            int i;
                            for (i = 0; i < margin; i++){
                                printf(" ");
                            }
                            printf("%s\n", tempLine);
                            strcpy(tempLine, token);
                        }
                    }
                    token = strtok(NULL, " \n");
                }
            }
        }
    }
    if (strlen(tempLine)>0){
        if (format == 1){
            int j;
            for (j = 0; j < margin; j++){
                printf(" ");
            }
        }
        printf("%s\n",tempLine);
    }
    fclose(fp);
    return 0;

}
