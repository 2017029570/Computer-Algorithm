#include <stdio.h>
#include <algorithm>
using namespace std;

typedef struct Node* Tree;

struct Node {
		unsigned int key;
		unsigned int value;
		Node* Left;
		Node* Right;
		int height;
};

int Height(Tree tree) {
		if(tree == NULL) return -1;
		else return tree->height;
}

Tree find(Tree tree, unsigned int k,unsigned int v,unsigned int d, FILE* fo) {
		if(tree == NULL) {
				return NULL;
		}

		else {
			if(tree->key > k) find(tree->Left, k,v, ++d,fo);
			else if(tree->key < k) find(tree->Right, k,v,++d, fo);
			else if(tree->key == k) {
					fprintf(fo, "Found (%u, %u) on d=%d with h=%d ",k,tree->value,d,tree->height);
					if(v != -1) tree->value = v; 
					return tree;
			}
		}
}

Tree SingleLeft(Tree tree) {
		Tree temp = tree->Left->Right;
		tree->Left->Right = tree->Right;
		Tree NewTree = tree->Left;
		if(NewTree->Right != NULL)
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
		Tree temp = tree->Right->Left;
		tree->Right->Left = tree;
		Tree NewTree = tree->Right;
		if(NewTree->Left != NULL) NewTree->Left->Right = temp;
		else {
				NewTree->Left = tree;
				NewTree->Left->Right = NULL;
		}
		
		NewTree->Left->height = max(Height(NewTree->Left->Left), Height(NewTree->Left->Right))+1;
		NewTree->height = max(Height(NewTree->Left), Height(NewTree->Right))+1;
		
		return NewTree;
}

Tree DoubleRight(Tree tree) {
		tree->Right = SingleLeft(tree->Right);
		tree = SingleRight(tree);

		return tree;
}

Tree DoubleLeft(Tree tree) {
		tree->Left = SingleRight(tree->Left);
		tree = SingleLeft(tree);

		return tree;
}
Tree insert(Tree tree, unsigned int k, unsigned int v, FILE* fo) {
		
		if(find(tree, k, v,0,fo)!=NULL) 
				fprintf(fo,"update v=%d\n",v);  
		else {
			if(tree == NULL) {
					tree = (Tree)malloc(sizeof(struct Node));
					tree->key = k;
					tree->value = v;
					tree->Left = NULL;
					tree->Right = NULL;
					tree->height = 0;
					fprintf(fo,"Inserted (%d, %d)\n",k, v);
			}
			else {
					if(tree->key < k) {
							tree->Right = insert(tree->Right, k, v,fo);
							if(Height(tree->Right) - Height(tree->Left)>=2) {
									if(tree->Right->key > k) tree = DoubleRight(tree);
									else tree = SingleRight(tree);
							}
					}

					else if(tree->key > k) {
							tree->Left = insert(tree->Left, k, v,fo);
							if(Height(tree->Left) - Height(tree->Right)<=-2) {
									if(tree->Left->key < k) tree = DoubleLeft(tree);
									else tree = SingleLeft(tree);
							}
					}
			}
			tree->height = max(Height(tree->Left), Height(tree->Right))+1;
		}
		return tree;
}

void print(Tree tree, FILE* fo) {

		if(tree == NULL) fprintf(fo,"Tree is empty.\n");
		fprintf(fo,"(%d, %d) ",tree->key, tree->value);
		if(tree->Left != NULL) print(tree->Left, fo);
		if(tree->Right != NULL) print(tree->Right, fo);

}

int main(int argc, char* argv[]) {
	FILE* fi = fopen(argv[1], "rb");
	FILE* fo = fopen("output.txt","wb");

	char i;
	unsigned int k;
	Tree AVLTree = NULL;

	while(!feof(fi)) {
			fscanf(fi, "%c", &i);

			if(i == 'I') {
					unsigned int v;
					fscanf(fi,"%u %u", &k, &v);

					AVLTree = insert(AVLTree, k, v, fo);
			}

			else if(i == 'P') {
					print(AVLTree, fo);
					fprintf(fo,"\n");
			}

			else if(i == 'F') {
					fscanf(fi, "%u", &k);

					if(find(AVLTree, k, -1, 0, fo) == NULL) fprintf(fo,"Not Found\n");
					else fprintf(fo,"\n");
			}
			else if(i == 'Q') {
					fclose(fi);
					fclose(fo);
					return 0;
			}
	}

}
