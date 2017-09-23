#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

pthread_t barber1;      // This barber shaves the client's beard
pthread_t barber2;      // This barber paints the client's hair
pthread_t barber3;      // This barber cuts the client's hair

sem_t barbershop_seats; // Grants access to update the 'seats' variable

sem_t client_queue;     // Arranges the clients in a queue to be served

sem_t barber1_ready;    // Mutex that tells when barber 1 is ready to serve the clients
sem_t barber2_ready;    // Mutex that tells when barber 2 is ready to serve the clients
sem_t barber3_ready;    // Mutex that tells when barber 3 is ready to serve the clients

int seats = 10;         // Amount of seats in the barbershop

void*
shave()
{
  while(1)
  {
    sem_wait(&client_queue);
    sem_wait(&barbershop_seats);
    seats++;
    sem_signal(&barber1);
    sem_signal(&barbershop_seats);
    
    printf("Barber number 1 is shaving a customer's beard\n");
  }
}

void*
paint()
{
  while(1)
  {
    sem_wait(&client_queue);
    sem_wait(&barbershop_seats);
    seats++;
    sem_signal(&barber1);
    sem_signal(&barbershop_seats);
    
    printf("Barber number 2 is painting a customer's hair\n");
  }
}

void*
haircut()
{
  while(1)
  {
    sem_wait(&client_queue);
    sem_wait(&barbershop_seats);
    seats++;
    sem_signal(&barber1);
    sem_signal(&barbershop_seats);
    
    printf("Barber number 3 is cutting a customer's hair\n");
  }
}

void*
client(char task, int id)
{
  sem_t* barber;
  char* message;
  switch(task)
  {
    case 's':
      barber = &barber1_ready;
      message = "Client number %d is having his beard shaved\n";
      break;

    case 'p':
      barber = &barber2_ready;
      message = "Client number %d is having his hair painted\n";
      break;

    case 'h':
      barber = &barber3_ready;
      message = "Client number %d is having a haircut\n";
      break;
  }
 
  sem_wait(&barbershop_seats);
  
  if(seats > 0)
  {
    seats--;
    sem_signal(&client_queue);
    sem_signal(&barbershop_seats);
    sem_wait(barber);
    printf(message, id);
  }
  else
  {
    sem_signal(&barbershop_seats);
    printf("Client number %d left due to no available seats\n", id);
  }
}

int
main(int argc, char** argv)
{
  sem_init(&barbershop_seats, 0, 1);

  sem_init(&client_queue, 0, 0);

  sem_init(&barber1_ready, 0, 0);
  sem_init(&barber2_ready, 0, 0);
  sem_init(&barber3_ready, 0, 0);


  pthread_create(&barber1, NULL, &shave, NULL);
  pthread_create(&barber2, NULL, &paint, NULL);
  pthread_create(&barber3, NULL, &haircut, NULL);

  if(argc < 1)
  {
    printf("-- Clients string wasn't specified\n");
    exit(1);
  }

  char* clients_arg;
  clients_arg = argv[1];

  int client_num = strlen(clients_arg);

  pthread_t clients[client_num];

  int i;
  for(i = 0; i < client_num; i++)
  {
    char task = clients_arg[i];

    if(task != 's' &&
        task != 'p' &&
        task != 'h')
    {
      printf("-- Unknown symbol '%c' was entered\n", task);
      exit(1);
    }

    pthread_create(&clients[i], NULL, &client, &task);
  }

  return 0;
}
