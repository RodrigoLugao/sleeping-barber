#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

pthread_t barber1;
pthread_t barber2;
pthread_t barber3;

sem_t client_queue;

sem_t barber1_ready;
sem_t barber2_ready;
sem_t barber3_ready;

void
barber()
{
  DOWN(&client_queue);
}

void
client()
{
  UP(&client_queue);
}

int
main(int argc, char** argv)
{
  sem_init(&client_queue, 0, 0);

  sem_init(&barber1_ready, 0, 1);
  sem_init(&barber2_ready, 0, 1);
  sem_init(&barber3_ready, 0, 1);


  pthread_create(&barber1, NULL, &barber, NULL);
  pthread_create(&barber2, NULL, &barber, NULL);
  pthread_create(&barber3, NULL, &barber, NULL);

  int client_num;

  if(argc != 2)
  {
    printf("-- The amount of clients wasn't specified\n");
    exit(1);
  }
  
  client_num = argv[2];
  if(client_num < 1)
  {
    printf("-- The amount of clients can't be zero or negative\n");
  }

  pthread_t clients[client_num];

  int i;
  for(i = 0; i < client_num; i++)
  {
    pthread_create(clients[i], NULL, &&shave, NULL);
  }



  return 0;
}
