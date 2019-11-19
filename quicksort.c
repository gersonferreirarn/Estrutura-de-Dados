#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define MICRO_PER_SECOND 1000000 

int *cria_vetor(int n, int ord);
void quick_sort(int *v, int s, int e);
int particao(int *v, int s, int e);
int exists(int *v, int n, int x);
int *fill(int *v, int s, int e);

int main(){
	int tam_vetor=100;
	int *vetor;
	struct timeval start_time, stop_time;
	FILE *arquivo = fopen("quicksort_aleatorio_caso.txt", "a"); // o pior caso eh quando ta ordenado
	float media = 0.0, time_diff;
	int repeticoes_vetor = 100; // faz 100 vezes para cada tamanho de vetor para realizar a media
	int modo_vetor = 3; // 0 ordenado crescente, 1  decrescente, 2 vetor zerado, 3 aleatorio; ord = 4, vetor fill para melhor caso do quicksort
	
	do{
		for(int i=0; i<repeticoes_vetor; i++){ 
			vetor = cria_vetor(tam_vetor, modo_vetor);
			gettimeofday(&start_time, NULL);
			quick_sort(vetor, 0, tam_vetor-1);
			gettimeofday(&stop_time, NULL);
			time_diff = (float)(stop_time.tv_sec - start_time.tv_sec);
			time_diff += (stop_time.tv_usec - start_time.tv_usec)/(float)MICRO_PER_SECOND;
			media += time_diff;
			free(vetor);
		}
		fprintf(arquivo, "%d %.10f\n", tam_vetor, media/repeticoes_vetor);
		printf("- %d Concluido! Gastou %.10f\n", tam_vetor, media/repeticoes_vetor);
		tam_vetor+=200;
		media = 0.0;
	}while(tam_vetor <=10000);
	
	fclose(arquivo);
	return 0;
}

int exists(int *v, int n, int x){ // verifica se existe numero repetido no vetor
    for (int i = 0; i < n; i++) {
        if (v[i] == x)
            return 1;
    }

    return 0;
}

int *cria_vetor(int n, int ord){ // ord = 0, crescente; ord = 1, decrescente; ord = 2, vetor zerado; ord = 3, vetor aleatorio; ord = 4, vetor fill para melhor caso do quicksort
	int * vetor = (int *) calloc (n, sizeof (int)); // calloc aloca vetor de n elementos, onde cada em 4(sizeof(int)) bytes

	if(ord == 0){ // vetor crescente
		for(int i = 0; i < n; i++){
			vetor[i] = i+1;
		}
	}else if(ord == 1){ // vetor decrescente
		for(int i = n; i!=0; i--){
			vetor[n-i]= i;
		}
	}else if(ord == 2){ // vetor zerado
		for(int i = n; i!=0; i--){
			vetor[i] = 0;
		}
	}else if(ord == 3){ // vetor aleatorio
		srand(time(NULL));
		int count = 0, x;
		while(count < n){
			x = 1 + ( rand() % n );
		    if (exists(vetor, n, x) == 0) {
		        vetor[count] = x;
		        count++;
		    }
		}
	}else{ // para melhor caso do quicksort
		for(int i = 0; i < n; i++){
			vetor[i] = i+1;
		}
		
		vetor = fill(vetor, 0, n-1);
	}

	return vetor;
}

int *fill(int* v, int s, int e){
	int m=(s+e)/2;
	if(e-s+1 == 3){
		v[e] = m+1;
		v[s]=m;
		v[s+1] = m+2;
	}else if(e-s+1){
		fill(v,s,m-1);
		fill(v,m+1,e);
		v[m]=v[e];
		v[e]= m+1;
		
	}
	return v;
}
		
void quick_sort(int *v, int s, int e){
	if (s<e) {
		int p = particao(v,s,e);
		
		quick_sort(v,s,p-1);
		quick_sort(v,p+1,e);
	}
}

int particao(int *v, int s, int e){
	int l = s;
	int aux;
	for(int i=s;i<e;i++){
		if(v[i]<v[e]){
			aux = v[i];		
			v[i] = v[l];
			v[l] = aux;
			l++;
		}
	}
	
	aux = v[e];
	v[e]= v[l] ;
	v[l]= aux;
	 
	return l;	
}
