
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define DIMENSION 3

int number_of_points;
int number_of_clusters;
int modified, number_of_processors;

double *points;
double *mean;
int *cluster;

void *calculate_clusters(void *arg) {
    int index = *(int *) arg;

    int i, j, d;
    double min_distance, cur_distance, sum;
    int min_index;

    for (i = index; i < number_of_points; i += number_of_processors) {
        sum = 0;
        for (d = 0; d < DIMENSION; d++) {
            sum += ((points[i * DIMENSION + d] - mean[d]) * (points[i * DIMENSION + d] - mean[d]));
        }
        min_distance = sum;
        min_index = 0;
        for (j = 1; j < number_of_clusters; j++) {
            sum = 0;
            for (d = 0; d < DIMENSION; d++) {
                sum += ((points[i * DIMENSION + d] - mean[j * DIMENSION + d]) * (points[i * DIMENSION + d] - mean[j * DIMENSION + d]));
            }
            cur_distance = sum;

            if (cur_distance < min_distance) {
                min_distance = cur_distance;
                min_index = j;
            }
        }

        if (cluster[i] != min_index) {
            cluster[i] = min_index;
            modified = 1;
        }
    }

    return (void *) 0;
}


void *calculate_means(void *arg) {
    int index = *(int *) arg;
    int i, j, d, count;
    double *sum = (double *) malloc(sizeof(double) * number_of_clusters * DIMENSION);

    for (i = index; i < number_of_clusters; i += number_of_processors) {
        memset(sum, 0, DIMENSION * sizeof(double));
        count = 0;

        for (j = 0; j < number_of_points; j++) {
            if (cluster[j] == i) {
                for (d = 0; d < DIMENSION; d++) {
                    sum[d] += points[j * DIMENSION + d];
                }
                count++;
            }
        }

        for (d = 0; d < DIMENSION; d++) {
            if (count != 0) {
                mean[i * DIMENSION + d] = sum[d] / count;
            }
        }
    }
    free(sum);

    return (void *) 0;
}

int main() {
    int i, j;

    // number of mean
    scanf("%d", &number_of_clusters);

    // number of points
    scanf("%d", &number_of_points);

    if ((number_of_processors = sysconf(_SC_NPROCESSORS_ONLN)) <= 0) {
        perror("ERROR: Number of processors is less than 0\n");
    }
    if (getenv("MAX_CPUS")) {
        number_of_processors = atoi(getenv("MAX_CPUS"));
    }
    if (number_of_processors > number_of_clusters) number_of_processors = number_of_clusters;

    // initial centroids
    mean = (double *) malloc(sizeof(double) * DIMENSION * number_of_clusters);
    for (i = 0; i < number_of_clusters; i++)
        scanf("%lf %lf %lf", mean + i * DIMENSION, mean + i * DIMENSION + 1, mean + i * DIMENSION + 2);


    // initial points
    cluster = (int *) malloc(sizeof(int) * number_of_points);
    memset(cluster, -1, sizeof(int) * number_of_points);

    points = (double *) malloc(sizeof(double) * DIMENSION * number_of_points);
    for (i = 0; i < number_of_points; i++)
        scanf("%lf %lf %lf", points + i * DIMENSION, points + i * DIMENSION + 1, points + i * DIMENSION + 2);


    pthread_t th[2];
    int *tid = (int *) malloc(sizeof(int) * number_of_processors);

    modified = 1;
    while (modified) {
        modified = 0;

        for (i = 0; i < number_of_processors; i++) {
            tid[i] = i;
            if (pthread_create(&th[i], NULL, calculate_clusters, &tid[i]) != 0) {
                perror("ERROR: Pthread create error\n");
            }
        }

        for (i = 0; i < number_of_processors; i++) {
            pthread_join(th[i], NULL);
        }

        for (i = 0; i < number_of_processors; i++) {
            tid[i] = i;
            if ((pthread_create(&th[i], NULL, calculate_means, &tid[i])) != 0) {
                perror("ERROR: Pthread create error\n");
            }
        }

        for (i = 0; i < number_of_processors; i++) {
            pthread_join(th[i], NULL);
        }
    }

    for (i = 0; i < number_of_clusters; i++) {
        for (j = 0; j < DIMENSION; j++)
            printf("%5.2f ", mean[i * DIMENSION + j]);
        printf("\n");
    }

    free(points);
    free(mean);
    free(cluster);

    return 0;
}