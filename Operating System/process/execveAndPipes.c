/*
 * execveAndPipes.c
 *
 *  Created on: 12 Mar 2019
 *      Author: harel
 */

#include "execveAndPipes.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX 9
#define MAXSIZE 81
#define  NUMBERPROCESS 3

int main(int argc, char *argv[]) {
	const char trueLegal[NUMBERPROCESS + 1] = { '1', '1', '1', '\0' };
	char *filename;
	char *argv2[] = { NULL };
	char buffer[MAX][MAX];
	char bufferOfCheckSudoku[NUMBERPROCESS + 1];
	int check, flag = 1;
	int pfdAnswer[2], pfdRead1[2], pfdRead2[2], pfdRead3[2];
	filename = (char*) malloc(sizeof(char) * MAXSIZE);
	for (int num = 1; num < argc || flag == 1; num++) {
		flag = 0;
		if (argc == 1) {
			printf("please enter name of file to check\n");
			scanf("%s", filename);
		}
		//read form file the sudoku
		else {
			sscanf(argv[num], "%s", filename);
		}
		FILE *fP = fopen(filename, "rb");
		for (int i = 0; i < MAX; i++) {
			for (int j = 0; j < MAX; j++) {
				fread(&buffer[i][j], sizeof(char), 1, fP);
				fseek(fP, 1, SEEK_CUR);
			}
		}
		//create two FD one for read the sudoku and the second for return answer if sudoku is legal
		if (pipe(pfdAnswer) == -1) {
			printf("error pipe\n");
			return -1;
		}
		if (pipe(pfdRead1) == -1) {
			printf("error pipe\n");
			return -1;
		}

		switch (fork()) {
		case -1:
			printf("error");
			break;

		case 0:
			//child check the row
			close(pfdAnswer[0]);
			close(pfdRead1[1]);
			// copy  the  FD to STDIN and STDOUT
			dup2(pfdAnswer[1], STDOUT_FILENO);
			dup2(pfdRead1[0], STDIN_FILENO);
			execv("./execveLines", argv2);
			break;
		default:
			// father write the sudoku to pipe
			check = write(pfdRead1[1], buffer[0], MAXSIZE);
			if (check != MAXSIZE) {
				printf("error of write to pipe");
				return -1;
			}
			//close the write and the read of father
			close(pfdRead1[0]);
			close(pfdRead1[1]);
			break;
		}

		//--------------now began to check the MATRIX 3*3---------------------------
		if (pipe(pfdRead2) == -1) {
			printf("error pipe\n");
			return -1;
		}
		switch (fork()) {
		case -1:
			printf("error\n");
			break;
		case 0:
			//child check the Matrix
			close(pfdAnswer[0]);
			close(pfdRead2[1]);
			// copy  the  FD to STDIN and STDOUT
			dup2(pfdRead2[0], STDIN_FILENO);
			dup2(pfdAnswer[1], STDOUT_FILENO);
			execv("./execveMatrix", argv2);
			break;
		default:
			// father write the sudoku to pipe
			check = write(pfdRead2[1], buffer[0], MAXSIZE);
			if (check != MAXSIZE) {
				printf("error of write to pipe");
				return -1;
			}
			//close the write and the read of father
			close(pfdRead2[0]);
			close(pfdRead2[1]);
			break;
		}

		//--------------now began to check the column---------------------------
		if (pipe(pfdRead3) == -1) {
			printf("error pipe\n");
			return -1;
		}
		switch (fork()) {
		case -1:
			printf("error\n");
			break;
		case 0:
			//child check the Matrix
			close(pfdAnswer[0]);
			close(pfdRead3[1]);
			// copy  the  FD to STDIN and STDOUT
			dup2(pfdAnswer[1], STDOUT_FILENO);
			dup2(pfdRead3[0], STDIN_FILENO);
			execv("./execvColumn", argv2);
			break;
		default:
			//write the sudoku to pipe
			check = write(pfdRead3[1], buffer[0], MAXSIZE);
			if (check != MAXSIZE) {
				printf("error of write to pipe\n");
				return -1;
			}
			//close the write and the read of father
			close(pfdRead3[0]);
			close(pfdRead3[1]);
			break;
		}

		for (int i = 0; i < NUMBERPROCESS; i++) {
			wait(NULL);
		}
		read(pfdAnswer[0], bufferOfCheckSudoku, NUMBERPROCESS);
		close(pfdAnswer[0]);
		close(pfdAnswer[1]);
		bufferOfCheckSudoku[NUMBERPROCESS] = '\0';
		if (strcmp(bufferOfCheckSudoku, trueLegal) == 0) {
			printf("FILENAME is legal\n");
		} else {
			printf("FILENAME is not legal\n");
		}
	}
	return 1;
}

