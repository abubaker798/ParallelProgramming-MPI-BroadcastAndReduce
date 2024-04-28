#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char* argv[])
{
    int rank, np, Count = 0, total_count, x, y, r, i, j, start, end;

    /* Start up MPI */
    MPI_Init(&argc, &argv);

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &np);

       double Stime = MPI_Wtime();

        if (rank == 0) {
            scanf("%d %d", &x, &y);
            printf("\nMPI_Bcast and MPI_Reduce ONLY\n------------------\n-------------------------\n");

            if (np == 1) {

                r = (y - x) / (np);

                for (i = x; i <= y; i++) {
                    int isPrime = 1;

                    for (j = 2; j < i; j++) {
                        // if divisible for any number except it self or one, break loop
                        if (i % j == 0) {

                            isPrime = 0; break;
                        }

                    } if (isPrime == 1 && i > 1)
                        Count++;
                }

            }

            else

                r = (y - x) / (np - 1);
        }

        MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&y, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&r, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&start, 1, MPI_INT, 0, MPI_COMM_WORLD);
        start = r * (rank - 1) + x;
        MPI_Bcast(&end, 1, MPI_INT, 0, MPI_COMM_WORLD);
        end = r * (rank - 1) + r;

        if (rank == (np - 1))
        {
            end = y;
        }

        if (rank != 0) {

            //get prime number from range
            for (i = start; i <= end; i++) {
                int isPrime = 1;

                for (j = 2; j < i; j++) {
                    // if divisible for any number except it self or one, break loop
                    if (i % j == 0) {

                        isPrime = 0; break;
                    }

                } if (isPrime == 1 && i > 1)
                    Count++;
            }
            // printf("\np%d: calculate partial count of prime numbers from %d to %d -> Count = %d ", rank, start, end, Count);
        }

        MPI_Reduce(&Count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


        double Etime = MPI_Wtime();

        if (rank == 0) {
            printf("\nAfter reduction, P%d will have Count = %d \n", rank, total_count);
            printf("\nexecution times = %f \n", (Etime - Stime));
        }

    MPI_Finalize();
    return 0;
}
