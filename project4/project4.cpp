#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <pthread.h>
#include <queue>
#include <utility>
#include <limits>
#include <string.h>
#include <algorithm>
#define INFINITE 2147483647
using namespace std;
//vector<vector<int> > arr(1000,vector<int>(1000));
vector<vector<pair<int, int> > > arr(10000000);
//vector<priority_queue<pair<int, int> > > arr(1000);
vector<priority_queue<int> > ans(204800);
pthread_mutex_t s;

typedef struct a{
		int start;
		int end;
		int cnt;
}arrr;

void* shortPath(void* st) {
		arrr ab = *(arrr*)st;		
//		vector<priority_queue<pair<int, int> > > temp(1000);
//		temp=arr;
		static int retval = 999;

		priority_queue<pair<int,int> > Q;
		int answer=INFINITE;
		int ch[10000000];
//		memset(ch,INFINITE,10000);
		fill_n(ch,10000000,INFINITE);

		Q.push({0,ab.start});

		while(!Q.empty()) {
				int u=Q.top().second;
				int c = -Q.top().first;
				if(u==ab.end) {
						answer=ch[u];
						break;
				}
				ch[u] = c;
				Q.pop();
				
				if(ch[u]<c||ch[u]==INFINITE) continue;
				for(int i=0;i<arr[u].size();i++) {
		//				printf("%d %d\n",arr[u][i].first, arr[u][i].second);

						if(ch[arr[u][i].first]>ch[u]+arr[u][i].second){

								ch[arr[u][i].first]=ch[u]+arr[u][i].second;
								Q.push({-(ch[u]+arr[u][i].second), arr[u][i].first});
						}
				}

			/*	if(u==ab.end) break;
				if(temp[u].size()==0) {
						answer=numeric_limits<int>::max();
						break;
				}
				printf("%d\n",u);
				ch[u] = 1;
				Q.pop();
				int z=temp[u].top().second;

				while(ch[z]!=0) {
						temp[u].pop();
						z=(temp[u].top()).second;
						if(temp[u].size()==0){
								answer=numeric_limits<int>::max();
								break;
						}
				}
				Q.push(z);
			

				answer+=(-(temp[u].top().first));*/
		}
		ans[ab.cnt].push(-answer);
		pthread_mutex_unlock(&s);
		pthread_exit((void*)&retval);
		// pthread_cond_signal(&count_threshold_cv);

}

int main(int argc, char** argv) {
		FILE* fi = fopen(argv[1],"r");
		FILE* fo = fopen("output.txt","w");
		
		arrr* value;

		pthread_t threads[2048];
		vector<arrr*> vvec;


		int v, e, i, j, w, p, h;
		int* arr1;

		pthread_mutex_init(&s,NULL);

		fscanf(fi,"%d",&v);
		fscanf(fi,"%d",&e);
		value = (arrr*)malloc(sizeof(arrr));

		for(int a=0;a<e;a++) {
				fscanf(fi, "%d %d %d", &i, &j, &w);
			//	pair<int, int> f = make_pair(-w, j);
			//	arr[i].push(f);
			//	pair<int, int> z = make_pair(-w, i);
			//	arr[j].push(z);
				pair<int, int> f = make_pair(j, w);
				pair<int, int> z = make_pair(i, w);
				arr[i].push_back(f);
				arr[j].push_back(z);
		}
		arr1=(int*)malloc(sizeof(int)*h);

		fscanf(fi,"%d",&p);
		for(int a=0;a<p;a++) {
				fscanf(fi,"%d",&h);
			//	value = (arrr*)malloc(sizeof(arrr));
			//	value->arr1 = (int*)malloc(h*sizeof(int));
			//	value->size=h;

				for(int b=0;b<h;b++) {
						int i;
						fscanf(fi,"%d",&i);
						arr1[b]=i;
				}
				int i=0;
				for(int b=0;b<h;b++) {
					for(int c=b+1;c<h;c++) {
						pthread_mutex_lock(&s);
						value->start=arr1[b];
						value->end=arr1[c];
						value->cnt=a;
						vvec.push_back(value);
						
							
						pthread_create(&threads[i++],NULL,shortPath,(void*)vvec.back());
						
					}
				}
				while(ans[a].size()!=i);
				fprintf(fo,"%d\n",-ans[a].top());
		}
		free(arr1);

		pthread_exit(NULL);
}






