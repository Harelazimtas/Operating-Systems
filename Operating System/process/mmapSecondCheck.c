/*
 * mmapSecondSheck.c
 *
 *  Created on: 7 May 2019
 *      Author: harel
 */

#include "mmapSecondCheck.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
	char *filename;
	int flag = 1;
	shared *sharedMemory;
	filename = (char*) malloc(sizeof(char) * MAXSIZE);
	sharedMemory = (shared*) mmap(NULL, sizeof(sharedMemory),
	PROT_WRITE | PROT_READ,
	MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (sharedMemory == MAP_FAILED) {
		printf("error mmap");
		return 1;
	}
	//loop of  file check
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
				fscanf(fP, "%d", &sharedMemory->solution[i][j]);
			}
		}
		//-----------------------child row check------------------------
		if (fork() == 0) {
			int flag1 = 1;
			int bucket1[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			for (int i = 0; i < MAX; i++) {
				for (int j = 0; j < MAX; j++) {
					if (bucket1[sharedMemory->solution[i][j] - 1] == 0) {
						bucket1[sharedMemory->solution[i][j] - 1] = 1;
					} else {
						sharedMemory->status[0] = 0;
						flag1 = 0;
					}
				}
				for (int j = 0; j < MAX; j++)
					bucket1[j] = 0;
			}
			if (flag1 == 1)
				sharedMemory->status[0] = 1;
			exit(1);
		}
		//------------------------child column check-------------------
		if (fork() == 0) {
			int flag1 = 1;
			int bucket2[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			for (int i = 0; i < MAX; i++) {
				for (int j = 0; j < MAX; j++) {
					if (bucket2[sharedMemory->solution[j][i] - 1] == 0) {
						bucket2[sharedMemory->solution[j][i] - 1] = 1;
					} else {
						sharedMemory->status[1] = 0;
						flag1 = 0;
					}
				}
				for (int j = 0; j < MAX; j++)
					bucket2[j] = 0;
			}
			if (flag1 == 1)
				sharedMemory->status[1] = 1;
			exit(1);
		}
		//---------------------------------child matrix check------------------
		if (fork() == 0) {
			int flag1 = 1;
			int bucket3[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			for (int i = 0; i < MAX; i++) {
				for (int j = 0; j < SIZEMATRIX; j++) {
					for (int k = 0; k < SIZEMATRIX; k++) {
						if (bucket3[sharedMemory->solution[j
								+ ((i / SIZEMATRIX) * SIZEMATRIX)][k
								+ ((i % SIZEMATRIX) * SIZEMATRIX)] - 1] == 0) {
							bucket3[sharedMemory->solution[j
									+ ((i / SIZEMATRIX) * SIZEMATRIX)][k
									+ ((i % SIZEMATRIX) * SIZEMATRIX)] - 1] = 1;
						} else {
							sharedMemory->status[2] = 0;
							flag1 = 0;
						}
					}
				}
				for (int j = 0; j < MAX; j++)
					bucket3[j] = 0;
			}
			if (flag1 == 1)
				sharedMemory->status[2] = 1;
			exit(1);

		}
		int count = 0;
		for (int i = 0; i < NUMBERPROCESS; i++)
			wait(NULL);
		//get status of check
		for (int i = 0; i < NUMBERPROCESS; i++) {
			if (sharedMemory->status[i] == 1)
				count++;
		}
		if (count == 3)
			printf("FILENAME is legal\n");
		else
			printf("FILENAME is not legal\n");

	}
	return 1;
}
