#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct vertice{
	int v;
	struct vertice *prox;
};

int converter_string_int(char numero[]);
void mostrar_vetor(struct vertice** vetor, int tamanho, int *v);
int posicao_mapeado(int* v, int x, int n);
struct vertice** carregar_dados();

int main(){
	
	carregar_dados();
	
	return 0;
}

int converter_string_int(char numero[]){
    int convertido = 0;
    int inicio = 1;

    for(int i=1; numero[i] != '\0'; i++){
        inicio *= 10;
    }

    for(int j=0; numero[j] != '\0'; j++){
        convertido += (numero[j]-48) * inicio;

        inicio /= 10;
    }

    return convertido;
}

void mostrar_vetor(struct vertice** vetor, int tamanho, int *v){
	struct vertice* aux;
	
	printf("\nVetor:\n\n");
	
	for(int z=0; z<tamanho; z++){
		if(vetor[z] != NULL){
			printf("[%d] => %d", v[z], v[vetor[z]->v]);
			aux = vetor[z]->prox;
			while(aux != NULL){
				printf(" => %d", v[aux->v]);
				aux = aux->prox;
			}
			printf("\n");
		}else{
			printf("[%d] => NULL\n", v[z]);
		}
	}
}

int posicao_mapeado(int* v, int x, int n){
	for(int i=0; i<n; i++){
		if(v[i] == x){
			return i;
		}
	}
	
	return 0;
}

struct vertice** carregar_dados(){
    FILE *file_dados = fopen("dados.txt", "r");

    if(file_dados == NULL){
        printf("\nArquivo nao encontrado!\n");
        exit(1);
    }

    char dado[21];
	int passou = 0; // nao passou pelo #
	int par = 0;
	int tamanho = 0; // nao conta com o zero
	struct vertice **vetor;
	int primeiro, segundo;
	int *mapeamento = (int *) malloc(sizeof(int) * 1); // mapeamento dos numeros informados para organizar em numero sequencial (0 .. n)
	
    while(fscanf(file_dados, "%s", dado) != EOF){
        if(strcmp(dado, "#") == 0){ // passando agora pelo #
        	passou = 1;
        	vetor = (struct vertice**)malloc((tamanho)*sizeof(struct vertice *));
        	
        	for(int i=0; i<tamanho; i++)
				vetor[i] = NULL;
				
        }else if(!passou){// se nao tiver passado pelo #
			mapeamento =(int *) realloc(mapeamento, (tamanho+1)*sizeof(int));
			mapeamento[tamanho] = converter_string_int(dado);
        	tamanho++;
        }else{ // se tiver passado pelo #
        	if(par < 1){
				primeiro = posicao_mapeado(mapeamento, converter_string_int(dado), tamanho);
        		
        		//printf("%d e ", converter_string_int(dado));
        		
        		par++;
        	}else{
				segundo = posicao_mapeado(mapeamento, converter_string_int(dado), tamanho);
				struct vertice *novo = (struct vertice*)malloc(sizeof(struct vertice));
				novo->v = segundo;
				novo->prox = NULL;
				
				if(vetor[primeiro] == NULL){
					vetor[primeiro] = novo;
				}else{
					struct vertice* aux = vetor[primeiro];
					vetor[primeiro] = novo;
					vetor[primeiro]->prox = aux;
				}
        		
        		//printf("%d\n", converter_string_int(dado));
        		
        		par = 0;
        	}
        }
    }

    fclose(file_dados);
    
    mostrar_vetor(vetor, tamanho, mapeamento);
    
    return vetor;
}
