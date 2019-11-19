#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

struct tnode *new_no(int v);
struct tnode *buscar(struct tnode *R, int v2);
int max(int a, int b);
void inserir(struct tnode **R, int v2);
void rotacao_direita(struct tnode *x);
void rotacao_esquerda(struct tnode *x);
void rotacao_esquerda_direita(struct tnode *p);
void rotacao_direita_esquerda(struct tnode *p);
void updatet(struct tnode *n, struct tnode** R);
void tprint(struct tnode *r);
void tremove(struct tnode *r);
void re(struct tnode *x);
int valor_absoluto(struct tnode* a, struct tnode* b);
int altura1(struct tnode *n);
int altura2(struct tnode *n);

#define MICRO_PER_SECOND 1000000 

struct tnode{
	int v, h; // h é altura
	struct tnode *l;
	struct tnode *r;
	struct tnode *p; // pai
};

int main(){
	struct timeval start_time, stop_time;
	FILE *arquivo = fopen("arvorebalanceada_caso_medio.txt", "a");
	float media = 0.0, time_diff;
	int repeticoes_vetor = 0; 
	int n=100; // numero de elementos
	//srand(time(NULL));
	do{
		repeticoes_vetor = n;
		struct tnode *R;
		R = NULL;
		for(int i=1; i<n; i++){
			int x;
			do{
				x = 1 + ( rand() % n*10 );
				//if(x%2 != 0)
					//x += 1;
			}while(buscar(R, x) != NULL);
				
			inserir(&R, x);
		}
		
		//tprint(R);
		
		for(int i=0; i<repeticoes_vetor; i++){ 
			gettimeofday(&start_time, NULL);
			int x_buscar = ((1 + ( rand() % n*10 ))); // caso medio
			
			//int x_buscar = 1 + ( rand() % n*10 ); // pior caso
			//if(x_buscar%2 == 0) // pior caso
				//x_buscar -= 1; // pior caso
			
			buscar(R, x_buscar);
			gettimeofday(&stop_time, NULL);
			time_diff = (float)(stop_time.tv_sec - start_time.tv_sec);
			time_diff += (stop_time.tv_usec - start_time.tv_usec)/(float)MICRO_PER_SECOND;
			media += time_diff;
		}
		fprintf(arquivo, "%d %.20f\n", n, media/repeticoes_vetor);
		printf("- %d Concluido! Gastou %.20f\n", n, media/repeticoes_vetor);
		media = 0.0;
		n+=100;
		tremove(R);
	}while(n <= 10000);
	
	fclose(arquivo);
	return 0;
}

int max(int a, int b){
	if(a>b){
		return a;
	}else{
		return b;
	}
}

struct tnode *new_no(int v){
	struct tnode *n = (struct tnode*)malloc(sizeof(struct tnode));
	n->v=v;
	n->h=1;
	n->l=NULL;
	n->r=NULL;
	return n;
}

void inserir(struct tnode **R, int v2){
	struct tnode *p, *f, *w=new_no(v2);
	if(*R == NULL){
		(*R) = w;
	}else{
		p = (*R);
		while(p != NULL){
			f=p;
			//if(p->v == v2)
				//return;
			if(p->v > w->v)
				p = p->l;
			else
				p = p->r;
		}
		
		w->p = f;
		if(w->v < f->v){
			f->l = w;
		}else{
			f->r = w;
		}
		updatet(f, R);
	}
}

int valor_absoluto(struct tnode* a, struct tnode* b){
	if(a != NULL && b != NULL){
		if(a->h-b->h < 0){
			return (a->h-b->h)*(-1);
		}else{
			return (a->h-b->h);
		}
	}else{
		if(a!=NULL){
			if(a->h>=0){
				return a->h;
			}else{
				return a->h*(-1);
			}
		}else if(b!=NULL){
			if(b->h>=0){
				return b->h;
			}else{
				return b->h*(-1);
			}
		}else{
			return 0;
		}
	}
	
}

void rotacao_direita(struct tnode *x){
	struct tnode *y = x->l;
	struct tnode *b = y->r;
	
	if(x->p != NULL){
		if(x->p->l == x)
			x->p->l = y;
		else
			x->p->r=y;
	}
	
	y->p=x->p;
	x->p=y;
	y->r=x;
	if(b != NULL){
		b->p=x;
	}
	x->l=b;
	
	x->h = altura1(x);
	y->h = altura1(y);
}

void rotacao_esquerda(struct tnode *x){
	struct tnode *y = x->r;
	struct tnode *b = y->l;
	
	if(x->p != NULL){
		if(x->p->r == x)
			x->p->r = y;
		else
			x->p->l=y;
	}
	
	y->p=x->p;
	x->p=y;	
	y->l=x;
	if(b != NULL){
		b->p=x;
	}
	x->r=b;
	
	x->h = altura1(x);
	y->h = altura1(y);
}

void rotacao_esquerda_direita(struct tnode *p){
	rotacao_esquerda(p->l);
	rotacao_direita(p);
}

void rotacao_direita_esquerda(struct tnode *p){
	rotacao_direita(p->r);
	rotacao_esquerda(p);
}

int altura1(struct tnode *n){
	int h = 0;
	if(n->r != NULL && n->l != NULL)
		h = max(n->r->h, n->l->h)+1;
	else
		if(n->r != NULL)
			h = (n->r->h) +1;
		else
			if(n->l != NULL)
				h = (n->l->h) +1;
			else
				h = 0;	
				
	return h;
}
// hd > 1 ou 3
int altura2(struct tnode *n){
	// -1 -> esquerda maior
	// 0 -> iguais
	// 1 -> direita maior
	int h = 0;
	
	if(n->r == NULL && n->l == NULL)
		h = 0;
	else if(n->r == NULL){
		h = -1;
	}else if(n->l == NULL){
		h = 1;
	}else if(n->l->h == n->r->h){
		h = 0;
	}else{
		if(n->r->h > n->l->h){
			h = 1;
		}else{
			h = -1;
		}
	}
	
	return h;
}

void updatet(struct tnode *n, struct tnode** R){
	int h = altura1(n);
	//printf("- h = %d -\n", h);
	if(n->h != h){
	    n->h = h;
		if(valor_absoluto(n->r, n->l) >= 2){
				if(altura2(n) < 0){
					// esquerda maior
					if(altura2(n->l) < 0){
						// esquerda da esquerda maior
						//printf("\nCaso 2\n");
						rotacao_direita(n);
					}else{
						// direita da esquerda maior
						//printf("\nCaso 4\n");
						rotacao_esquerda_direita(n);
					}
				}else if(altura2(n) > 0){
					// direita maior
					if(altura2(n->r) > 0){
						// direita da direita maior
						//printf("\nCaso 1\n");
						rotacao_esquerda(n);
					}else{
						// esquerda da direita maior
						//printf("\nCaso 3\n");
						rotacao_direita_esquerda(n);
					}
				}else{
					// iguais ou nulos
				}
				
				if(*R == n){
					*R = n->p;
				}
		}
		//n->h = h;
		if(n->p != NULL){
			updatet(n->p, R);
		}
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
		printf("%p %d %d %p %p \n", r, r->v, r->h, r->l, r->r);
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
