#include <pthread.h>
#include "caltrain.h"

void station_init(struct station *station) {
    station -> num_empty_seats = 0;   
    station -> num_waiting_passengers = 0;  
    station -> num_passengers_walking_on_train = 0;
    pthread_mutex_init(&station -> mutex, NULL);
    pthread_cond_init(&station -> train_available, NULL);   
    pthread_cond_init(&station -> passengers_done, NULL);   
}

void station_load_train(struct station *station, int count) {
    pthread_mutex_lock(&station -> mutex);
    station -> num_empty_seats = count;
    // Announce that the train has arrived
    if (station -> num_empty_seats > 0 && station -> num_waiting_passengers > 0){
        pthread_cond_broadcast(&station -> train_available);
    } 
    // Train doesn't leave unless it's filled or no more waiting passengers
    while ((station -> num_empty_seats > 0 && station -> num_waiting_passengers > 0) || (station -> num_passengers_walking_on_train > 0)){
        pthread_cond_wait(&station -> passengers_done, &station -> mutex);
    }
    station -> num_empty_seats = 0; 
    pthread_mutex_unlock(&station -> mutex); 
}

void station_wait_for_train(struct station *station) {
    pthread_mutex_lock(&station -> mutex);
    // Increase number of waiting passengers until the train is available 
    station -> num_waiting_passengers++;
    while (station -> num_empty_seats == 0){
        pthread_cond_wait(&station -> train_available, &station -> mutex);
    }
    // When train is availble start boarding passengers
    station -> num_waiting_passengers--;
    station -> num_empty_seats--;
    station -> num_passengers_walking_on_train++;
    // Check if the passengers are completed
    pthread_cond_signal(&station -> passengers_done);
    pthread_mutex_unlock(&station -> mutex); 
    
}

void station_on_board(struct station *station) {
    pthread_mutex_lock(&station -> mutex);
    // Let the passengers get seated 
    station -> num_passengers_walking_on_train--;
    // Check if the passengers are all seated
    pthread_cond_signal(&station -> passengers_done);
    pthread_mutex_unlock(&station -> mutex);
    
}