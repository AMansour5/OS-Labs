#include <pthread.h>
#include "caltrain.h"

void station_init(struct station *station) {
    station->num_empty_seats = 0;
    station->num_waiting_passengers = 0;
    station->num_passengers_walking_on_train = 0;
    pthread_mutex_init(&station->mutex, NULL);
    pthread_cond_init(&station->train_available, NULL);
    pthread_cond_init(&station->passengers_done, NULL);
}

void station_load_train(struct station *station, int count) {
    pthread_mutex_lock(&station->mutex);
    station->num_empty_seats = count;

    // Signal all waiting passengers that a train has arrived
    if (station->num_empty_seats > 0 && station->num_waiting_passengers > 0) {
        pthread_cond_broadcast(&station->train_available);
    }

    // Wait until either:
    // 1. The train is full, or
    // 2. No passengers are waiting AND all boarded passengers have seated
    while ((station->num_empty_seats > 0 && station->num_waiting_passengers > 0) ||
           station->num_passengers_walking_on_train > 0) {
        pthread_cond_wait(&station->passengers_done, &station->mutex);
    }

    station->num_empty_seats = 0; // Reset for next train
    pthread_mutex_unlock(&station->mutex);
}

void station_wait_for_train(struct station *station) {
    pthread_mutex_lock(&station->mutex);
    station->num_waiting_passengers++;

    // Wait until there's a seat available on the train
    while (station->num_empty_seats == 0) {
        pthread_cond_wait(&station->train_available, &station->mutex);
    }

    // Passenger starts boarding
    station->num_waiting_passengers--;
    station->num_empty_seats--;
    station->num_passengers_walking_on_train++;

    // Signal the train to recheck conditions
    pthread_cond_signal(&station->passengers_done);
    pthread_mutex_unlock(&station->mutex);
}

void station_on_board(struct station *station) {
    pthread_mutex_lock(&station->mutex);
    station->num_passengers_walking_on_train--;

    // Signal the train that a passenger has seated
    pthread_cond_signal(&station->passengers_done);
    pthread_mutex_unlock(&station->mutex);
}