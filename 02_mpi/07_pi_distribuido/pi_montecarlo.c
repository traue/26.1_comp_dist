#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Total de pontos: passado como argumento, ou padrao de 100 milhoes
    long long total_pontos = 100000000LL;
    if (argc > 1) {
        total_pontos = atoll(argv[1]);
    }

    // Broadcast para garantir que todos tenham o mesmo valor,
    // mesmo que o argumento so tenha sido passado ao processo 0.
    MPI_Bcast(&total_pontos, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Cada processo pega sua fatia. O rank 0 absorve qualquer sobra.
    long long pontos_locais = total_pontos / size;
    if (rank == 0) {
        pontos_locais += total_pontos % size;
    }

    // Identifica em que maquina o processo esta rodando
    char mpi_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(mpi_name, &name_len);
    printf("Rank %d em %s: sorteando %lld pontos\n",
           rank, mpi_name, pontos_locais);
    fflush(stdout);

    // Semente diferente por processo, senao todos sorteiam os mesmos pontos
    unsigned int seed = (unsigned int)(time(NULL) + rank * 7919);
    srand(seed);

    MPI_Barrier(MPI_COMM_WORLD);
    double t_inicio = MPI_Wtime();

    long long dentro_local = 0;
    for (long long i = 0; i < pontos_locais; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            dentro_local++;
        }
    }

    double t_fim_calculo = MPI_Wtime();

    // Soma os acertos de todos os processos no rank 0
    long long dentro_total = 0;
    MPI_Reduce(&dentro_local, &dentro_total, 1, MPI_LONG_LONG,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double t_fim_total = MPI_Wtime();

    if (rank == 0) {
        double pi_estimado = 4.0 * (double)dentro_total / (double)total_pontos;
        double erro = fabs(pi_estimado - M_PI);

        printf("\n--- Resultado ---\n");
        printf("Processos:            %d\n", size);
        printf("Pontos totais:        %lld\n", total_pontos);
        printf("Pontos dentro:        %lld\n", dentro_total);
        printf("Pi estimado:          %.10f\n", pi_estimado);
        printf("Pi real (M_PI):       %.10f\n", M_PI);
        printf("Erro absoluto:        %.2e\n", erro);
        printf("Tempo de calculo:     %.3f s\n", t_fim_calculo - t_inicio);
        printf("Tempo com reduce:     %.3f s\n", t_fim_total - t_inicio);
    }

    MPI_Finalize();
    return 0;
}
