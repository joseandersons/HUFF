

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct binary_tree bt;
typedef struct avl_tree avl;


//     ### ABB ###


struct binary_tree
{
	int item;
	bt *left;
	bt *right;
};

bt *create_bt(int item, bt *left, bt *right)
{
	bt *new_bt = (bt*) malloc(sizeof(bt));
	new_bt->item = item;
	new_bt->left = left;
	new_bt->right = right;

	return new_bt;
}

bt *add_leaf_bt(bt *tree, int item)
{
	if(tree == NULL)
	{
		tree = create_bt(item, NULL, NULL);
	}
	else if(tree->item > item)
	{
		tree->left = add_leaf_bt(tree->left, item);
	}
	else
	{
		tree->right = add_leaf_bt(tree->right, item);
	}

	return tree;
}


//		### AVL ###


struct avl_tree
{
	int item;
	int h;
	avl *left;
	avl *right;
};

int max(int a, int b)
{
	return (a > b) ? a : b;
}

int height(avl *tree)
{
	if(tree == NULL)
	{
		return -1;
	}
	else
	{
		return 1 + max(height(tree->left), height(tree->right));
	}
}

avl *create_avl(int item, avl *left, avl *right)
{
	avl *new_avl = (avl*)malloc(sizeof(avl));
	new_avl->item = item;
	new_avl->left = left;
	new_avl->right = right;
	new_avl->h = height(new_avl);

	return new_avl;
}

avl *rotate_left(avl *tree)
{
	avl *subtree_root = NULL;
	if(tree != NULL && tree->right != NULL)
	{
		subtree_root = tree->right;
		tree->right	= subtree_root->left;
		subtree_root->left = tree;
	}
	subtree_root->h = height(subtree_root);
	tree->h = height(tree);

	return subtree_root;
}

avl *rotate_right(avl *tree)
{
	avl *subtree_root = NULL;
	if(tree != NULL & tree->left != NULL)
	{
		subtree_root = tree->left;
		tree->left = subtree_root->right;
		subtree_root->right = tree;
	}
	subtree_root->h = height(subtree_root);
	tree->h = height(tree);

	return subtree_root;
}

int balance_factor(avl *tree)
{
	if(tree != NULL)
	{
		return (height(tree->left) - height(tree->right));
	}
	return 0;
}

avl *add_leaf_avl(avl *tree, int item)
{
	if(tree == NULL)
	{
		return create_avl(item, NULL, NULL);
	}
	else if(tree->item > item)
	{
		tree->left = add_leaf_avl(tree->left, item);
	}
	else
	{
		tree->right = add_leaf_avl(tree->right, item);
	}

	tree->h = height(tree);
	avl *child;

	if(balance_factor(tree) == 2 || balance_factor(tree) == -2)
	{
		if(balance_factor(tree) == 2)
		{
			child = tree->left;
			if(balance_factor(child) == -1)
			{
				tree->left = rotate_left(child);
			}
			tree = rotate_right(tree);
		}
		else if(balance_factor(tree) == -2)
		{
			child = tree->right;

			if(balance_factor(child) == 1)
			{
				tree->right = rotate_right(child);
			}
			tree = rotate_left(tree);
		}
	}
	return tree;
}

int ABB_AVL_counter(avl *tree, int item)
{
	if(tree == NULL || tree->item == item)
	{
		return 1;
	}
	else if(tree->item > item)
	{
		return 1 + ABB_AVL_counter(tree->left, item);
	}
	else
	{
		return 1 + ABB_AVL_counter(tree->right, item);
	}
}

int main()
{
	bt *binary_tree = NULL;
	avl *tree = NULL; 

	FILE *Comp_Data = NULL;

	Comp_Data = fopen("Comp_Data.txt", "w+");

	fprintf(Comp_Data, "%s\t%s\t%s\t%s\n", "Comparações_AVL", "Valor", "Comparações_ABB", "Valor");

	int i, x, index, array_size, counter;
	int countAVL = 0, countABB = 0;
	srand(time(NULL));

	printf("Insira o tamanho do array:\t");
	scanf("%d", &array_size);
	printf("Insira quantas buscas deve-se fazer:\t");
	scanf("%d", &counter);

	int *array = (int*)malloc(sizeof(int) * array_size);
	int *array_visited = (int*)calloc(array_size, sizeof(int));

	for(i = 0; i < array_size; i++)
	{
		array[i] = i;
		binary_tree = add_leaf_bt(binary_tree, i);
		tree = add_leaf_avl(tree, i);
	}

	for(i = 0; i < counter; i++)
	{
		index = rand() % array_size;
		if(!array_visited[index])
		{
			countABB = ABB_AVL_counter(binary_tree, array[index]);
			countAVL = ABB_AVL_counter(tree, array[index]);
			array_visited[index] = 1;

			fprintf(Comp_Data, "%d\t%d\t%d\t%d\n", countAVL, array[index], countABB, array[index]);
		}
	}

	free(array);
	free(array_visited);

	return 0;
}