/*
 * execveLines.c
 *
 *  Created on: 2 May 2019
 *      Author: harel
 */
#include "execveLines.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX 9
#define MAXSIZE 81
#define CONSTWRONGORTRUE 1

int main() {
	int bucket[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	char buffer[MAX][MAX];
	char true[1] = { "1" }, false[1] = { "0" };

	read(STDIN_FILENO, buffer[0], MAXSIZE);
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) {
			// each number insert in the position -1 if the number dont exist enter 1 if exist finish
			if (bucket[buffer[i][j] - '0' - 1] == 0) {
				bucket[buffer[i][j] - '0' - 1] = 1;
			} else {
				write(STDOUT_FILENO, false, CONSTWRONGORTRUE);
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				return 1;
			}
		}
		//to zero the bucket
		for (int k = 0; k < MAX; k++)
			bucket[k] = 0;
	}
	//Success write to pipe 1
	write(STDOUT_FILENO, true, CONSTWRONGORTRUE);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	return 1;
}
