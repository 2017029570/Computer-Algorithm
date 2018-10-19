#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int col = 0;
typedef struct Node {
		unsigned int key;
		unsigned int value;
		bool status = false;
}Node;

typedef struct HashTable {
		int size;
		int TableSize;
		Node* Table;
}Hash;
		

void rehash(Hash hTable) {
		Node* oldTable = hTable.Table;

		hTable.TableSize *= 2;
		free(hTable.Table);
		hTable.Table = (Node*)malloc(sizeof(Node)*hTable.TableSize);
		for(int i=0;i<hTable.TableSize;i++) {
				if(oldTable[i].status == true) {
						hTable.Table[i].key = oldTable[i].key;
						hTable.Table[i].value = oldTable[i].value;
						hTable.Table[i].status = oldTable[i].status;
				}
		}
}

void insert(unsigned int k, unsigned int v, Hash hTable, Hash dTable,int n, FILE* fo) {
		int i = k%hTable.TableSize+(n*n);
		if(i >= hTable.TableSize) i -= hTable.TableSize;
		if((double)(dTable.size/dTable.TableSize) > 0.5 || (double)(hTable.size/hTable.TableSize) > 0.5) {
				rehash(hTable);
				rehash(dTable);
		}
	
		if(hTable.Table[i].status == false) {
				hTable.Table[i].key = k;
				hTable.Table[i].value = v;
				hTable.Table[i].status = true;
				hTable.size++;
				if(n == 0) fprintf(fo,"%s\n","Inserted");
				return;
		}
		
		else {
				if(n == 0) 	{
						fprintf(fo,"%s\n","Duplicated");
						col++;
				}
				if(hTable.Table[i].key==k) {
						hTable.Table[i].value = v;
						return;
				}
				else {
					insert(hTable.Table[i].key, hTable.Table[i].value, dTable, hTable, n+1,fo);
					hTable.Table[i].key = k;
					hTable.Table[i].value = v;
				}
		}
}

void deletehash(unsigned int k, Hash hTable, Hash dTable, int n, FILE* fo) {

		if(n == hTable.TableSize+dTable.TableSize+1) {
				fprintf(fo,"%s\n","Not found");
				return;
		}

		int i = k%hTable.TableSize+(n*n);
		if(i >= hTable.TableSize) i -= hTable.TableSize;
		
		if(hTable.Table[i].status == true&&hTable.Table[i].key == k) {
				hTable.Table[i].key=NULL;
				hTable.Table[i].value=NULL;
				hTable.Table[i].status = false;
				fprintf(fo,"%s\n","Deleted");
				return;
		}
		else deletehash(k, dTable, hTable, n+1,fo);
}	

void findhash(unsigned int k, Hash hTable, Hash dTable,int n, FILE* fo) {
		if(n == hTable.TableSize+dTable.TableSize+1) {
				fprintf(fo,"%s\n","Not found");
				return;
		}
		int i = k%hTable.TableSize+(n*n);
		if(i>=hTable.TableSize) i -= hTable.TableSize;
		if(hTable.Table[i].key == k) {
				fprintf(fo,"%d\n",hTable.Table[i].value);
				return;
		}
		else findhash(k, dTable,hTable,n+1,fo);
}

void printTable(Hash hTable, Hash dTable,FILE* fo) {
		fprintf(fo,"%s","key : ");
		for(int i=0;i<hTable.TableSize;i++) {
				if(hTable.Table[i].status == true) fprintf(fo,"%d ",hTable.Table[i].key);

			//	if(dTable.Table[i].status == true) printf("%d ",dTable.Table[i].key);
		}
		for(int i=0;i<dTable.TableSize;i++) {
			//	if(hTable.Table[i].status == true) printf("%d ",hTable.Table[i].key);

				if(dTable.Table[i].status == true) fprintf(fo,"%d ",dTable.Table[i].key);
		}

		fprintf(fo,"%s","\n");
		fprintf(fo,"%s","collision : ");
		fprintf(fo,"%d\n",col);
}
		
int main(int argc, char* argv[]) {
		FILE* fi = fopen(argv[1],"r");

		unsigned int n, k;
		char i;

		fscanf(fi, "%d", &n);

		Hash hTable;
		hTable.TableSize = n;
		hTable.size = 0;
		hTable.Table = (Node*)malloc(sizeof(Node)*n);
		

		Hash dTable;
		dTable.TableSize= n;
		dTable.size = 0;
		dTable.Table = (Node*)malloc(sizeof(Node)*n);

		int col = 0;
		FILE* fo = fopen("output.txt","wb");
		while(!feof(fi)) {
				fscanf(fi, "%c", &i);
				if(i == 'I') {
						unsigned int v;
						fscanf(fi, "%u %u",&k,&v);

						insert(k,v,hTable,dTable,0,fo);
				}
				else if(i == 'D') {
						fscanf(fi, "%u",&k);
						
						deletehash(k,hTable,dTable,0,fo);
				}
				else if(i == 'F') {
						fscanf(fi, "%u", &k);

						findhash(k,hTable,dTable,0,fo);
				}
				else if(i == 'P') printTable(hTable, dTable,fo);

				else if(i == 'Q') break;
		}
}

