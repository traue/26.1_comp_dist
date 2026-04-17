#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char hostname[256];
    gethostname(hostname, sizeof(hostname));

    int name_len;
    char mpi_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(mpi_name, &name_len);

    printf("Processo %d de %d rodando em %s (MPI: %s)\n",
           rank, size, hostname, mpi_name);

    MPI_Finalize();
    return 0;
}
