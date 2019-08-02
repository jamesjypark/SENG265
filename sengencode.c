/* This file is created for Assignment 4 in SENG 265 Fall 2018
 * Student Name: James (Juyoung) Park
 * Note: The implementation of MTF encode has been given
 * to you and your task is to implement the Run-Length 
 * encoding and decoding algorithms. 
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
 
#define MAX_MSG_SIZE 200
#define MAX_TABLE_SIZE 256

char *table = NULL;

void init_mtf_table(int size)
{
	int i;
	
	/* initialize the table */
	for ( i = 0; i < size; i++) 
    {
        /* Can you figure out why assigning the 
         * values from the back of array? 
         */
		table[size-i-1] = (char) i;
	}
}

int alloc_mtf_table(int size)
{
	if (size <= 0) size = MAX_TABLE_SIZE;
	
	table = (char *) malloc(sizeof(char) * size);
	if (!table) 
    {
		fprintf(stderr, "error alloc!");
		return 0;
	}
	init_mtf_table(size);
	return 1;
}

void free_mtf_table(void)
{
	if (table) 
        free(table);
}

int move_to_front(char *str, char c)
{
    char *p, *q;
    int shift = 0;
    
    /* create a copy of str */
    p = (char *)malloc(strlen(str)+1);
    strncpy(p, str, strlen(str)+1);
    /* returns pointer to location of char c in string str */
    q = strchr(p,c);  shift = q-p;      
    /* number of characters from 0 to the position of c in str */
    shift = q-p;
    /* shift the characters from the second position in str */
    strncpy(str+1, p, shift);
    /* put character c to the first position */
    str[0]=c;
    /* release the memory allocated for p */
    free(p);
    /*  printf("\n%s\n",str); */
    /* return the code number */
    return shift+1;
}
 
void mtf_decode(int *code, int size, char *msg)
{
    int i,index;
    char c;
    /* char table[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; */
    alloc_mtf_table(MAX_TABLE_SIZE);
    for(i=0; i<size; i++)
    {
        c = table[code[i]-1];
        index = move_to_front(table,c);
        if(code[i] != index) 
        {
            printf("there is an error in the decode process");
        }
        msg[i] = c;
    }
    msg[size]='\0';
    free_mtf_table();
}
 
void mtf_encode(char *msg, int size, int *code) 
{
    int i=0;
    char c;
    /* char table[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; */

    alloc_mtf_table(MAX_TABLE_SIZE);

    for(i=0; i<size; i++) 
    {
        c = msg[i];
        code[i] = move_to_front(table, c);
    }

    free_mtf_table();
}



/* This function is implemented to apply run-length decoding. 
 * @param code: the run-length-encoded message.
 * @param size: the size of the protential 'shorter' run-length code.
 * @param msg: the recovered mtf-encoded message.
 * @return: the recovered length of the mtf-encoded message.
 */
int run_length_decode(int *code, int size, int *msg) 
{
    int msgIndex = 0;
    int i = 0;
    int k = 0;
    for (i=0; i < size; i++){
        if (code[i] == 0){
            for (k = 0; k<code[i+1]; k++){
                msg[msgIndex] = 1;
                msgIndex++;
            }
            i++;
        } else {
            msg[msgIndex] = code[i];
            msgIndex++;
        }
    }
    return msgIndex;
}



/* This function is implemented to apply run-length encoding. 
 * @param msg: the original mtf-encoded message.
 * @param size: the size of the mtf-encoded message.
 * @param code: the run_length encoded message.
 * @return: the new length of the run-length-encoded message.
 */
int run_length_encode(int *msg, int size, int *code) 
{
    int countOne = 0;
    int codeIndex = 0;
    int i = 0;
    int z = 0;

    for (i = 0; i < size; i++){
        if (msg[i]==1){
            countOne++;
        } else { 
            /* if not one */
            if (countOne >= 3){ 
                /* if previous sequence has more than three 1's */
                code[codeIndex] = 0;
                codeIndex++;
                code[codeIndex] = countOne;
                codeIndex++;
                countOne = 0;
            } else { 
                /* if previous sequence has less than three 1's */
                for (z = 0; z < countOne; z++){
                    code[codeIndex] = 1;
                    codeIndex++;
                }
                countOne = 0;
            }
            code[codeIndex] = msg[i];
            codeIndex++;
        }
    }
    if (countOne >= 0){
        if (countOne >= 3){ 
            /* if previous sequence has more than three 1's */
            code[codeIndex] = 0;
            codeIndex++;
            code[codeIndex] = countOne;
            codeIndex++;
            countOne = 0;
        } else { 
            /* if previous sequence has less than three 1's */
            for (z = 0; z < countOne; z++){
                code[codeIndex] = 1;
                codeIndex++;
            }
            countOne = 0;
        }
    }
    return codeIndex;
}




/* This function is implemented to check if the run_length 
 * encoded message can be recovered correctly.
 * @param msg: the original mtf encoded message
 * @param size: the size of the protential 'shorter' run_length code
 * @param code: the run_length encoded message.
 * @return: 1 for True, 0 for False.
 */
int check_run_length(int *msg, int size, int *code)
{
    /* Ensures that enough memory is allocated for msg2 */
    int *msg2 = (int *) malloc(sizeof(int) * MAX_MSG_SIZE);
    int i, val = 1;

    run_length_decode(code, size, msg2);

    for (i=0; i < sizeof(msg2) && i < sizeof(msg); i++){
        if (msg[i] != msg2[i]){
            val = 0;
        }
    }

    free(msg2);

    return val;
}

