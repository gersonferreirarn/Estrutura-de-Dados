#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define MICRO_PER_SECOND 1000000 

int *cria_vetor(int n, int ord);
int search(int *v, int x, int s, int e);
int exists(int *v, int n, int x);

int main(){
	int tam_vetor = 100;
	int *vetor;
	struct timeval start_time, stop_time;
	FILE *arquivo = fopen("buscabinaria_aleatorio_caso.txt", "a");
	float media = 0.0, time_diff;
	int repeticoes_vetor = 100; // faz 100 vezes para cada tamanho de vetor para realizar a media
	int modo_vetor = 0;  // ord = 0, crescente; ord = 1, decrescente; ord = 2, vetor zerado; ord = 3, vetor aleatorio
	int x;
	do{
		srand(time(NULL));
		for(int i=0; i<repeticoes_vetor; i++){
			vetor = cria_vetor(tam_vetor, modo_vetor);
			//x = vetor[tam_vetor/2]; // x=vetor[tam_vetor/2] para melhor caso
			//x = -1; // x=-1 pior caso
			x = 1 + ( rand() % tam_vetor ); // aleatorio caso
			gettimeofday(&start_time, NULL);
			search(vetor, x, 0, tam_vetor);
			gettimeofday(&stop_time, NULL);
			time_diff = (float)(stop_time.tv_sec - start_time.tv_sec);
			time_diff += (stop_time.tv_usec - start_time.tv_usec)/(float)MICRO_PER_SECOND;
			media += time_diff;
			free(vetor);
		}
		fprintf(arquivo, "%d %.20f\n", tam_vetor, media/repeticoes_vetor);
		printf(" - %d Concluido! Gastou %.20f \n", tam_vetor, media/repeticoes_vetor);
		tam_vetor += 100;
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


int *cria_vetor(int n, int ord){ // ord = 0, crescente; ord = 1, decrescente; ord = 2, vetor zerado; ord = 3, vetor aleatorio
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
	}else{ // vetor aleatorio
		srand(time(NULL));
		int count = 0, x;
		while(count < n){
			x = 1 + ( rand() % n );
		    if (exists(vetor, n, x) == 0) {
		        vetor[count] = x;
		        count++;
		    }
		}
	}

	return vetor;
}

int search(int *v, int x, int s, int e){ // so funciona para um vetor de ordenado crescente
	
	if(s<=e){
		int meio = (s+e)/2;
		if(v[meio] == x){

			return meio;
		}

		if(x > v[meio]){
			return(v, x, meio+1, e); 
		}
		return(v, x, s, meio-1);
		
	}
	return -1;
}
