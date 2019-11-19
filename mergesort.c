#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


#define MICRO_PER_SECOND 1000000 

void merge(int *v, int s, int m, int e);
int *cria_vetor(int n, int ord);
void merge_sort(int *v, int s, int e);

int main(){
	int tam_vetor = 100;
	int *vetor;
	struct timeval start_time, stop_time;
	FILE *arquivo = fopen("mergesort.txt", "a");
	float media = 0.0, time_diff;
	int repeticoes_vetor = 100; // faz 100 vezes para cada tamanho de vetor para realizar a media
	int modo_vetor = 1; // 0 ordenado crescente, 1  decrescente, 2 vetor zerado, 3 aleatorio
	
	do{
		for(int i=0; i<repeticoes_vetor; i++){
			vetor = cria_vetor(tam_vetor, modo_vetor);
			gettimeofday(&start_time, NULL);
			merge_sort(vetor, 0, tam_vetor);
			gettimeofday(&stop_time, NULL);
			time_diff = (float)(stop_time.tv_sec - start_time.tv_sec);
			time_diff += (stop_time.tv_usec - start_time.tv_usec)/(float)MICRO_PER_SECOND;
			media += time_diff;
			free(vetor);
		}
		fprintf(arquivo, "%d %.10f\n", tam_vetor, media/repeticoes_vetor);
		printf("- %d Concluido! Gastou %.10f\n", tam_vetor, media/repeticoes_vetor);
		tam_vetor += 200;
		media = 0.0;
	}while(tam_vetor <=10000);
	
	fclose(arquivo);
	return 0;
}

int *cria_vetor(int n, int ord){ // ord = 0, crescente; ord = 1, decrescente; ord = 2, vetor zerado; ord = 3, vetor aleatorio
	int * vetor = (int *) calloc (n, sizeof (int)); // calloc aloca vetor de n elementos, onde cada em 4(sizeof(int)) bytes

	if(ord == 0){ // vetor crescente
		for(int i = 0; i <= n; i++){
			vetor[i] = i;
		}
	}else if(ord == 1){ // vetor decrescente
		for(int i = n; i>=0; i--){
			vetor[n-i]= i;
		}
	}else if(ord == 2){ // vetor zerado
		for(int i = n; i>=0; i--){
			vetor[i] = 0;
		}
	}else{ // vetor aleatorio
		srand(time(NULL));
		for(int i = n; i>=0; i--){
			vetor[i] = 1 + ( rand() % n );
		}
	}

	return vetor;
}

void merge(int *v, int s, int m, int e){
	int i=s;
	int j=m+1;
	int k=1;
	int w[e-s+1];
	
	while(k<=(e-s+1)){
		if(v[i] < v[j] && i<=m || j>e){
			w[k] = v[i];
			i++;
		}else{
			w[k] = v[j];
			j++;
		}
	
		k++;
	}
	
	k=1;
		
	while(k<=(e-s+1)){
		v[s+k-1] = w[k];
		k++;
	}
}

void merge_sort(int *v, int s, int e){
	int m;
	if(s<e){
		m = (s+e)/2;
		merge_sort(v, s, m);
		merge_sort(v, m+1, e);
		merge(v,s,m,e);
	}
}
