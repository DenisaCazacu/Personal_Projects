#define _GNU_SOURCE // enable some extended POSIX functionality (pthread_barrier_t)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sqlite3.h>

#define PORT 2909
#define MAX_NUMBER_OF_PLAYERS 3
#define NUMBER_OF_QUESTIONS 10
#define BUFFER_SIZE 1024
#define RED "\033[31m"
#define ORANGE "\033[38;5;214m"
#define PURPLE "\033[0;35m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RESET_COLOR "\033[0m"
#define handle_error(error_name)                                             \
  {                                                                          \
    fprintf(stderr, RED "[Server] Error at %s.\n " RESET_COLOR, error_name); \
    exit(EXIT_FAILURE);                                                      \
  }

pthread_mutex_t database_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t room_lock = PTHREAD_MUTEX_INITIALIZER;
sqlite3 *questions_database;

struct Player
{
  int socket;
  int score;
  pthread_t thread_id;
};
struct Room
{
  int room_number;
  int player_count;
  struct Player *players[MAX_NUMBER_OF_PLAYERS];
  pthread_barrier_t barrier;
  pthread_t thread_id;
};
struct StartGameArgs
{
  struct Player *player;
  struct Room *room;
};
static void *start_game(void *arg);
static void *join_game(void *arg);
void initialize_socket(int *socket_descriptor, struct sockaddr_in *server, struct sockaddr_in *from);
sqlite3 *initialize_database(const char *db_path);
int socket_write(int descriptor, const char *message);
char *socket_read(int descriptor);
void handle_disconnection(struct Room *room, struct Player *disconnected_player);
int send_question(sqlite3 *db, int id_question, int player_descriptor);
int verify_answear(sqlite3 *db, int id_question, char *answear);

