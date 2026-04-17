#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITERACOES 1000
#define TAMANHO_MIN 1
#define TAMANHO_MAX (4 * 1024 * 1024)  // 4 MB

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            fprintf(stderr, "Este programa exige exatamente 2 processos.\n");
        }
        MPI_Finalize();
        return 1;
    }

    char *buffer = (char *)malloc(TAMANHO_MAX);
    if (!buffer) {
        fprintf(stderr, "Falha ao alocar buffer.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    memset(buffer, 0, TAMANHO_MAX);

    int parceiro = (rank == 0) ? 1 : 0;

    if (rank == 0) {
        printf("%12s %12s %15s %15s\n",
               "Tamanho (B)", "Iter", "Tempo (s)", "Banda (MB/s)");
        printf("--------------------------------------------------------\n");
    }

    for (int tam = TAMANHO_MIN; tam <= TAMANHO_MAX; tam *= 2) {
        MPI_Barrier(MPI_COMM_WORLD);
        double inicio = MPI_Wtime();

        for (int i = 0; i < ITERACOES; i++) {
            if (rank == 0) {
                MPI_Send(buffer, tam, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD);
                MPI_Recv(buffer, tam, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
            } else {
                MPI_Recv(buffer, tam, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
                MPI_Send(buffer, tam, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD);
            }
        }

        double fim = MPI_Wtime();

        if (rank == 0) {
            double tempo_total = fim - inicio;
            // 2 * ITERACOES * tam bytes trafegam no total (ida e volta)
            double bytes_totais = 2.0 * ITERACOES * tam;
            double banda_mbs = (bytes_totais / tempo_total) / (1024.0 * 1024.0);
            printf("%12d %12d %15.6f %15.2f\n",
                   tam, ITERACOES, tempo_total, banda_mbs);
        }
    }

    free(buffer);
    MPI_Finalize();
    return 0;
}
