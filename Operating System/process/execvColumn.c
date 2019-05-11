/*
 * execvColumn.c
 *
 *  Created on: 3 May 2019
 *      Author: harel
 */

#include "execvColumn.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 9
#define MAXSIZE 81
#define CONSTWRONGORTRUE 1

int main() {
	int bucket[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	char buffer[MAX][MAX];
	const char true[1] = { "1" }, false[1] = { "0" };
	read(STDIN_FILENO, buffer[0], MAXSIZE);
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) {
			// each number insert in the position -1 if the number dont exist enter 1 if exist finish
			if (bucket[buffer[j][i] - '0' - 1] == 0) {
				bucket[buffer[j][i] - '0' - 1] = 1;
			} else {
				//if sudoku is wrong write to  pipe 0
				write(STDOUT_FILENO, false, CONSTWRONGORTRUE);
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				return 1;
			}
		}
		//to zero the array
		for (int k = 0; k < MAX; k++)
			bucket[k] = 0;
	}
	//Success write to pipe 1
	write(STDOUT_FILENO, true, CONSTWRONGORTRUE);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	return 1;
}
