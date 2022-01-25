#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct cabecalho{
	char tipo[3];
	int linha;
	int coluna;
	int tom_de_cinza_maximo;
}header;

void menu();
void info_header(FILE *fp, header *head);
void pula_duas_linhas(char *string, FILE *fp, header *head);
void resolucao(char *string, FILE *fp, header *head);
int pega_maior_tom_d_cinza(char *string, FILE *fp);
int **aloca_matriz(header *head);
int **copia_matriz(FILE *fp, header *head);
void limiarizacao(int **mat, FILE *fp, header head);
void negativo(int **mat, FILE *fp, header head);
void histograma(int **mat, FILE *fp, header head);
void cola_header(FILE *fp, header head);

int main ()
{
	char nome_arquivo[21];
	FILE *fp;
	int **matriz;
	header head;
	int op;
	
	menu(); //printa o menu no console
	
	do{
		printf("|>|Escolha uma opcao: ");
		scanf(" %d", &op);
			fflush(stdin);
		switch (op){
		case 1:
			break;
		case 0:
			printf("Encerrando...");
			return 0;
			break;
		default:
			printf("Opcao invalida\n");
			break;
		}
	}while(op != 1 && op != 0); //do-while pro usuario escolher a opção
	
	printf("|>|Insira o nome do arquivo no formato \"exemplo.pgm\": ");	//aqui o arquivo é escolhido.
	gets(nome_arquivo);
	fp = fopen(nome_arquivo, "r");
	
		if (fp == NULL){
			printf("\nErro ao abrir o arquivo, encerrando...");
			return 0;
		}
		
		info_header(fp, &head);
		matriz = copia_matriz(fp, &head);
	fclose(fp);
	
	limiarizacao(matriz, fp, head);
	negativo(matriz, fp, head);
	histograma(matriz, fp, head);
	
	printf("|>|================= Operacoes feitas ==================|<|\n");
	printf("|>|Encerrando...");
	getch();
	return 0;
}

void menu()
{
	char op;
	
	printf("|>|=====================================================|<|\n");
	printf("|>|===================== Projeto 1 =====================|<|\n");
	printf("|>|Opcoes: \n");
	printf("|>|(1): Faz as operacoes no arquivo .pgm\n");
	printf("|>|(0): Termina o programa\n");
}

void info_header(FILE *fp, header *head)
{
	char string[70];
	
	pula_duas_linhas(string, fp, head); //pula duas linhas e copia o tipo do arquivo
	resolucao(string, fp, head); //pega a resolução da linha e coluna
	head->tom_de_cinza_maximo = pega_maior_tom_d_cinza(string, fp);
}

void pula_duas_linhas(char *string, FILE *fp, header *head)
{
	fgets(string, 70, fp);
		strcpy(head->tipo, string);
	fgets(string, 70, fp);
}

void resolucao(char *string, FILE *fp, header *head)
{
	char *string2;
	long int aux;
	
	//se a imagem tiver aquela assinatura q começa com #, ent entra nesse if
	if (string[0] == '#')
		fgets(string, 70, fp);	//dá outro fgets() pro ponteiro string pegar a string das resoluções
	//caso contrário, string já tem a string com as resoluções
	
	aux = strtol(string, &string2, 10);
		head->coluna = aux; //tamanho da coluna
	aux = strtol(string2, NULL, 10);
		head->linha = aux; //tamanho da linha
}

int pega_maior_tom_d_cinza(char *string, FILE *fp)
{
	int aux;
	
	fgets(string, 70, fp); //copia a string q tem o maior tom de cinza
	aux = atoi(string); //passa pra inteiro
	
	return aux;
}

int **aloca_matriz(header *head)
{
	int i;
	int **mat;
	
	mat = (int **) malloc(head->linha * sizeof(int *));
	for (i = 0; i < head->linha ; i++)
		mat[i] = (int *) malloc(head->coluna * sizeof(int));
	
	return mat;
}

int **copia_matriz(FILE *fp, header *head)
{
	char string[4], *ptr;
	long int aux;
	int i = 0, j = 0;
	int **mat;
		mat = aloca_matriz(head);
	
	while(!feof(fp)){
		fscanf(fp, "%s", string);
		ptr = string;
		
		//se j chegar no final da linha
		if (j == head->coluna){
			j = 0; //volta o j pro index 0
			i++; //vai pra próxima linha
		}
		//verifica se chegou no ultimo espaço da matriz
		if(i == head->linha)
			break;
		
		aux = strtol(ptr, &ptr, 10); //converte o numero da string pra inteiro
		//aux = atoi(ptr); //por algum motivo, n sei qual, o programa rodou mais rápido convertendo a string com o strtol();
		
		mat[i][j] = aux; //copia o valor de cada pixel na matriz
		j++;
		
		if(i == head->linha && j == head->coluna)
			break;
	}
	return mat;
}

void limiarizacao(int **mat, FILE *fp, header head)
{
	int i, j;
	int **limiar;
		limiar = aloca_matriz(&head);
	
	for (i = 0; i < head.linha; i++)
		for (j = 0; j < head.coluna; j++)
			limiar[i][j] = mat[i][j];
	
	fp = fopen("limiarizacao.pgm", "w");
		for (i = 0; i < head.linha; i++){
			for (j = 0; j < head.coluna; j++){
				if (limiar[i][j] > 150)
					limiar[i][j] = 255;
				else
					limiar[i][j] = 0;
			}
		}
		//printa tudo no arquivo
		cola_header(fp, head);
		for (i = 0; i < head.linha; i++)
			for (j = 0; j < head.coluna; j++)
				fprintf(fp, "%d ", limiar[i][j]);
	fclose(fp);
}

void negativo(int **mat, FILE *fp, header head)
{
	int i, j;
	int **negativo;
		negativo = aloca_matriz(&head);
	
	for (i = 0; i < head.linha; i++)
		for (j = 0; j < head.coluna; j++)
			negativo[i][j] = mat[i][j]; //copia uma matriz na outra
	
	fp = fopen("negativo.pgm", "w");
		//converte pra negativo
		for (i = 0; i < head.linha; i++){
			for (j = 0; j < head.coluna; j++){
				negativo[i][j] = 255 - negativo[i][j];
			}
		}
		//printa tudo no arquivo
		cola_header(fp, head);
		for (i = 0; i < head.linha; i++)
			for (j = 0; j < head.coluna; j++)
				fprintf(fp, "%d ", negativo[i][j]);
	fclose(fp);
}

void histograma(int **mat, FILE *fp, header head)
{
	int pixel[head.tom_de_cinza_maximo + 1];
	int i, j, aux;
	
	for (i = 0; i < head.tom_de_cinza_maximo + 1; i++)
		pixel[i] = 0; //zera todo o vetor contador dos pixels
	
	for (i = 0; i < head.linha; i++){
		for (j = 0; j < head.coluna; j++){
			aux = mat[i][j];
			pixel[aux]++;
		}
	}
	
	//abre e printa o vetor do histograma no arquivo
	fp = fopen("histograma.txt", "w");
		for (i = 0; i < head.tom_de_cinza_maximo + 1; i++)
			fprintf(fp, "pixel [%d]: %d\n", i, pixel[i]);
	fclose(fp);
}

void cola_header(FILE *fp, header head)
{
	fprintf(fp, "%s\n%d %d\n%d\n", head.tipo, head.coluna, head.linha, head.tom_de_cinza_maximo);
}

