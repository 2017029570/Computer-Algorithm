#include <stdio.h>
#include <map>
#include <vector>
#include <stdlib.h>
using namespace std;

typedef struct Node {
		unsigned int key;
		unsigned int value;
		bool status = false;
}Node;
 typedef struct SHash {
		int size;
		int tsize;
		Node* table;
		Node first;
		int a,b;
		int hashfunc(unsigned int k,int a, int b){
					int p = 101;
					int m = size*size;
					return ((((a*k)+b)%p)%m);
		}

}SHash;

typedef struct PHash {
		int Size;
		int col;
		map<unsigned int, int> ch;
		map<unsigned int, SHash> Table;
}Hash;

Hash insert1(unsigned int k, Hash HashTable) {
		HashTable.ch[k%HashTable.Size] += 1;
		return HashTable;
}

Hash FirstHashing(Hash HashTable,FILE* fo) {
		map<unsigned int, int>::iterator it = HashTable.ch.begin();
		while(it!=HashTable.ch.end()) {
			SHash S;
			Node N;
			S.size = HashTable.ch[it->first];
			S.first = N;
			S.tsize = 0;
			S.a = rand() % 101;
			S.b = rand() % 101;

			S.table = (Node*)malloc(sizeof(Node)*S.size*S.size);
			HashTable.Table[it->first] = S;
			it++;

		}

		fprintf(fo,"%s\n","Hashing completed");
		return HashTable;
}


Hash insert(unsigned int k, unsigned int v, Hash HashTable, FILE* fo) {
		if(HashTable.Table[k%HashTable.Size].first.status == false) {
				HashTable.Table[k%HashTable.Size].first.key = k;
				HashTable.Table[k%HashTable.Size].first.value = v;
				HashTable.Table[k%HashTable.Size].first.status = true;
				fprintf(fo,"%s\n","Inserted in first hash");
		}

		else {
				int i = HashTable.Table[k%HashTable.Size].hashfunc(k,HashTable.Table[k%HashTable.Size].a,HashTable.Table[k%HashTable.Size].b);
				if(HashTable.Table[k%HashTable.Size].table[i].status == true) {
						if(HashTable.Table[k%HashTable.Size].table[i].key == k) {
							fprintf(fo,"%s\n","Duplicated");
							HashTable.col++; 
							HashTable.Table[k%HashTable.Size].table[i].value = v;
						}
				}
				
					else {
							HashTable.Table[k%HashTable.Size].table[i].key = k;
							HashTable.Table[k%HashTable.Size].table[i].value = v;
							HashTable.Table[k%HashTable.Size].table[i].status = true;
							HashTable.Table[k%HashTable.Size].tsize++;
							fprintf(fo,"%s\n","Inserted in second hash");
					}
				 
		}
		return HashTable;
}
		
void findhash(unsigned int k, Hash HashTable,FILE* fo) {
		if(HashTable.Table[k%HashTable.Size].first.status == true) {
				if(HashTable.Table[k%HashTable.Size].first.key == k)
						fprintf(fo,"%d\n",HashTable.Table[k%HashTable.Size].first.value);
		

				else {
					int i = HashTable.Table[k%HashTable.Size].hashfunc(k,HashTable.Table[k%HashTable.Size].a,HashTable.Table[k%HashTable.Size].b);
					if(HashTable.Table[k%HashTable.Size].table[i].status == true) {
						fprintf(fo,"%d\n",HashTable.Table[k%HashTable.Size].table[i].value);
					}
					else fprintf(fo,"%s\n","Not Found");
				}
		}
		else fprintf(fo,"%s\n","Not Found");
}

