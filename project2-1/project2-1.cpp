#include <stdio.h>
#include <cstring>
#include <stdlib.h>
int col=0;
typedef struct Node {
		unsigned int key;
		unsigned int value;
		bool status = false;
}Node;

typedef struct HashTable {
		int MaxTableSize;
		Node* Table;
}Hash;

int find(unsigned int k, Hash HashTable) {
		if(HashTable.Table[k%HashTable.MaxTableSize].key == k) return k%HashTable.MaxTableSize;
		return -1;
}

void findhash(unsigned int k, Hash HashTable,FILE* fo) {
		int i = find(k,HashTable);
		if (i != -1) fprintf(fo, "%d\n", HashTable.Table[i].value);
		else fprintf(fo, "%s\n","Not Found");
}

void insert(unsigned int k, unsigned int v,Hash HashTable,FILE* fo) {
		int i = find(k,HashTable);
		if(i == -1) {
			HashTable.Table[k%HashTable.MaxTableSize].key = k;
			HashTable.Table[k%HashTable.MaxTableSize].value = v;
			HashTable.Table[k%HashTable.MaxTableSize].status = true;

			fprintf(fo, "%s\n", "Inserted");
		}
		else {
				fprintf(fo, "%s\n", "Duplicated");
				col++;
				HashTable.Table[i].value = v;
		}
}

void deletehash(unsigned int k, Hash HashTable,FILE* fo) {
		int i = find(k,HashTable);
		if(i == -1) fprintf(fo,"%s\n","Not Found");
		else {
				HashTable.Table[i].key = NULL;
				HashTable.Table[i].value = NULL;
				HashTable.Table[i].status = false;

				fprintf(fo,"%s\n","Deleted");

		}
}

void printTable(Hash HashTable,FILE* fo) {
		fprintf(fo,"%s","keys : ");
		for(int i=0;i<HashTable.MaxTableSize;i++) {
				if(HashTable.Table[i].status == true) {
						fprintf(fo,"%d ",HashTable.Table[i].key);
				}
		}
		fprintf(fo,"%s","\n");
		fprintf(fo,"%s","collision : ");
		fprintf(fo,"%d\n",col);
}

int main(int argc, char* argv[]) {
		FILE *fi = fopen(argv[1],"r");
		
		unsigned int n, k;
		char i;

		fscanf(fi, "%d", &n);
		
		Hash HashTable = {};
		HashTable.MaxTableSize = 3*n;
		HashTable.Table = (Node*)malloc(sizeof(Node)*3*n);
		
		
		FILE* fo = fopen("output.txt","wb");

		while(!feof(fi)) {
				fscanf(fi, "%c", &i);
				if(i == 'I') {
						unsigned int v;
						fscanf(fi,"%u %u",&k, &v);

						insert(k,v,HashTable,fo);
				}
				else if(i == 'D') {
						fscanf(fi,"%u",&k);

						deletehash(k,HashTable,fo);
				}
				else if(i == 'F') {
						fscanf(fi,"%u",&k);
						
						findhash(k,HashTable,fo);
				}
				else if(i == 'P') printTable(HashTable,fo);

				else if(i == 'Q') break;
		}

}
