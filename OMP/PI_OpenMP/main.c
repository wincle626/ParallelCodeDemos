/*
 * main.c
 *
 *  Created on: 27 Jan 2022
 *      Author: UDRC
 */

#include <sequential.h>
#include <parallel.h>
#include "datastructparallel.h"
#include "ompforparallel.h"
#include "criticalparallel.h"
#include "atomicparallel.h"
#include "padparallel.h"
#include "scheduleparallel.h"
#include <reductionparallel.h>

int main(int argc, char** argv){

	sequential();
	printf("\n");
	parallel();
	printf("\n");
	datastructparallel();
	printf("\n");
	padparallel();
	printf("\n");
	ompforparallel();
	printf("\n");
	criticalparallel();
	printf("\n");
	atomicparallel();
	printf("\n");
	scheduleparallel();
	printf("\n");
	reductionparallel();

	return 0;
}