Hash deletehash(unsigned int k, Hash HashTable,FILE* fo) {
		if(HashTable.Table[k%HashTable.Size].first.key == k) {
				HashTable.Table[k%HashTable.Size].first.key = NULL;
				HashTable.Table[k%HashTable.Size].first.value = NULL;
				HashTable.Table[k%HashTable.Size].first.status = false;
				fprintf(fo,"%s\n","Deleted");
				
				if(HashTable.Table[k%HashTable.Size].tsize!=0) {
						for(int a=0;a<HashTable.Table[k%HashTable.Size].size;a++) {
								if(HashTable.Table[k%HashTable.Size].table[a].status == true) {
										HashTable.Table[k%HashTable.Size].first.key = HashTable.Table[k%HashTable.Size].table[a].key;
										HashTable.Table[k%HashTable.Size].first.value = HashTable.Table[k%HashTable.Size].table[a].value;
										HashTable.Table[k%HashTable.Size].first.status=true;
										HashTable.Table[k%HashTable.Size].table[a].key=NULL;
										HashTable.Table[k%HashTable.Size].table[a].value = NULL;
										HashTable.Table[k%HashTable.Size].table[a].status = false;
										HashTable.Table[k%HashTable.Size].tsize--;
										break;
								}
						}

			}
		}
		else {
				int i = HashTable.Table[k%HashTable.Size].hashfunc(k,HashTable.Table[k%HashTable.Size].a,HashTable.Table[k%HashTable.Size].b);
				if(HashTable.Table[k%HashTable.Size].table[i].status == true) {
					HashTable.Table[k%HashTable.Size].table[i].key = NULL;
					HashTable.Table[k%HashTable.Size].table[i].value = NULL;
					HashTable.Table[k%HashTable.Size].table[i].status = false;
					fprintf(fo,"%s\n","Deleted");
					HashTable.Table[k%HashTable.Size].tsize--;
				}
				else fprintf(fo,"%s\n","Not Found");
		}
		return HashTable;
}
				
void printTable(Hash HashTable,FILE* fo) {
		fprintf(fo,"%s","key: : ");
		map<unsigned,int>::iterator it = HashTable.ch.begin();

		for(it = HashTable.ch.begin(); it != HashTable.ch.end(); ++it) {
				if(HashTable.Table[it->first].first.status == true) {
							fprintf(fo,"%d ",HashTable.Table[it->first].first.key);
							for(int i = 0;i<(it->second)*(it->second);i++) {
									if(HashTable.Table[it->first].table[i].status == true) {
											fprintf(fo,"%d ",HashTable.Table[it->first].table[i].key);
									}
							}
				}
		}

		fprintf(fo,"%s","\n");
		fprintf(fo,"%s","collision : ");
		fprintf(fo,"%d\n",HashTable.col);
}
		
int main(int argc, char* argv[]) {
		FILE* fi = fopen(argv[2], "r");

		int n;
		unsigned int k;
		char i;

		fscanf(fi, "%d", &n);

		Hash HashTable={};
		HashTable.Size = n;	
		HashTable.col = 0;
		vector<unsigned int> v;

	
		for(int i=0;i<n;i++) {
				fscanf(fi,"%d",&k);
				v.push_back(k);
				HashTable.ch[k%HashTable.Size]=0;
		}
		for(int i=0;i<n;i++) {
				HashTable = insert1(v[i],HashTable);
		}
	
		FILE* fo = fopen("output.txt","wb");
		HashTable = FirstHashing(HashTable,fo);
		fi = fopen(argv[1],"r");
		while(!feof(fi)) {
				fscanf(fi,"%c",&i); 
				if(i == 'I') {
						unsigned int v;
						fscanf(fi, "%u %u", &k,&v);

						HashTable = insert(k,v,HashTable,fo);
				}
				else if(i == 'D') {
						fscanf(fi, "%u",&k);

						HashTable = deletehash(k,HashTable,fo);
				}
				else if(i == 'F') {
						fscanf(fi,"%u",&k);

						findhash(k,HashTable,fo);
				}
				else if(i == 'P') printTable(HashTable,fo);

				else if(i == 'Q') break;
		}
}



