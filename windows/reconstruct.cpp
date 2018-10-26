#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>

#define MAX_STRING 100

const int hash_table_size = 30000000;

typedef float real;                    // Precision of float numbers

struct ClassVertex
{
	double degree, sum_weight;
	char *name;
};

struct Neighbor
{
	int vid;
	double weight;
	friend bool operator < (Neighbor n1, Neighbor n2)
	{
		return n1.weight > n2.weight;
	}
};


char train_file[MAX_STRING], output_file[MAX_STRING];
struct ClassVertex *vertex;
int *vertex_hash_table;
int max_num_vertices = 1000, num_vertices = 0;
long long num_edges = 0;

int max_depth = 1, max_k = 0;
std::vector<int> vertex_set;
std::vector<Neighbor> *neighbor;

Neighbor *rank_list;
std::map<int, double> vid2weight;

/* Build a hash table, mapping each vertex name to a unique vertex id */
unsigned int Hash(char *key)
{
	unsigned int seed = 131;
	unsigned int hash = 0;
	while (*key)
	{
		hash = hash * seed + (*key++);
	}
	return hash % hash_table_size;
}

void InitHashTable()
{
	vertex_hash_table = (int *)malloc(hash_table_size * sizeof(int));
	for (int k = 0; k != hash_table_size; k++) vertex_hash_table[k] = -1;
}

void InsertHashTable(char *key, int value)
{
	int addr = Hash(key);
	while (vertex_hash_table[addr] != -1) addr = (addr + 1) % hash_table_size;
	vertex_hash_table[addr] = value;
}

int SearchHashTable(char *key)
{
	int addr = Hash(key);
	while (1)
	{
		if (vertex_hash_table[addr] == -1) return -1;
		if (!strcmp(key, vertex[vertex_hash_table[addr]].name)) return vertex_hash_table[addr];
		addr = (addr + 1) % hash_table_size;
	}
	return -1;
}

/* Add a vertex to the vertex set */
int AddVertex(char *name)
{
	int length = strlen(name) + 1;
	if (length > MAX_STRING) length = MAX_STRING;
	vertex[num_vertices].name = (char *)calloc(length, sizeof(char));
	strcpy(vertex[num_vertices].name, name);
	vertex[num_vertices].sum_weight = 0;
	num_vertices++;
	if (num_vertices + 2 >= max_num_vertices)
	{
		max_num_vertices += 1000;
		vertex = (struct ClassVertex *)realloc(vertex, max_num_vertices * sizeof(struct ClassVertex));
	}
	InsertHashTable(name, num_vertices - 1);
	return num_vertices - 1;
}

/* Read network from the training file */
void ReadData()
{
	FILE *fin;
	char name_v1[MAX_STRING], name_v2[MAX_STRING], str[2 * MAX_STRING + 10000];
	int vid, u, v;
	double weight;
	Neighbor nb;

	fin = fopen(train_file, "rb");
	if (fin == NULL)
	{
		printf("ERROR: network file not found!\n");
		exit(1);
	}
	num_edges = 0;
	while (fgets(str, sizeof(str), fin)) num_edges++;
	fclose(fin);
	printf("Number of edges: %lld          \n", num_edges);

	fin = fopen(train_file, "rb");
	num_vertices = 0;
	for (int k = 0; k != num_edges; k++)
	{
		fscanf(fin, "%s %s %lf", name_v1, name_v2, &weight);

		if (k % 10000 == 0)
		{
			printf("Reading edges: %.3lf%%%c", k / (double)(num_edges + 1) * 100, 13);
			fflush(stdout);
		}

		vid = SearchHashTable(name_v1);
		if (vid == -1) vid = AddVertex(name_v1);
		vertex[vid].degree += weight;

		vid = SearchHashTable(name_v2);
		if (vid == -1) vid = AddVertex(name_v2);
		vertex[vid].degree += weight;
	}
	fclose(fin);
	printf("Number of vertices: %lld          \n", num_vertices);

	neighbor = new std::vector<Neighbor>[num_vertices];
	rank_list = (Neighbor *)calloc(num_vertices, sizeof(Neighbor));

	fin = fopen(train_file, "rb");
	for (long long k = 0; k != num_edges; k++)
	{
		fscanf(fin, "%s %s %lf", name_v1, name_v2, &weight);

		if (k % 10000 == 0)
		{
			printf("Reading neighbors: %.3lf%%%c", k / (double)(num_edges + 1) * 100, 13);
			fflush(stdout);
		}

		u = SearchHashTable(name_v1);

		v = SearchHashTable(name_v2);

		nb.vid = v;
		nb.weight = weight;
		neighbor[u].push_back(nb);
	}
	fclose(fin);
	printf("\n");

	for (int k = 0; k != num_vertices; k++)
	{
		vertex[k].sum_weight = 0;
		int len = neighbor[k].size();
		for (int i = 0; i != len; i++)
			vertex[k].sum_weight += neighbor[k][i].weight;
	}
}

