#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
using namespace std;

typedef struct Node* List;

struct Node {
		unsigned long long int key;
		unsigned long long int value;
		int ListSize;
		int Level;
		vector<Node*> forward;
};



List find(List list, unsigned long long int k,unsigned long long int v, int level, FILE* fo) {
		if(level < 0) return NULL;
		
		if(list->forward[level]->key > k) {
				find(list, k, v, --level, fo);
		}
		else if(list->forward[level]->key < k) {
				find(list->forward[level], k, v, level, fo);
		}
		else  {
				fprintf(fo,"Found (%llu, %llu) ",k,list->forward[level]->value);
				if(v != -1) list->forward[level]->value = v;
				return list;
		}
}

List insert(List list, int level, Node* node, FILE* fo) {
		if(level < 0) {
					list->ListSize += 1;
					fprintf(fo,"Inserted (%llu, %llu)\n",node->key, node->value);
					node->ListSize = list->ListSize;
			}

		else {
			if(find(list, node->key, node->value, level, fo) != NULL) 
				fprintf(fo,"update v=%llu\n",node->value);
	
			else {
				if(list->forward[level]->key<node->key) {
						insert(list->forward[level],level, node, fo);
				}
				
				else if(list->forward[level]->key>node->key) {
						node->forward[level] = list->forward[level];
						list->forward[level] = node;
						insert(list, --level,node,fo);
					}
				}
			}
			return list;
			
}

int print(List list, FILE* fo) {
		if(list->key == 18446744073709551615) return 0;
		if(list->key != 0) 
			fprintf(fo,"(%llu, %llu) with level=%lu ",list->key, list->value, list->Level);
		print(list->forward[0],fo);
		return 0;
}

int main(int argc, char* argv[]) {
		FILE* fi = fopen(argv[1], "rb");
		FILE* fo = fopen("output.txt","wb");

		char i;
		unsigned long long int k;
		List SkipList = (List)malloc(sizeof(struct Node));
		SkipList->key = 0;
		SkipList->value = -1;
		SkipList->ListSize = 0;
		SkipList->Level = 0;	
		Node* end = (Node*)malloc(sizeof(struct Node));
		end->key = 18446744073709551615;
		end->value = -1;
		end->ListSize = 0;
		end->Level = 0;
		end->forward.push_back(NULL);
		SkipList->forward.push_back(end);	

						
		while(!feof(fi)) {
				fscanf(fi, "%c", &i);

				if(i == 'I') {
						unsigned long long int v;
						fscanf(fi, "%llu %llu", &k, &v);
						
						Node* n = (Node*)malloc(sizeof(struct Node));
						n->key = k;
						n->value = v;
						n->ListSize = SkipList->ListSize;
						srand(time(NULL));
						n->Level = rand() % (SkipList->ListSize+2);
						if(n->Level >= SkipList->Level) {
								SkipList->forward.resize(n->Level+2,end);
								SkipList->forward[n->Level+1] = SkipList->forward[SkipList->Level];
								SkipList->Level = n->Level+1;
								

						}
						
						n->forward.resize(n->Level+1,NULL);
						insert(SkipList, SkipList->Level-1,n,fo);
						if(SkipList->ListSize < n->ListSize) 
								SkipList->ListSize = n->ListSize;
				}

				else if(i == 'P') {
						print(SkipList, fo);
						fprintf(fo, "\n");
				}

				else if(i == 'F') {
						fscanf(fi,"%llu", &k);
						if(find(SkipList, k, -1, SkipList->Level,fo) == NULL) fprintf(fo, "Not Found\n");
						else fprintf(fo, "\n");
				}

				else if(i == 'Q') {
						fclose(fi);
						fclose(fo);
						return 0;
				}
		}
}
