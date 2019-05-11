/*
 * mmapSecondSheck.h
 *
 *  Created on: 7 May 2019
 *      Author: harel
 */

#ifndef MMAPSECONDCHECK_H_
#define MMAPSECONDCHECK_H_

#define MAX 9
#define MAXSIZE 81
#define  NUMBERPROCESS 3
#define SIZEMATRIX 3

typedef struct {
	int solution[MAX][MAX];
	int status[NUMBERPROCESS];
}shared;

#endif /* MMAPSECONDCHECK_H_ */
