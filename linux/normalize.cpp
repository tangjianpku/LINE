#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

#define MAX_STRING 100

typedef float real;                    // Precision of float numbers

char input_file[MAX_STRING], output_file[MAX_STRING];
int binary = 0;

void Normalize()
{
	long long num_vertices, vector_dim, a, b;
	char name[MAX_STRING], ch;
	real *vec;
	double len;

	FILE *fi, *fo;

	fi = fopen(input_file, "rb");
	fo = fopen(output_file, "wb");

	fscanf(fi, "%lld %lld", &num_vertices, &vector_dim);
	vec = (real *)malloc(vector_dim * sizeof(real));
	fprintf(fo, "%lld %lld\n", num_vertices, vector_dim);
	for (a = 0; a < num_vertices; a++)
	{
		fscanf(fi, "%s%c", name, &ch);
		for (b = 0; b < vector_dim; b++) fread(&vec[b], sizeof(real), 1, fi);
		len = 0;
		for (b = 0; b < vector_dim; b++) len += vec[b] * vec[b];
		len = sqrt(len);
		for (b = 0; b < vector_dim; b++) vec[b] /= len;

		fprintf(fo, "%s ", name);
		if (binary)
		{
			for (b = 0; b < vector_dim; b++)
				fwrite(&vec[b], sizeof(real), 1, fo);
		}
		else
		{
			for (b = 0; b < vector_dim; b++)
				fprintf(fo, "%lf ", vec[b]);
		}
		fprintf(fo, "\n");
	}
	free(vec);
	fclose(fi);
	fclose(fo);
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
		printf("Normalize vertex embeddings by setting their L2 norm as 1\n\n");
		printf("Options:\n");
		printf("Parameters for training:\n");
		printf("\t-input <file>\n");
		printf("\t\tThe original vertex embeddings\n");
		printf("\t-output <file>\n");
		printf("\t\tUse <file> to save the normalized vertex embeddings\n");
		printf("\t-binary <int>\n");
		printf("\t\tSave the learnt embeddings in binary moded; default is 0 (off)\n");
		printf("\nExamples:\n");
		printf("./normalize -input vec_wo_norm.txt -output vec_norm.txt -binary 1\n\n");
		return 0;
	}
	if ((i = ArgPos((char *)"-input", argc, argv)) > 0) strcpy(input_file, argv[i + 1]);
	if ((i = ArgPos((char *)"-output", argc, argv)) > 0) strcpy(output_file, argv[i + 1]);
	if ((i = ArgPos((char *)"-binary", argc, argv)) > 0) binary = atoi(argv[i + 1]);
	Normalize();
	return 0;
}