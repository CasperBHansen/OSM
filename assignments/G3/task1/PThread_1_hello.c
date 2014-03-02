#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>

#define NUM_THREADS 6

char* progname;

void error(void) {
  fprintf(stderr, "%s: %s\n", progname, strerror(errno));
  exit(EXIT_FAILURE);
}

void* thread_function(void* data) {
  printf("Hello, it’s me, thread no. %d\n", (int)data);
  pthread_exit(NULL);
}

int main(int argc, char** argv) {
  int i, r, n;

  progname = argv[0];
  if (argc < 2) n = NUM_THREADS; else n = atoi(argv[1]);

  pthread_t* threads = malloc(sizeof(pthread_t)*n);
  if ( !threads ) error();
  int* thread_data = (int*) malloc(sizeof(int)*n);
  if ( !thread_data ) error();

  for(i = 0; i < n; i++) {
    thread_data[i] = i;
    r = pthread_create(&threads[i], NULL, thread_function, 
                       (void*)(thread_data[i]));
    if (r != 0) { error(); }
  }
  for(i = 0; i < n; i++) {
    r = pthread_join(threads[i], NULL);
    if (r != 0) { error(); }
  }
  free(threads);
  free(thread_data);

  return 0;
}

