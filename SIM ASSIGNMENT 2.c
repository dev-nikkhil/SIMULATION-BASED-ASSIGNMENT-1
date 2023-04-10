#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_ITEMS 3
#define NUM_STUDENTS 3

int items[NUM_ITEMS] = {0}; // 0: pen, 1: paper, 2: question paper
sem_t item_semaphores[NUM_ITEMS];
sem_t table_mutex;

void *teacher(void *arg) {
    int i = 0;
    while (1) {
        sem_wait(&table_mutex);
        if (i == NUM_ITEMS) {
            i = 0;
        }
        int item1 = i;
        int item2 = (i + 1) % NUM_ITEMS;
        printf("Teacher placed %s and %s on the table.\n", 
            item1 == 0 ? "pen" : item1 == 1 ? "paper" : "question paper", 
            item2 == 0 ? "pen" : item2 == 1 ? "paper" : "question paper");
        sem_post(&item_semaphores[item1]);
        sem_post(&item_semaphores[item2]);
        sem_post(&table_mutex);
        i++;
    }
}

void *student(void *arg) {
    int student_id = *(int *)arg;
    int item1 = (student_id + 1) % NUM_ITEMS;
    int item2 = (student_id + 2) % NUM_ITEMS;
    while (1) {
        sem_wait(&item_semaphores[item1]);
        sem_wait(&item_semaphores[item2]);
        sem_wait(&table_mutex);
        printf("Student %d completed the assignment.\n", student_id);
        sem_post(&table_mutex);
    }
}

int main() {
    // Initialize semaphores
    sem_init(&item_semaphores[0], 0, 1);
    sem_init(&item_semaphores[1], 0, 1);
    sem_init(&item_semaphores[2], 0, 1);
    sem_init(&table_mutex, 0, 1);
    // Create teacher thread
    pthread_t teacher_thread;
    pthread_create(&teacher_thread, NULL, teacher, NULL);
    // Create student threads
    pthread_t student_threads[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];
    for (int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i;
        pthread_create(&student_threads[i], NULL, student, &student_ids[i]);
    }
    // Join threads
    pthread_join(teacher_thread, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(student_threads[i], NULL);
    }
}