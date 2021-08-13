#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <locale.h>

const int L = 3; /*Grau das Matrizes L x L */
/*srand(time(NULL));  Gerador da seed da função rand(), usada na sub-rotina preencher*/

void preencher(int matriz[][L]);
void exibir(int matriz[][L]);
void clear(int matriz[][L]);
int menor_diagonal(int A[][L], int B[][L], int C[][L], int AB[][L], int ABC[][L]);

int main() {
	setlocale(LC_ALL, "");
	clock_t t;
	double time_taken, media = 0;
	int A[L][L], B[L][L], C[L][L], AB[L][L], ABC[L][L], menor, i;

	/*Preenchimento das Matrizes A, B e C: */
	preencher(A);
	preencher(B);
	preencher(C);

	/*Exibição das Matrizes A, B e C: */
	printf("Matriz A:\n");
	exibir(A);
	printf("\n\nMatriz B:\n");
	exibir(B);
	printf("\n\nMatriz C:\n");
	exibir(C);

	/*Cálculos e exibição de A*B e A*B*C: */

	for (i = 0; i < 10; i++) {
		clear(AB);
		clear(ABC);

		t = clock();
		menor = menor_diagonal(A, B, C, AB, ABC);
		t = clock() - t;
		time_taken = ((double)t) / CLOCKS_PER_SEC; // em segundos
		media += time_taken;
		sleep(0.005);
	}

	media = media / i;
	printf("A funcao menor_diagonal()em C levou %f segundos, numa media de 10 testes, para executar\n", media);
	media = 0;
	printf("Menor valor da diagonal principal em C: %d", menor);

	extern int mulmat(int A[][L], int B[][L], int C[][L], int AB[][L], int ABC[][L], int L);

	for (i = 0; i < 10; i++) {
		clear(AB);
		clear(ABC);

		t = clock();
		menor = mulmat(A, B, C, AB, ABC, L);
		t = clock() - t;
		time_taken = ((double)t) / CLOCKS_PER_SEC; // em segundos
		media += time_taken;
		sleep(0.005);
	}

	media = media / i;
	printf("\n\nA funcao mulmat() em NASM levou %f segundos, numa media de 10 testes, para executar", media);
	printf("\nMenor valor da diagonal principal em Assembly: %d", menor);
	printf("\n\nMatriz A*B (resultado parcial):\n");
	exibir(AB);

	printf("\n\nMatriz A*B*C:\n");
	exibir(ABC);

	return 0;
}

void preencher(int matriz[][L]) { /*Preenche a matriz com a função rand() */
	int i, j, k = 0;
	for (i = 0; i < L; i++) {
		for (j = 0; j < L; j++) {
			matriz[i][j] = rand() % 10 + 1;
			k++;
		}
	}
}

void clear(int matriz[][L]) {
	int i, j;
	for (i = 0; i < L; i++) {
		for (j = 0; j < L; j++)
			matriz[i][j] = 0;
	}
}

void exibir(int matriz[][L]) {
	int j, i;
	for (j = 0; j < L; j++) {
		for (i = 0; i < L; i++)
			printf("%5d ", matriz[j][i]);
		printf("\n");
	}
}

int menor_diagonal(int A[][L], int B[][L], int C[][L], int AB[][L], int ABC[][L]) {
	int i, j, k, acum, menor;
	for (i = 0; i < L; i++) {
		for (j = 0; j < L; j++) {
			acum = 0;
			for (k = 0; k < L; k++)
				acum += A[i][k] * B[k][j];

			AB[i][j] = acum;
		}
	}

	for (i = 0; i < L; i++) {
		for (j = 0; j < L; j++) {
			acum = 0;
			for (k = 0; k < L; k++)
			{
				acum += AB[i][k] * C[k][j];
			}
			ABC[i][j] = acum;
		}
	}

	menor = ABC[0][0];
	for (i = 0; i < L; i++) {
		for (j = 0; j < L; j++) {
			if (ABC[i][j] < menor && i == j)
				menor = ABC[i][j];
		}
	}

	return menor;
}
