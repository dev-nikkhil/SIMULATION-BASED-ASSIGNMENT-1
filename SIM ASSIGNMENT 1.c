#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t pen_sem, paper_sem, question_paper_sem, table_sem;

void *teacher()
{
    while(1) {
        sem_wait(&table_sem); // wait for table to be free
        sem_post(&pen_sem);
        sem_post(&paper_sem);
        sem_post(&question_paper_sem);
    }
}

void *student1()
{
    while(1) {
        sem_wait(&pen_sem);
        sem_wait(&paper_sem);
        sem_wait(&table_sem);
        printf("Student 1 has completed the assignment.\n");
        sem_post(&table_sem);
    }
}

void *student2()
{
    while(1) {
        sem_wait(&paper_sem);
        sem_wait(&question_paper_sem);
        sem_wait(&table_sem);
        printf("Student 2 has completed the assignment.\n");
        sem_post(&table_sem);
    }
}

void *student3()
{
    while(1) {
        sem_wait(&pen_sem);
        sem_wait(&question_paper_sem);
        sem_wait(&table_sem);
        printf("Student 3 has completed the assignment.\n");
        sem_post(&table_sem);
    }
}

int main()
{
    sem_init(&pen_sem, 0, 0);
    sem_init(&paper_sem, 0, 0);
    sem_init(&question_paper_sem, 0, 0);
    sem_init(&table_sem, 0, 1); // initialize to 1, since only one student can access the table at a time

    pthread_t teacher_thread, student1_thread, student2_thread, student3_thread;

    pthread_create(&teacher_thread, NULL, teacher, NULL);
    pthread_create(&student1_thread, NULL, student1, NULL);
    pthread_create(&student2_thread, NULL, student2, NULL);
    pthread_create(&student3_thread, NULL, student3, NULL);

    pthread_join(teacher_thread, NULL);
    pthread_join(student1_thread, NULL);
    pthread_join(student2_thread, NULL);
    pthread_join(student3_thread, NULL);

    return 0;
}