void Reconstruct()
{
	FILE *fo = fopen(output_file, "wb");

	int sv, cv, cd, len, pst;
	long long num_edges_renet = 0;
	double cw, sum;
	std::queue<int> node, depth;
	std::queue<double> weight;

	for (sv = 0; sv != num_vertices; sv++)
	{
		if (sv % 10 == 0)
		{
			printf("%cProgress: %.3lf%%", 13, (real)sv / (real)(num_vertices + 1) * 100);
			fflush(stdout);
		}

		while (!node.empty()) node.pop();
		while (!depth.empty()) depth.pop();
		while (!weight.empty()) weight.pop();
		vid2weight.clear();

		for (int i = 0; i != num_vertices; i++)
		{
			rank_list[i].vid = i;
			rank_list[i].weight = 0;
		}

		len = neighbor[sv].size();
		if (len > max_k)
		{
			for (int i = 0; i != len; i++)
				fprintf(fo, "%s\t%s\t%lf\n", vertex[sv].name, vertex[neighbor[sv][i].vid].name, neighbor[sv][i].weight);
			num_edges_renet += len;
			continue;
		}

		vid2weight[sv] += vertex[sv].degree / 10.0; // Set weights for self-links here!

		len = neighbor[sv].size();
		sum = vertex[sv].sum_weight;

		node.push(sv);
		depth.push(0);
		weight.push(sum);

		while (!node.empty())
		{
			cv = node.front();
			cd = depth.front();
			cw = weight.front();

			node.pop();
			depth.pop();
			weight.pop();

			if (cd != 0) vid2weight[cv] += cw;

			if (cd < max_depth)
			{
				len = neighbor[cv].size();
				sum = vertex[cv].sum_weight;

				for (int i = 0; i != len; i++)
				{
					node.push(neighbor[cv][i].vid);
					depth.push(cd + 1);
					weight.push(cw * neighbor[cv][i].weight / sum);
				}
			}
		}

		pst = 0;
		std::map<int, double>::iterator iter;
		for (iter = vid2weight.begin(); iter != vid2weight.end(); iter++)
		{
			rank_list[pst].vid = (iter->first);
			rank_list[pst].weight = (iter->second);
			pst++;
		}
		std::sort(rank_list, rank_list + pst);

		for (int i = 0; i != max_k; i++)
		{
			if (i == pst) break;
			fprintf(fo, "%s\t%s\t%.6lf\n", vertex[sv].name, vertex[rank_list[i].vid].name, rank_list[i].weight);
			num_edges_renet++;
		}
	}
	printf("\n");
	fclose(fo);

	printf("Number of edges in reconstructed network: %lld\n", num_edges_renet);
	return;
}

void TrainLINE()
{
	InitHashTable();
	ReadData();
	Reconstruct();
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
		printf("Reconstruct the network by using a Breadth-First-Search strategy\n\n");
		printf("Options:\n");
		printf("Parameters for training:\n");
		printf("\t-train <file>\n");
		printf("\t\tReconstruct the network from <file>\n");
		printf("\t-output <file>\n");
		printf("\t\tUse <file> to save the reconstructed network\n");
		printf("\t-depth <int>\n");
		printf("\t\tThe maximum depth in the Breadth-First-Search; default is 0\n");
		printf("\t-threshold <int>\n");
		printf("\t\tFor vertex whose degree is less than <int>, we will expand its neighbors until the degree reaches <iny>\n");
		printf("\nExamples:\n");
		printf("./reconstruct -train net.txt -output net_dense.txt -depth 2 -threshold 1000\n\n");
		return 0;
	}
	if ((i = ArgPos((char *)"-train", argc, argv)) > 0) strcpy(train_file, argv[i + 1]);
	if ((i = ArgPos((char *)"-output", argc, argv)) > 0) strcpy(output_file, argv[i + 1]);
	if ((i = ArgPos((char *)"-depth", argc, argv)) > 0) max_depth = atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-threshold", argc, argv)) > 0) max_k = atoi(argv[i + 1]);
	vertex = (struct ClassVertex *)calloc(max_num_vertices, sizeof(struct ClassVertex));
	TrainLINE();
	return 0;
}