#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <engine.h>
//---//
#include "../include/tree.h"
#define SIZE    20000
#define COUNT   2000

_Bool get_num_comp(double *x, TREE *tree, double *tree_y, TREE *avl, double *avl_y){
    TREE *aux;
    srand(time(NULL));

    int *visited = (int *)malloc(sizeof(int)*SIZE);
    if(!visited)return 0;
    memset(visited, 0, sizeof(int)*SIZE);

    int i = 0;
    while(1){
        int count_avl = 0;
        int count_tree = 0;
        if(i == COUNT)
            break;

        int num = 1 + (rand() % SIZE);
        if(visited[num])
            continue;

        aux = tree_search(avl, num, &count_avl);
        if(!aux)return 0;

        aux = tree_search(tree, num, &count_tree);
        if(!aux)return 0;

        x[i] = (double)num;
        avl_y[i] = (double)count_avl;
        tree_y[i] = (double)count_tree;

        visited[num] = 1;
        i++;
    }

    free(visited);
    return 1;
}

void tree_mount(TREE **avl, TREE **tree){
    TREE *tree_aux = NULL;
    TREE *avl_aux = NULL;

    for(int i = 1; i <= SIZE; i++){
        tree_aux = tree_add(tree_aux, i);
        avl_aux = avl_add(avl_aux, i);
    }

    *avl = avl_aux;
    *tree = tree_aux;
}

_Bool plot(double *x, double *tree_y, double *avl_y){
    Engine *ep;
	mxArray *X = NULL,
            *TY = NULL,
            *AY = NULL;

	if(!(ep = engOpen(""))) {
		fprintf(stderr, "\nCan't start MATLAB engine\n");
		return 0;
	}

	X = mxCreateDoubleMatrix(1, COUNT, mxREAL);
	memcpy((void *)mxGetPr(X), (void *)x, sizeof(double)*COUNT);
	engPutVariable(ep, "X", X);

    TY = mxCreateDoubleMatrix(1, COUNT, mxREAL);
	memcpy((void *)mxGetPr(TY), (void *)tree_y, sizeof(double)*COUNT);
	engPutVariable(ep, "TY", TY);

    AY = mxCreateDoubleMatrix(1, COUNT, mxREAL);
	memcpy((void *)mxGetPr(AY), (void *)avl_y, sizeof(double)*COUNT);
	engPutVariable(ep, "AY", AY);

    //PLOT ABB VS AVL
    engEvalString(ep, "subplot(3, 1, 1);");
	engEvalString(ep, "plot(X, AY, 'b.', X, TY, 'k.');");
	engEvalString(ep, "title('ABB Desbalanceada vs AVL');");
    engEvalString(ep, "legend('ABB', 'AVL');");
	engEvalString(ep, "xlabel('ITEM');");
	engEvalString(ep, "ylabel('NUMBER OF COMPARISONS');");
	engEvalString(ep, "grid;");

    //PLOT ABB
    engEvalString(ep, "subplot(3, 1, 2);");
    engEvalString(ep, "plot(X, TY, 'k.');");
	engEvalString(ep, "title('ABB Desbalanceada');");
    engEvalString(ep, "legend('ABB');");
	engEvalString(ep, "xlabel('ITEM');");
	engEvalString(ep, "ylabel('NUMBER OF COMPARISONS');");
	engEvalString(ep, "grid;");

    //PLOT AVL
    engEvalString(ep, "subplot(3, 1, 3);");
    engEvalString(ep, "plot(X, AY, 'b.');");
	engEvalString(ep, "title('AVL');");
    engEvalString(ep, "legend('AVL');");
	engEvalString(ep, "xlabel('ITEM');");
	engEvalString(ep, "ylabel('NUMBER OF COMPARISONS');");
	engEvalString(ep, "grid;");

	printf("Hit return to continue\n\n");
	fgetc(stdin);

	mxDestroyArray(X);
	mxDestroyArray(TY);
	mxDestroyArray(AY);
	engEvalString(ep, "close;");
    return 1;
}

int main(){
    _Bool status;

    printf("Montando arvores.... ");
    fflush(stdout);

    TREE *avl = NULL, *tree = NULL;
    tree_mount(&avl, &tree);
    printf("Ok\n");

    double *x = (double *)malloc(sizeof(double)*COUNT);
    if(!x)return EXIT_FAILURE;

    double *tree_y = (double *)malloc(sizeof(double)*COUNT);
    if(!tree_y)return EXIT_FAILURE;

    double *avl_y = (double *)malloc(sizeof(double)*COUNT);
    if(!avl_y)return EXIT_FAILURE;

    status = get_num_comp(x, tree, tree_y, avl, avl_y);
    if(!status)return EXIT_FAILURE;

    for(int i = 0; i < COUNT; i++){
        printf("=======================\n");
        printf("TREE: N: %.0lf C: %.0lf\n", x[i], tree_y[i]);
        printf("AVL: N: %.0lf C: %.0lf\n", x[i], avl_y[i]);
    }
    printf("=======================\n");

    status = plot(x, tree_y, avl_y);
    if(!status)return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
}