#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
using namespace std;

typedef struct Node* Tree;
#define Red false
#define Black true

struct Node {
		unsigned long long int key;
		unsigned long long int value;
		Node* Left;
		Node* Right;
		int height;
		bool Color;
};

int Height(Tree tree) {
		if(tree == NULL) return -1;
		else return tree->height;
}

Tree find(Tree tree, unsigned long long int k, unsigned long long int v, unsigned int d, FILE* fo) {
		if(tree == NULL) return NULL;
		else {
				if(tree->key > k) find(tree->Left, k, v, ++d, fo);
				else if(tree->key < k) find(tree->Right, k, v, ++d, fo);
				else if(tree->key == k) {
						fprintf(fo, "Found (%llu, %llu) on d=%d with c=", k, tree->value, d);
						if(tree->Color == Red) fprintf(fo,"Red ");
						else fprintf(fo,"Black ");
						if(v != -1) tree->value = v;
						return tree;
				}
		}
}

Tree ReColor(Tree tree) {
		tree->Color = Red;
		tree->Right->Color = Black;
		tree->Left->Color = Black;
		
		return tree;
}

Tree SingleLeft(Tree tree) {
		tree->Color = Red;
		tree->Left->Color = Black;
		Tree temp = tree->Left->Right;
		tree->Left->Right = tree->Right;
		Tree NewTree = tree->Left;
		if(NewTree->Right!=NULL)
				NewTree->Right->Left = temp;
		else {
				NewTree->Right = tree;
				NewTree->Right->Left = NULL;
		}
		NewTree->Right->height = max(Height(NewTree->Right->Left), Height(NewTree->Right->Right))+1;
		NewTree->height = max(Height(NewTree->Left), Height(NewTree->Right))+1;
		return NewTree;
}

Tree SingleRight(Tree tree) {
		tree->Color = Red;
		tree->Right->Color = Black;
		Tree temp = tree->Right->Left;
		tree->Right->Left = tree->Left;
		Tree NewTree = tree->Right;
		if(NewTree->Left!=NULL)
				NewTree->Left->Right = temp;
		else {
				NewTree->Left = tree;
				NewTree->Left->Right = NULL;
		}
		NewTree->Left->height = max(Height(NewTree->Left->Right), Height(NewTree->Left->Left))+1;
		NewTree->height = max(Height(NewTree->Right), Height(NewTree->Left))+1;
		return NewTree;
}

Tree DoubleRight(Tree tree) {
		tree->Color = Red;
		//tree->Right->Left->Color = Black;
		tree->Right = SingleLeft(tree->Right);
		tree = SingleRight(tree);

		return tree;
}

Tree DoubleLeft(Tree tree) {
		tree->Color = Red;
		//tree->Left->Right->Color = Black;
		tree->Left = SingleRight(tree->Left);
		tree = SingleLeft(tree);

		return tree;
}


Tree insert(Tree tree, unsigned long long int k, unsigned long long int v, FILE* fo) {
		if(find(tree, k, v, 0, fo) != NULL)
				fprintf(fo, "update v=%llu\n",v);
		else {
				if(tree == NULL) {
						tree = (Tree)malloc(sizeof(struct Node));
						tree->key = k;
						tree->value = v;
						tree->Left = NULL;
						tree->Right = NULL;
						tree->Color = Red;
						tree->height = 0;
						fprintf(fo, "Inserted (%llu, %llu)\n",k,v);
				}
				else {
						if(tree->key < k) {
								tree->Right = insert(tree->Right, k, v, fo);
								if(tree->Right->Color == Red && tree->Right->height > 0)  {
									if(tree->Left == NULL || tree->Left->Color == Black) {
											if(tree->Right->Right!=NULL&&tree->Right->Right->Color == Red) 
													tree = SingleRight(tree);
											else if(tree->Right->Left!=NULL&&tree->Right->Left->Color == Red) {
													tree = DoubleRight(tree);
											}
									}
									else if(tree->Left->Color == Red)
											tree = ReColor(tree);
								}
					}
						else if(tree->key > k) {
								tree->Left = insert(tree->Left, k, v, fo);
								if(tree->Left->Color == Red && tree->Left->height > 0) {
									if(tree->Right == NULL || tree->Right->Color == Black) {
											if(tree->Left->Left->Color == Red)
													tree = SingleLeft(tree);
											else if(tree->Left->Right->Color == Red)
													tree = DoubleLeft(tree);
									}
									else if(tree->Right->Color == Red)
											tree = ReColor(tree);
								}
						}
				}
		
			tree->height = max(Height(tree->Left), Height(tree->Right))+1;
		}
		return tree;
}

Tree root(unsigned long long int k, unsigned long long int v, FILE* fo) {
		Tree tree = (Tree)malloc(sizeof(struct Node));
		tree->key = k;
		tree->value = v;
		tree->Right = NULL;
		tree->Left = NULL;
		tree->Color = Black;
		tree->height = 0;

		fprintf(fo,"Inserted (%llu, %llu)\n",k, v);
		return tree;
}

void print(Tree tree, FILE* fo) {
		if(tree == NULL) fprintf(fo, "Tree is empty.\n");
		fprintf(fo,"(%llu, %llu) ",tree->key, tree->value);
		if(tree->Left!=NULL) print(tree->Left, fo);
		if(tree->Right!=NULL) print(tree->Right,fo);
}

int main(int argc, char* argv[]) {
		FILE* fi = fopen(argv[1], "rb");
		FILE* fo = fopen("output.txt","wb");

		char i;
		unsigned long long int k;
		Tree RBTree = NULL;
		while(!feof(fi)) {
				fscanf(fi, "%c", &i);

				if(i == 'I') {
						unsigned long long int v;
						fscanf(fi, "%llu %llu", &k, &v);
						
						if(RBTree == NULL) 
								RBTree = root(k,v,fo);
						else
								RBTree = insert(RBTree, k, v, fo);
				}

				else if(i == 'P') {
						print(RBTree, fo);
						fprintf(fo, "\n");
				}

				else if(i == 'F') {
						fscanf(fi, "%llu", &k);
						if(find(RBTree, k, -1, 0, fo) == NULL) fprintf(fo, "Not Found\n");
						else fprintf(fo, "\n");
				}

				else if(i == 'Q') {
						fclose(fi);
						fclose(fo);
						return 0;
				}
		}
}