int main()
{
  fprintf(stdout, GREEN "Server running at the address 127.0.0.1 : 2909!\n" RESET_COLOR);
  fflush(stdout);

  int socket_descriptor;
  struct sockaddr_in server;
  struct sockaddr_in from;
  initialize_socket(&socket_descriptor, &server, &from);

  // attach the socket descriptor to server
  if (bind(socket_descriptor, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    handle_error("bind");

  // listen for clients
  if (listen(socket_descriptor, 2) == -1)
    handle_error("listen");

  int client_count = 0;
  int rooms = 0;
  struct Room *room;
  questions_database = initialize_database("questions.db");

  while (true)
  {
    if (client_count == 0)
    {
      // prepare the structures for the incoming players
      rooms++;
      room = (struct Room *)malloc(sizeof(struct Room));
      if (room == NULL)
        handle_error("memory allocation for a new room");
      for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++)
      {
        room->players[i] = (struct Player *)malloc(sizeof(struct Player));
        if (room->players[i] == NULL)
          handle_error("memory allocation for a new player");
        room->room_number = rooms;
        room->player_count = 0;
        room->players[i]->score = 0;
      }
    }
    int client_socket;
    int length = sizeof(from);
    // accept clients
    if ((client_socket = accept(socket_descriptor, (struct sockaddr *)&from, &length)) < 0)
      handle_error("accept");

    room->players[client_count]->socket = client_socket;
    client_count++;
    room->player_count = client_count;

    fprintf(stdout, YELLOW "[Room %d] Client connected.\n" RESET_COLOR, rooms);
    fflush(stdout);

    // send a message for the new client
    char msg[100];
    sprintf(msg, " Waiting for %d more players to join...", MAX_NUMBER_OF_PLAYERS - client_count);

    if (MAX_NUMBER_OF_PLAYERS - client_count > 0)
      if (socket_write(client_socket, msg) < 0)
        handle_error("write");
    if (MAX_NUMBER_OF_PLAYERS == client_count)
      if (socket_write(client_socket, "The room is full. Starting the game.") < 0)
        handle_error("write");

    // if the room is full the players can join the game
    if (client_count == MAX_NUMBER_OF_PLAYERS)
    {
      client_count = 0;
      if (pthread_barrier_init(&room->barrier, NULL, room->player_count) != 0)
        handle_error("pthread_barrier_init");
      // start a new game session (a thread for each room)
      if (pthread_create(&room->thread_id, NULL, &join_game, room) != 0)
        handle_error("pthred_create");
    }
  }
}
void initialize_socket(int *socket_descriptor, struct sockaddr_in *server, struct sockaddr_in *from)
{
  // Create the socket
  if ((*socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    handle_error("socket");

  // Enable SO_REUSEADDR to allow address reuse
  int on = 1;
  if (setsockopt(*socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
    handle_error("setsockopt");

  // Clear and prepare the server structure
  bzero(server, sizeof(*server));
  bzero(from, sizeof(*from));

  // Initialize server socket structure
  server->sin_family = AF_INET;
  server->sin_addr.s_addr = htonl(INADDR_ANY);
  server->sin_port = htons(PORT);
}
int socket_write(int descriptor, const char *message)
{
  int length = strlen(message) + 1; // Include terminatorul '\0'
  int total_written = 0;
  int bytes_written;

  while (total_written < length)
  {
    bytes_written = write(descriptor, message + total_written, length - total_written);
    if (bytes_written < 0)
      return -1;

    total_written += bytes_written;
  }

  return total_written;
}
char *socket_read(int descriptor)
{
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  int bytes_read = read(descriptor, buffer, BUFFER_SIZE - 1);
  if (bytes_read < 0)
  {
    return NULL;
  }
  else if (bytes_read == 0)
  {
    strcpy(buffer, "client disconnected");
  }
  else
  {
    buffer[bytes_read] = '\0';
  }
  char *result = strdup(buffer);
  if (!result)
    handle_error("strdup");
  return result;
}
static void *start_game(void *arg)
{
  struct StartGameArgs *args = (struct StartGameArgs *)arg;
  struct Player *player = args->player;
  struct Room *room = args->room;

  for (int j = 1; j <= NUMBER_OF_QUESTIONS; j++)
  {
    // Each thread is responsible for the communication with one player: it sends the question and waits for the response
    fprintf(stdout, PURPLE "[Debug] Sending question %d to the player.\n" RESET_COLOR, j);
    fflush(stdout);

    char question[100];
    char *buffer;
    if (send_question(questions_database, j, player->socket) < 0)
      handle_error("sending the question");

    fprintf(stdout, PURPLE "[Debug] The question have been sent successfully.\n" RESET_COLOR);
    fflush(stdout);
    // Waiting for the answear
    buffer = socket_read(player->socket);
    if (buffer == NULL)
    {
      handle_error("read");
    }
    else if (strcmp(buffer, "client disconnected") == 0 || strcmp(buffer, "quit") == 0)
    {
      handle_disconnection(room, player);
      pthread_detach(pthread_self());
      return (NULL);
    }
    fprintf(stdout, YELLOW "[Client] Thread %ld: Player sent: %s\n" RESET_COLOR, pthread_self(), buffer);
    fflush(stdout);

    int result = verify_answear(questions_database, j, buffer);
    if (result < 0)
    {
      handle_error("checking the result");
    }
    else if (result == 0)
    {
      fprintf(stdout, YELLOW "Player responded incorrectly\n" RESET_COLOR);
      fflush(stdout);
    }
    else if (result == 1)
    {
      player->score++;
      fprintf(stdout, YELLOW "Player responded correctly\n" RESET_COLOR);
      fflush(stdout);
    }

    fprintf(stdout, ORANGE "[Debug] Thread %ld reached barrier.\n" RESET_COLOR, pthread_self());
    fflush(stdout);
    pthread_barrier_wait(&room->barrier);
    fprintf(stdout, GREEN "[Debug] Thread %ld passed barrier.\n" RESET_COLOR, pthread_self());
    fflush(stdout);
  }

  // The game is over
  fprintf(stdout, PURPLE "[Debug] The game finished. Sending 'GAME OVER!'.\n" RESET_COLOR);
  fflush(stdout);
  
  if (socket_write(player->socket, "GAME OVER!") <= 0)
    handle_error("write");
  // compute highscore
  sleep(1);
  pthread_mutex_lock(&room_lock);
  int highscore = 0;
  for (int i = 0; i < room->player_count; i++)
  {
    if (room->players[i]->score > highscore)
    {
      highscore = room->players[i]->score;
    }
  }
  fprintf(stdout, PURPLE "[Debug] The highscore is: %d\n" RESET_COLOR, highscore);
  char highscore_msg[BUFFER_SIZE];
  if (player->score == highscore)
  {
    sprintf(highscore_msg, "YOU WON!\nYour score: %d points\nHighest score: %d points", player->score,highscore);
  }
  else
  {
    sprintf(highscore_msg, "YOU LOST!\nYour score: %d points\nHighest score: %d points", player->score,highscore);
  }
  fprintf(stdout, PURPLE "[Debug] Sending the message '%s' to the player.\n" RESET_COLOR, highscore_msg);
  if (socket_write(player->socket, highscore_msg) <= 0)
    handle_error("write");
  fprintf(stdout, PURPLE "[Debug] The message has been sent successfully.\n" RESET_COLOR);
  pthread_mutex_unlock(&room_lock);
  pthread_detach(pthread_self());
  return (NULL);
}

static void *join_game(void *arg)
{
  struct Room *room;
  room = (struct Room *)arg;
  fprintf(stdout, PURPLE "[Debug] New thread started! Waiting for the players to be ready.\n" RESET_COLOR);
  fflush(stdout);
  int players_ready = 0;
  char *buffer;

  fprintf(stdout, PURPLE "[Debug][Thread %d] Sending 'Ready?' to all the players.\n" RESET_COLOR, room->room_number);
  fflush(stdout);

  char ready_msg[] = "Ready?";
  for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++)
  {
    if (socket_write(room->players[i]->socket, ready_msg) < 0)
      handle_error("write");
  }

  fprintf(stdout, PURPLE "[Debug][Thread %d] All the messages have been sent successfully.\n" RESET_COLOR, room->room_number);
  fflush(stdout);

  // File descriptors for select
  fd_set readfds;
  int max_fd = 0;

  // Find the biggest file descriptor
  for (int i = 0; i < room->player_count; i++)
  {
    if (room->players[i]->socket > max_fd)
    {
      max_fd = room->players[i]->socket;
    }
  }
  // waiting for all the players to be ready

  while (players_ready < room->player_count)
  {
    FD_ZERO(&readfds); // clear the set
    for (int i = 0; i < room->player_count; i++)
    {
      FD_SET(room->players[i]->socket, &readfds); // add the players to the set of descriptors to be monitorized
    }
    int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
    if (activity < 0)
      handle_error("select");

    for (int i = 0; i < room->player_count; i++)
    {
      if (FD_ISSET(room->players[i]->socket, &readfds))
      { // check which descriptor sent the message
        // read the answear
        buffer = socket_read(room->players[i]->socket);
        if (buffer == NULL)
          handle_error("read");
        fprintf(stdout, YELLOW "[CLient][Thread %d] Player %d sent: %s\n" RESET_COLOR, room->room_number, i + 1, buffer);
        fflush(stdout);
        // update the number of ready players
        if (strcmp(buffer, "y") == 0)
        {
          players_ready++;
          fprintf(stdout, PURPLE "[Debug][Thread %d] Player %d is ready. Total ready: %d/%d\n" RESET_COLOR, room->room_number, i + 1, players_ready, room->player_count);
          fflush(stdout);
        }
      }
    }
  }
  // Since all the players are ready we can start sending the questions
  fprintf(stdout, PURPLE "[Debug][Thread %d] The geme can start. Sending 'START!' to all the players.\n" RESET_COLOR, room->room_number);
  fflush(stdout);
  char start_msg[] = "START!";
  for (int i = 0; i < room->player_count; i++)
  {
    if (socket_write(room->players[i]->socket, start_msg) < 0)
      handle_error("write");
  }
  fprintf(stdout, PURPLE "[Debug][Thread %d] All the messages have been sent successfully.\n" RESET_COLOR, room->room_number);
  fflush(stdout);
  pthread_detach(pthread_self());
  // start a thread for each player
  for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++)
  {
    struct StartGameArgs *args = malloc(sizeof(struct StartGameArgs));
    if (!args)
      handle_error("malloc");
    args->player = room->players[i];
    args->room = room;
    if (pthread_create(&room->players[i]->thread_id, NULL, &start_game, args) != 0)
      handle_error("pthred_create");
  }

  return (NULL);
};
void handle_disconnection(struct Room *room, struct Player *disconnected_player)
{
  pthread_mutex_lock(&room_lock);
  for (int i = 0; i < room->player_count; i++)
  {
    if (room->players[i] == disconnected_player)
    {
      fprintf(stdout, RED "[Debug] The client has disconnected.\n" RESET_COLOR);
      fflush(stdout);

      for (int j = i; j < room->player_count - 1; j++)
      {
        room->players[j] = room->players[j + 1];
      }
      room->player_count--;
      break;
    }
  }

  pthread_barrier_wait(&room->barrier);
  pthread_barrier_destroy(&room->barrier);
  if (pthread_barrier_init(&room->barrier, NULL, room->player_count) != 0)
    handle_error("pthread_barrier_init");

  pthread_mutex_unlock(&room_lock);
}
sqlite3 *initialize_database(const char *path)
{
  sqlite3 *db;
  if (sqlite3_open(path, &db))
  {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    exit(EXIT_FAILURE);
  }
  return db;
}
int send_question(sqlite3 *db, int id_question, int player_descriptor)
{
  const char *sql = "SELECT question, option_a, option_b, option_c FROM questions WHERE id = ?";
  sqlite3_stmt *stmt; // sql statement result
  char buffer[BUFFER_SIZE];
  pthread_mutex_lock(&database_lock);
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
  {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  sqlite3_bind_int(stmt, 1, id_question);
  pthread_mutex_unlock(&database_lock);
  if (sqlite3_step(stmt) == SQLITE_ROW)
  {
    const char *question = (const char *)sqlite3_column_text(stmt, 0);
    const char *option_a = (const char *)sqlite3_column_text(stmt, 1);
    const char *option_b = (const char *)sqlite3_column_text(stmt, 2);
    const char *option_c = (const char *)sqlite3_column_text(stmt, 3);
    sprintf(buffer, "Question %d: %s\n A:%s\n B:%s\n C:%s\n", id_question, question, option_a, option_b, option_c);
    if (socket_write(player_descriptor, buffer) <= 0)
    {
      return -1;
    }
  }
  else
  {
    return -1;
  }
  sqlite3_finalize(stmt);
  return 0;
}
int verify_answear(sqlite3 *db, int id_question, char *answear)
{
  const char *sql = "SELECT correct_option FROM questions WHERE id = ?";
  sqlite3_stmt *stmt;
  char *correct_option;
  pthread_mutex_lock(&database_lock);
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
  {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return -1;
  }
  sqlite3_bind_int(stmt, 1, id_question);
  pthread_mutex_unlock(&database_lock);
  if (sqlite3_step(stmt) == SQLITE_ROW)
  {
    correct_option = (char *)sqlite3_column_text(stmt, 0);
  }
  else
  {
    return -1;
  }
  if (strcmp(answear, correct_option) == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}