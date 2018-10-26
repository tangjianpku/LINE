//  Copyright 2013 Google Inc. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

//  The input is biterm_file. The program will run word2vec on the word net.
//  Multi-threads are supported in this version.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;

#define MAX_STRING 200
#define MAX_LABEL 1000

struct Entry
{
	int label;
	double value;
	friend bool operator < (Entry e1, Entry e2)
	{
		return e1.value > e2.value;
	}
};

int label_size = 47, ntests = 0;
int pst2lb[MAX_LABEL];
Entry ranked_list[MAX_LABEL];
char candidate_file[MAX_STRING], predict_file[MAX_STRING];
vector< set<int> > can;

void TrainModel()
{
	set<int> curset;
	int len, lb, tmp;
	char str[MAX_STRING];
	double prob;

	FILE *fi = fopen(candidate_file, "rb");
	while (fscanf(fi, "%d", &len) == 1)
	{
		for (int k = 0; k != len; k++)
		{
			fscanf(fi, "%d", &lb);
			curset.insert(lb);
		}
		can.push_back(curset);
		curset.clear();
		ntests++;
	}
	fclose(fi);

	double macro_f1, micro_f1;
	double tp, fn, fp;
	double stp = 0, sfn = 0, sfp = 0;
	double P, R;
	double sP = 0, sR = 0;

	fi = fopen(predict_file, "rb");
	fscanf(fi, "%s", str);
	for (int k = 0; k != label_size; k++)
	{
		fscanf(fi, "%d", &lb);
		pst2lb[k] = lb;
	}
	for (int k = 0; k != ntests; k++)
	{
		fscanf(fi, "%d", &tmp);
		for (int i = 0; i != label_size; i++)
		{
			fscanf(fi, "%lf", &prob);
			lb = pst2lb[i];
			ranked_list[i].label = lb;
			ranked_list[i].value = prob;
		}
		sort(ranked_list, ranked_list + label_size);
		int n = can[k].size();
		tp = 0;
		for (int i = 0; i != n; i++)
		{
			lb = ranked_list[i].label;
			if (can[k].count(lb) != 0)
				tp++;
		}
		fn = n - tp;
		fp = n - tp;

		stp += tp;
		sfn += fn;
		sfp += fp;

		P = tp / (tp + fp);
		R = tp / (tp + fn);
		
		sP += P;
		sR += R;
	}

	P = sP / ntests;
	R = sR / ntests;
	macro_f1 = 2 * P * R / (P + R);

	P = stp / (stp + sfp);
	R = stp / (stp + sfn);
	micro_f1 = 2 * P * R / (P + R);

	printf("number of tests: %d\n", ntests);
	printf("macro-f1: %lf\n", macro_f1);
	printf("micro-f1: %lf\n", micro_f1);
}

int ArgPos(char *str, int argc, char **argv) {
	int a;
	for (a = 1; a < argc; a++) if (!strcmp(str, argv[a])) {
		if (a == argc - 1) {
			printf("Argument missing for %s\n", str);
			exit(1);
		}
		return a;
	}
	return -1;
}

int main(int argc, char **argv) {
	int i;
	if (argc == 1) {
		printf("WORD VECTOR estimation toolkit v 0.1b\n\n");
		printf("Options:\n");
		printf("Parameters for training:\n");
		printf("\t-train <file>\n");
		printf("\t\tUse text data from <file> to train the model\n");
		printf("\t-test <file>\n");
		printf("\t\tUse text data from <file> to test the model\n");
		printf("\t-vector <file>\n");
		printf("\t\tUse vector data from <file>\n");
		printf("\nExamples:\n");
		printf("./evl -train train.txt -test test.txt -vector vec.txt \n\n");
		return 0;
	}
	if ((i = ArgPos((char *)"-predict", argc, argv)) > 0) strcpy(predict_file, argv[i + 1]);
	if ((i = ArgPos((char *)"-candidate", argc, argv)) > 0) strcpy(candidate_file, argv[i + 1]);
	TrainModel();
	return 0;
}