#include <stdio.h>
#include <stdlib.h>

#define DIMENSION 3

int main(void) {

    int number_of_clusters, number_of_points, c, i, j;
    double dmin, dx;
    double *points, *mean, *sum;
    int *cluster, *count, color;
    int flips;
    scanf("%d", &number_of_clusters);
    scanf("%d", &number_of_points);

    // initialization
    points = (double *) malloc(sizeof(double) * DIMENSION * number_of_points);
    mean = (double *) malloc(sizeof(double) * DIMENSION * number_of_clusters);
    sum = (double *) malloc(sizeof(double) * DIMENSION * number_of_clusters);
    cluster = (int *) malloc(sizeof(int) * number_of_points);
    count = (int *) malloc(sizeof(int) * number_of_clusters);

    for (i = 0; i < number_of_points; i++)
        cluster[i] = 0;
    for (i = 0; i < number_of_clusters; i++)
        scanf("%lf %lf %lf", mean + i * DIMENSION, mean + i * DIMENSION + 1, mean + i * DIMENSION + 2);
    for (i = 0; i < number_of_points; i++)
        scanf("%lf %lf %lf", points + i * DIMENSION, points + i * DIMENSION + 1, points + i * DIMENSION + 2);

    flips = number_of_points;
    while (flips > 0) {
        flips = 0;
        for (j = 0; j < number_of_clusters; j++) {
            count[j] = 0;
            for (i = 0; i < DIMENSION; i++)
                sum[j * DIMENSION + i] = 0.0;
        }
        for (i = 0; i < number_of_points; i++) {
            dmin = -1;
            color = cluster[i];
            for (c = 0; c < number_of_clusters; c++) {
                dx = 0.0;
                for (j = 0; j < DIMENSION; j++)
                    dx += (points[i * DIMENSION + j] - mean[c * DIMENSION + j]) * (points[i * DIMENSION + j] - mean[c * DIMENSION + j]);
                if (dx < dmin || dmin == -1) {
                    color = c;
                    dmin = dx;
                }
            }
            if (cluster[i] != color) {
                flips++;
                cluster[i] = color;
            }
        }

        for (i = 0; i < number_of_points; i++) {
            count[cluster[i]]++;
            for (j = 0; j < DIMENSION; j++)
                sum[cluster[i] * DIMENSION + j] += points[i * DIMENSION + j];
        }
        for (i = 0; i < number_of_clusters; i++) {
            for (j = 0; j < DIMENSION; j++) {
                mean[i * DIMENSION + j] = sum[i * DIMENSION + j] / count[i];
            }
        }
    }
    for (i = 0; i < number_of_clusters; i++) {
        for (j = 0; j < DIMENSION; j++)
            printf("%5.2f ", mean[i * DIMENSION + j]);
        printf("\n");
    }

    return (0);
}