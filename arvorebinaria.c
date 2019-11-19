#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

struct tnode *buscar(struct tnode *R, int v2);
struct tnode *new_no(int v);
void inserir(struct tnode **R, int v2);
void tprint(struct tnode *r);
void tremove(struct tnode *r);
#define MICRO_PER_SECOND 1000000 

struct tnode{
	int v;
	struct tnode *l;
	struct tnode *r;
};

int main(){
	struct timeval start_time, stop_time;
	FILE *arquivo;
	float media = 0.0, time_diff;
	int repeticoes_vetor = 100; 
	int n=100; // numero de elementos
	srand(time(NULL));
	do{
		arquivo = fopen("t2.txt", "a");
		repeticoes_vetor = 100000;
		struct tnode *R;
		R = NULL;
		for(int i=0; i<n; i++){
			inserir(&R, 1 + ( rand() % n )); // pior caso: insere i+1, caso medio: inserir aleatorio e buscar aleatorio
			//inserir(&R, i+1);
		}
		for(int i=0; i<repeticoes_vetor; i++){ 
			gettimeofday(&start_time, NULL);
			buscar(R, 1 + ( rand() % n )); // pior caso: busca por n
			//buscar(R, n+1);
			gettimeofday(&stop_time, NULL);
			time_diff = (float)(stop_time.tv_sec - start_time.tv_sec);
			time_diff += (stop_time.tv_usec - start_time.tv_usec)/(float)MICRO_PER_SECOND;
			media += time_diff;
		}
		tremove(R);
		fprintf(arquivo, "%d %.20f\n", n, media/repeticoes_vetor);
		printf("- %d Concluido! Gastou %.20f\n", n, media/repeticoes_vetor);
		media = 0.0;
		n+=100;
		fclose(arquivo);
	}while(n <= 10000);
	
	
	return 0;
}

struct tnode *new_no(int v){
	struct tnode *n = (struct tnode*)malloc(sizeof(struct tnode));
	n->v=v;
	n->l=NULL;
	n->r=NULL;
	return n;
}

void inserir(struct tnode **R, int v2){
	struct tnode *p, *f;
	if(*R == NULL){
		(*R) = new_no(v2);
	}else{
		p = (*R);
		while(p != NULL){
			f=p;
			if(p->v == v2)
				return;
			if(p->v < v2)
				p = p->r;
			else
				p = p->l;
		}
		
		if(f->v < v2)
			f->r = new_no(v2);
		else
			f->l = new_no(v2); 
	}
}

struct tnode *buscar(struct tnode *R, int v2){
	if(R != NULL){
		if(R->v == v2){
			return R;
		}
		if(R->v < v2){
			return buscar(R->r, v2);
		}else{
			return buscar(R->l, v2);
		}
	}else{
		return NULL;
	}
}

void tprint(struct tnode *r){
	if(r != NULL){
		tprint(r->l);
		printf("%p %d %p %p \n", r, r->v, r->l, r->r);
		tprint(r->r);
	}
}

void tremove(struct tnode *r){
	if(r != NULL){
		tremove(r->l);
		tremove(r->r);
		free(r);
	}
}
