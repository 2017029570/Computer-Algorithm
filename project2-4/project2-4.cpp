#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
		unsigned int key;
		unsigned int value;
		bool status = false;
}Node;

typedef struct HashTable {
		int MaxTableSize;
		Node* Table;
		int H;
		int col;
}Hash;
int h(unsigned k,Hash HashTable) {
		return k%HashTable.MaxTableSize;
}

Hash insert(unsigned int k, unsigned int v, Hash HashTable,int i, FILE* fo) {
//		int i = h(k);
		if(HashTable.Table[i].status == false) {
				HashTable.Table[i].key = k;
				HashTable.Table[i].value = v;
				HashTable.Table[i].status = true;
				fprintf(fo,"%s\n","Inserted");
				return HashTable;
		}

		else {
				if(i == h(k,HashTable)) {
						fprintf(fo,"%s\n","Duplicated");
						HashTable.col += 1;
				}
				if(HashTable.Table[i].key == k) HashTable.Table[i].value = v;
				else {
					int a = h(k,HashTable);
					while(HashTable.Table[i].status == true) {
							i++;
							if(i >= HashTable.MaxTableSize) i -= HashTable.MaxTableSize;
					}
					
					if(i-a <= HashTable.H || a-i <= HashTable.H) {
							HashTable.Table[i].key = HashTable.Table[a].key;
							HashTable.Table[i].value = HashTable.Table[a].value;
							HashTable.Table[i].status = true;
	
							HashTable.Table[a].key = k;
							HashTable.Table[a].value = v;
							HashTable.Table[a].status = true;
					}
				
				
					else {
							int q = i-HashTable.H+1;
		
							insert(k,v,HashTable,q,fo);			
					}
				}
			}
		return HashTable;
}

Hash deletehash(unsigned int k, Hash HashTable,FILE* fo) {
		int i = h(k,HashTable);
		int ch = 0;
		for(int c=0;c<HashTable.H;c++) {
			if(HashTable.Table[i].status == true&&HashTable.Table[i].key == k) {
					HashTable.Table[i].key = NULL;
					HashTable.Table[i].value = NULL;
					HashTable.Table[i].status = false;
					fprintf(fo,"%s\n","Deleted");
					ch = 1;
					break;
			}
			else i++;
		}
		if(ch != 1) fprintf(fo,"%s\n","Not Found");
		return HashTable;
}

void find(unsigned int k, Hash HashTable, FILE* fo) {
		int i=h(k,HashTable);
		int ch=0;
		for(int c=0;c<HashTable.H;c++) {
				if(HashTable.Table[i].status == true&&HashTable.Table[i].key == k) {
						fprintf(fo,"%u\n",HashTable.Table[i].value);
						ch=1;
						break;
				}
				else i++;
		}
		if(ch != 1) fprintf(fo,"%s\n","Not Found");
}

void printTable(Hash HashTable,FILE* fo) {
		fprintf(fo,"%s","keys : ");
		for(int i=0;i<HashTable.MaxTableSize;i++) {
				if(HashTable.Table[i].status==true) 
						fprintf(fo,"%d ",HashTable.Table[i].key);
	}
		fprintf(fo,"%s","\n");
		fprintf(fo,"%s","collision : ");
		fprintf(fo,"%d\n",HashTable.col);
}
int main(int argc, char* argv[]) {
		FILE* fi = fopen(argv[1],"r");

		unsigned int n, k;
		char i;

		fscanf(fi,"%d",&n);

		Hash HashTable;
		HashTable.MaxTableSize = n;
		HashTable.Table = (Node*)malloc(sizeof(Node)*n);
		HashTable.H = sizeof(unsigned int);
		HashTable.col = 0;

		FILE* fo = fopen("output.txt","wb");

		while(!feof(fi)) {
				fscanf(fi,"%c",&i);
				if(i=='I') {
						unsigned int v;
						fscanf(fi,"%u %u",&k,&v);

						HashTable = insert(k,v,HashTable,h(k,HashTable),fo);
				}
				else if(i == 'D') {
						fscanf(fi,"%u",&k);

						HashTable = deletehash(k,HashTable,fo);
				}
				else if(i == 'F') {
						fscanf(fi,"%u",&k);

						find(k,HashTable,fo);
				}
				else if(i=='P') printTable(HashTable,fo);

				else if(i=='Q') break;
		}
}
