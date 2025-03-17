#include <pthread.h>

struct station {
    int num_empty_seats;
    int num_waiting_passengers;
    int num_passengers_walking_on_train;
    pthread_mutex_t mutex;
    pthread_cond_t train_available;
    pthread_cond_t passengers_done;
};

void station_init(struct station *station);
void station_load_train(struct station *station, int count);
void station_wait_for_train(struct station *station);
void station_on_board(struct station *station);