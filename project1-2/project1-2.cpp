#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;

int* MergeSort(int* arr, int* arr2, int size, int m) {
		int* answer = new int[m+size];
		
//		for(int i=0;i<size;i++) printf("%d ",arr2[i]);
		if(size == 0) {
				for(int i=0;i<m;i++) answer[i] = arr[i];
		}

		else {
				int i,j,k;
				i = j = k =0;
				while(i<m && j<size) {
						if(arr[i] <= arr2[j]) {
								answer[k] = arr[i];
								i++;
						}

						else {
								answer[k] = arr2[j];
								j++;
						}
						k++;
//						printf("i : %d j : %d\n",i,j);
				}

				if(j<size) {
						for(int l=k;l<size+m;l++) {
								answer[l] = arr2[j];
								j++;
						}
				}

				else if(i<m) {
						for(int l=k;l<size+m;l++) {
								answer[l] = arr[i];
								i++;
						}
				}
		}

//		for(int i=0;i<size;i++) printf("%d ",arr2[i]);
//		for(int b=0;b<size+m;b++) printf("%d ",answer[b]);
//		printf("\n");

		return answer;
}

int main(int argc, char* argv[]) {
		FILE* fi = fopen(argv[1], "r");

		int n, m, k;

		char str[1000001];
		fgets(str, sizeof(str),fi);
		sscanf(str, "%d %d %d",&n,&m,&k);
		
		int arr[m];
		int* answer = new int[n*m];
		int size=0;
		for(int g=2;g<argc;g++) {
				FILE* f = fopen(argv[g],"r");	
				fgets(str, sizeof(str), f);

				char* token;
				int i=0;
		
				token = strtok(str, " ");
				while(i!=m) {
						arr[i] = atoi(token);
						i++;
						token = strtok(NULL," ");
				};
				int* print = new int[m+size];
				print = MergeSort(arr,answer,size,m);
				answer = print;
	//			for(int a=0;a<size+m;a++) printf("%d ",print[a]);
	//			printf("\n");
				size+=m;
			//	free(print);
				fclose(f);
		}

//		for(int a=0;a<size+m;a++) printf("%d ",answer[a]);
	/*	int out[size/k];
		int b = k-1;
		for(int a=0;a<size/k;a++) {
				out[a] = answer[b];
						b+=k;
				}
	*/
	//	string out = "";
	//	for(int a=0;a<size/k;a++) out+=to_string(answer[a]);
	//	cout<<out<<endl;
		FILE* fo = fopen("output.txt","w");
	//	fwrite(out,sizeof(char), size/k,fo);
	//	for(int i=0;i<size/k;i++) putw(out[i],fo);
		for(int a=0;a<size/k;a++)
			fprintf(fo, "%d ",answer[a]);
		
		fclose(fo);

	/*	int bff[100001];
		fo = fopen("output.txt","rb");
		fread(bff,sizeof(int),size/k,fo);
		for(int i=0;i<size/k;i++) printf("%d ",bff[i]);
		printf("\n");
		fclose(fo);*/

}
