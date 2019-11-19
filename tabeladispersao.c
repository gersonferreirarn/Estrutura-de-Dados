#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

struct valor{
	int v;
	struct valor *prox;
};

struct deus{
	int m;
	int n;
	struct valor **tabela;
};

int f(int x, struct deus *mestre);
struct deus * inserir_na_tabela(int x, struct deus *mestre);
void nullo(struct valor **tabela, int n);
void mostrar_tabela(struct deus *mestre);
int buscar_valor(struct deus *mestre, int x);
void limpar_tabela(struct deus *mestre, int nn);
struct valor ** preencher_nova_tabela(struct deus *mestre, struct valor **tabela_nova);

#define MICRO_PER_SECOND 1000000 

int main(){

	struct timeval start_time, stop_time;
	FILE *arquivo;
	float media = 0.0, time_diff;
	int repeticoes_buscas = 10000; 
	srand(time(NULL));
	struct deus *mestre = (struct deus*)malloc(sizeof(struct deus));
	mestre->m=2; // m
	mestre->n=0; // n
	mestre->tabela=NULL;
	
		for(int i=100; i<=10000; i+=100){
	        mestre = inserir_na_tabela(1 + ( rand() % mestre->m ), mestre);
	        arquivo = fopen("tabeladispersao_caso_medio.txt", "a");
		    media = 0.0;
	        for(int j=0; j<repeticoes_buscas; j++){
			    gettimeofday(&start_time, NULL);
			    buscar_valor(mestre, 1 + ( rand() % mestre->m ));
			    gettimeofday(&stop_time, NULL);
                time_diff = (stop_time.tv_sec + stop_time.tv_usec/1000000.0)
                          - (start_time.tv_sec + start_time.tv_usec/1000000.0);
			    media += time_diff;
			}
			printf("  - %d Concluido - Gastou %.20f\n", i, media/repeticoes_buscas);
			fprintf(arquivo, "%d %.20f\n", i, media/repeticoes_buscas);
			fclose(arquivo);
		}

	return 0;
}

int f(int x, struct deus *mestre){ // tabela dispersao
	return x % mestre->m;
}

void nullo(struct valor **tabela, int n){ // tabela dispersao
	for(int i=0; i<n; i++){
		tabela[i] = NULL;
	}
}

struct valor ** preencher_nova_tabela(struct deus *mestre, struct valor **tabela_nova){ // tabela dispersao
	struct valor *aux;
	
	for(int i=0; i<mestre->m/2; i++){
		if(mestre->tabela[i] != NULL){
			
			int posicao = f(mestre->tabela[i]->v, mestre);
			struct valor *novo = (struct valor*)malloc(sizeof(struct valor));
			novo->v=mestre->tabela[i]->v;
			novo->prox=tabela_nova[posicao];
			tabela_nova[posicao] = novo;
			
			aux = mestre->tabela[i]->prox;
			
			while(aux != NULL){
				int posicao = f(mestre->tabela[i]->v, mestre);
				struct valor *novo = (struct valor*)malloc(sizeof(struct valor));
				novo->v=mestre->tabela[i]->v;
				novo->prox=tabela_nova[posicao];
				tabela_nova[posicao] = novo;
				
				aux = aux->prox;
			}
		}
	}
	
	return tabela_nova;
}

struct deus * inserir_na_tabela(int x, struct deus *mestre){ // tabela dispersao
	if(mestre->tabela == NULL){
		mestre->tabela = (struct valor**)malloc(mestre->m*sizeof(struct valor *));
		nullo(mestre->tabela, mestre->m);
	}else{
		if(mestre->m-mestre->n <= 1){
			// cria novo vetor e copia do velho para o novo
			mestre->m = mestre->m*2; // duplica o valor de m
			//n[1]=0;
			struct valor **tabela_nova = (struct valor**)malloc(mestre->m*sizeof(struct valor *));
			nullo(tabela_nova, mestre->m);
			tabela_nova = preencher_nova_tabela(mestre, tabela_nova);
			limpar_tabela(mestre, mestre->m/2);
			mestre->tabela = tabela_nova;
		}
	}
	
	int posicao = f(x, mestre);
	struct valor *novo = (struct valor*)malloc(sizeof(struct valor));
	novo->v=x;
	novo->prox=mestre->tabela[posicao];
	mestre->tabela[posicao] = novo;
	mestre->n++;
	
	return mestre;
}

void limpar_tabela(struct deus *mestre, int nn){
	for(int i=0; i<nn; i++){
		while(mestre->tabela[i] != NULL){
			struct valor *aux = mestre->tabela[i];
			mestre->tabela[i] = mestre->tabela[i]->prox;
			free(aux);
		}
		//free(tabela[i]);
	}
	free(mestre->tabela);
}

void mostrar_tabela(struct deus *mestre){
	struct valor *aux;
	for(int i=0; i<mestre->m; i++){
		if(mestre->tabela[i] != NULL){
			aux=mestre->tabela[i];
			int tab = 0;
			while(aux != NULL){
				for(int k=0; k<tab*2; k++){
					printf(" ");
				}
				printf("[%d]: %d\n", i, aux->v);
				aux=aux->prox;
				tab++;
			}
		}
	}
}


int buscar_valor(struct deus *mestre, int x){
	int posicao = f(x, mestre);
	struct valor *aux;
	
	if(mestre->tabela[posicao] != NULL){
		if(mestre->tabela[posicao]->v == x){
			return 1;
		}else{
			aux = mestre->tabela[posicao]->prox;
			struct valor *anterior = mestre->tabela[posicao];
			while(aux != NULL && aux->v != x){
				anterior = aux;
				aux = aux->prox;
			}
			
			if(aux != NULL){
				anterior->prox = aux->prox;
				return 1;
			}else{
				return 0;
			}
		}
	}else{
		return 0;
	}
}
