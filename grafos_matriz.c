#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int converter_string_int(char numero[]);
int** criar_matriz(int Linhas,int Colunas);
void mostra_matriz(int** matriz, int Linhas, int Colunas, int *v);
int posicao_mapeado(int* v, int x, int n);
int** carregar_dados();

int main(){
	int **matriz;
	matriz = carregar_dados();
	
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

int** criar_matriz(int Linhas,int Colunas){
	int i,j;
	int **m = (int**)malloc(Linhas * sizeof(int*));
	
	for (i = 0; i < Linhas; i++){ 
		m[i] = (int*) malloc(Colunas * sizeof(int));
		for (j = 0; j < Colunas; j++){
			m[i][j] = 0;
		}
	}
	
	return m;
}

void mostra_matriz(int** matriz, int Linhas, int Colunas, int *v){
	
	printf("\n Matriz: \n");
	for(int i=0; i<Linhas; i++){
		printf("  [%d]\t", v[i]);
		for(int j=0; j<Colunas; j++){
			printf(" %d", matriz[i][j]);
		}
		printf("\n");
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

int** carregar_dados(){
    FILE *file_dados = fopen("dados.txt", "r");

    if(file_dados == NULL){
        printf("\nArquivo nao encontrado!\n");
        exit(1);
    }

    char dado[21];
	int passou = 0; // nao passou pelo #
	int par = 0;
	int linhas = 0;
	int **matriz;
	int primeiro, segundo;
	int *mapeamento = (int *) malloc(sizeof(int) * 1); // mapeamento dos numeros informados para organizar em numero sequencial (0 .. n)
	
    while(fscanf(file_dados, "%s", dado) != EOF){
        if(strcmp(dado, "#") == 0){ // passando agora pelo #
        	passou = 1;
        	matriz = criar_matriz(linhas+1, linhas+1);
        	
        	for(int i=0; i<linhas; i++){ // zera todos os elementos da matriz
				for(int j=0; j<linhas; j++){
					matriz[i][j] = 0;
				}
			}
        }else if(!passou){// se nao tiver passado pelo #
			mapeamento =(int *) realloc(mapeamento, (linhas+1)*sizeof(int));
			mapeamento[linhas] = converter_string_int(dado);
        	linhas++;
        	
        }else{ // se tiver passado pelo #
        	if(par < 1){
				primeiro = posicao_mapeado(mapeamento, converter_string_int(dado), linhas);
        		
        		//printf("%d e ", converter_string_int(dado));
        		
        		par++;
        	}else{
				segundo = posicao_mapeado(mapeamento, converter_string_int(dado), linhas);
        		matriz[primeiro][segundo] = 1;
        		
        		//printf("%d\n", converter_string_int(dado));
        		
        		par = 0;
        	}
        }
    }

    fclose(file_dados);
    
    mostra_matriz(matriz, linhas, linhas, mapeamento);
    
    return matriz;
}
