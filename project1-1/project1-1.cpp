#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cstring>
using namespace std;
void swap(int* a, int* b) {
		int temp = *a;
		*a = *b;
		*b = temp;
}

int Partition(int arr[], int left, int right) {
		int first = left;
		int pivot = arr[first];
		 ++left;

		 while(left<=right) {
				 while(pivot>=arr[left] && left<right) ++left;
				 while(pivot<arr[right] && left<=right) --right;

				 if(left<right) swap(&arr[left], &arr[right]);
				 else break;
		 }

		 swap(&arr[first],&arr[right]);

		 return right;
}

void Quicksort(int arr[],int left, int right) {
		if(left<right) {
			int index = Partition(arr,left,right);

			Quicksort(arr, left, index-1);
			Quicksort(arr, index+1, right);
		}
}

int main(int argc, char* argv[]) {
		FILE* fi = fopen(argv[1], "r");

		int arr[100001];
		
		char* token;
		char str[200000];
		int size, i=0;
		fgets(str, sizeof(str), fi);
		token = strtok(str, " ");
		size = atoi(token);
		while(i != size) {
				token = strtok(NULL," ");
				arr[i] = atoi(token);
				i++;
		}

		Quicksort(arr,0,size-1);

		FILE* fo = fopen("output.txt","wb");
		fwrite(arr, sizeof(int), size, fo);
		fclose(fo);

		int bff[100001];
		fo = fopen("output.txt","rb");
		fread(bff,sizeof(int),size,fo);
		for(int i=0;i<size;i++) printf("%d ",bff[i]);
		printf("\n");
		fclose(fo);
}