int check_mtf(char *msg, int size, int *code)
{
    char *msg2 = (char *) malloc(sizeof(char) * size);
    int i, val = 1; /* True: 1, False: 0 */

    /* check decode of mtf */
    mtf_decode(code, size, msg2);
    if(strcmp(msg,msg2)!=0) val=0;
 
    free(msg2);
 
    return val;
}

void simple_test(void) 
{
    char msg_array[3][MAX_MSG_SIZE] = {"abcabaaaabc","sengfmtyoushallpass","abbbbbcccccdecccceee"};
    int code[MAX_MSG_SIZE] = {0};

    int i,len,j;
    for(i=0; i<3; i++)
    {
        len = strlen(msg_array[i]);
        mtf_encode(msg_array[i], len, code);
        /* print the original message */
        printf("%s : [", msg_array[i]);
        /* print encoded message */
        for(j=0; j<len; j++)
            printf("%d ", code[j]);
        printf("]\n");

        /* check if the encoded message can be decoded correctly */
        if(check_mtf(msg_array[i], len, code))
            printf("Correct :)\n");
        else
            printf("Incorrect :(\n");
    }
}

void run_length_test(void) 
{
    char test_msg_array[5][MAX_MSG_SIZE] = {
        "abcabcaaaaaab",
        "SeNG_enCoDE_2018 You are on it!",
        "abbbbbcccccdecccceee",
        "\tabc\nhellOWorld\r\n",
        "?sengfmt not-a-command\nI know you can climb the highest mountains.\nNever lose faith in yourself."
        };
    int test_msg_size[5] = {13, 31, 20, 17, 96};
    int test_rl_code_array[5][MAX_MSG_SIZE] = {
        {159, 159, 159, 3, 3, 3, 3, 0, 5, 3},
        {173, 156, 178, 185, 164, 4,151, 189, 152, 189, 189, 7, 206, 208, 208, 203, 224, 178, 10, 155, 4, 172, 159, 16, 4, 6, 16, 3, 166, 159, 224},
        {159, 159, 0, 4, 159, 0, 4, 159, 159, 3, 0, 3, 2, 1, 1},
        {247, 160, 160, 160, 247, 157, 160, 155, 1, 179, 172, 155, 153, 5, 163, 245, 10},
        {193, 142, 156, 148, 155, 156, 151, 147, 224, 6, 152, 4, 212, 162, 2, 161, 5, 8, 1, 5, 7, 161, 246, 187, 11, 160, 6, 9, 154, 5, 153, 4, 156, 4, 13, 12, 8, 4, 4, 160, 162, 15, 164, 6, 18, 164, 22, 4, 3, 7, 22, 3, 5, 23, 7, 7, 9, 15, 15, 14, 6, 15, 12, 4, 9, 213, 21, 184, 13, 162, 2, 163, 15, 19, 15, 10, 6, 5, 27, 14, 14, 15, 18, 6, 4, 16, 3, 22, 11, 18, 14, 13, 13, 14, 14, 18}
    };
    int test_rl_size[5] = {10, 31, 15, 17, 96};

    int mtf_code[MAX_MSG_SIZE] = {0};
    int rl_code[MAX_MSG_SIZE] = {0};

    int i,len,j;
    for(i=0; i<5; i++)
    {
        printf("==========Test %d==========\n", i+1);
        len = strlen(test_msg_array[i]);
        mtf_encode(test_msg_array[i], len, mtf_code);
        /* print the original message */
        printf("The orginal message is :\n%s\nmtf-encoded message is:\n[ ", test_msg_array[i]);
        /* print mtf-encoded message */
        for(j=0; j<len; j++)
            printf("%d ", mtf_code[j]);
        printf("]\n");
        /* print mtf-encoded message */
        int new_len = run_length_encode(mtf_code, len, rl_code);
        if (new_len == test_rl_size[i]) 
        {   
            int pass_encode = 1;
            for (j=0; j<new_len; j++)
                if (rl_code[j] != test_rl_code_array[i][j])
                {
                    printf("Test %d Failed in run_length_encode.\n", i+1);
                    printf("Expected run-length encode result: [");
                    int k;
                    for (k=0; k<test_rl_size[i]-1; k++)
                        printf("%d, ", test_rl_code_array[i][k]);
                    printf("%d]", test_rl_code_array[i][k]);
                    pass_encode = 0;
                    break;
                }
            if (pass_encode)
                printf("Test %d Succeed in run_length_encode.\n", i+1);
        }
        else 
        {
            printf("Test %d FAILED in run_length_encode.\n", i+1);
            printf("Expected new length: %d.\n", test_rl_size[i]);
            continue;
        }
        /* print run_length encoded message */
        /* for(j=0; j<new_len; j++)
            printf("%d ", rl_code[j]);
        printf("\n\n");
        */
        /* check if the run_length encoded message can be decoded correctly */
        if (check_run_length(mtf_code, new_len, rl_code))
            printf("Test %d Succeed in run_length_decode.\n",i+1);
        else   
            printf("Test %d FAILED in run_length_decode.\n", i+1);
        
        /* Now it's time to recover the original message */
        run_length_decode(rl_code, new_len, mtf_code); 

        if(check_mtf(test_msg_array[i], len, mtf_code))
            printf("Test %d Succeed in recovering the original message.\n", i+1);
        else
            printf("Test %d FAILED in recovering the original message.\n", i+1);
    }
}

int main(int argc, char *argv[])
{
    /*simple_test();*/
    /* uncomment the following line before you submit */
    run_length_test();

    return 0;
}
