#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define ITERACOES 10000

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

    char mpi_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(mpi_name, &name_len);
    printf("Rank %d em %s\n", rank, mpi_name);
    fflush(stdout);

    char buffer = 0;
    int parceiro = (rank == 0) ? 1 : 0;

    MPI_Barrier(MPI_COMM_WORLD);
    double inicio = MPI_Wtime();

    for (int i = 0; i < ITERACOES; i++) {
        if (rank == 0) {
            MPI_Send(&buffer, 1, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD);
            MPI_Recv(&buffer, 1, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(&buffer, 1, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            MPI_Send(&buffer, 1, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD);
        }
    }

    double fim = MPI_Wtime();

    if (rank == 0) {
        double tempo_total = fim - inicio;
        double rtt_us = (tempo_total / ITERACOES) * 1e6;
        double latencia_us = rtt_us / 2.0;

        printf("\n--- Resultado ---\n");
        printf("Iteracoes:            %d\n", ITERACOES);
        printf("Tempo total:          %.3f s\n", tempo_total);
        printf("RTT medio:            %.3f us\n", rtt_us);
        printf("Latencia (RTT/2):     %.3f us\n", latencia_us);
    }

    MPI_Finalize();
    return 0;
}
