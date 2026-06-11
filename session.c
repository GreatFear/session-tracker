#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <libpq-fe.h>

void session()
{
  printf("Session Starting\n");
  printf("Type done to exit\n");

  char buf[20];

  while(1)
    {
      if(fgets(buf, sizeof(buf), stdin) != NULL)
	{
	  // Replaces the enter symbol with a null character
	  buf[strcspn(buf, "\n")] = '\0';

	  // Checks if done is contained in buf
	  if(strstr("done", buf) != NULL)
	    {
	      break;
	    }
	  else
	    {
	      fprintf(stderr, "Invalid input: %s\nEnter done when the session is over\n", buf);
	    }
	}
    }
  printf("Session is done\n");
}

// Printing results to terminal
void printing_command_line(char session[20], char topic[255], char goal[255], char achieved[10])
{
  printf("=== New Entry ===\n");
  printf("Session Time: %02d:%02d:%02d\n", session[0], session[1], session[2]);
  printf("Topic: %s\n", topic);
  printf("Goal: %s\n", goal);
  printf("Achieved: %s\n", achieved);
}

// Inserting into the database
int insert_session(int session_length, char topic[255], char goal[255], char achieved[10])
{
  PGconn *connection = PQconnectdb("dbname=studying user=session_admin");

  if(PQstatus(connection) != CONNECTION_OK)
    {
      fprintf(stderr, "Connection Failed: %s\n", PQerrorMessage(connection));
      PQfinish(connection);
      return 1;
    }
  char* insert = malloc(4096);
  
  sprintf(insert, "INSERT into session (topic, goal, achieved, duration) VALUES ('%s', '%s', '%s', INTERVAL '%d seconds')", topic, goal, achieved, session_length);

  PGresult *result = PQexec(connection, insert);
  
  if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
      fprintf(stderr, "PQ Result Status: %d %d\n", PQresultStatus(result), PGRES_COMMAND_OK);
      fprintf(stderr, "Insert failed: %s\n", PQerrorMessage(connection));
      PQclear(result);
      PQfinish(connection);
      return 1;
    }

  PQclear(result);
  PQfinish(connection);
  free(insert);
  
  return 0;
}

// Affect recording 
void inserting(int manual)
{
  char topic[255], goal[255], achieved[10];

  // achieved is a boolean
  // session_duration is recorded in seconds
  int session_duration;

  
  printf("What is the topic for this session?\n");
  fgets(topic, sizeof(topic), stdin);
  topic[strcspn(topic, "\n")] = '\0';

  printf("Topic: %s\n", topic);
  
  printf("What is the goal for this session?\n");
  fgets(goal, sizeof(goal), stdin);
  goal[strcspn(goal, "\n")] = '\0';
  printf("Goal: %s\n", goal);

  if(manual)
    {
      session_duration = manual_questions();
    }
  else
    {
      // Records the time
      time_t start = time(NULL);
      session();
      time_t end = time(NULL);
  
      session_duration = difftime(end, start);
    }
  
  printf("What did you achieve this session? (Yes or No)\n");
  fgets(achieved, sizeof(achieved), stdin);
  achieved[strcspn(achieved, "\n")] = '\0';
  
  insert_session(session_duration, topic, goal, achieved); 
}

// Converts a human input into seconds
int manual_questions()
{
  char* session_length[255];
  printf("How long was the session?(HH:MM:SS)\n");
  fgets(*session_length, sizeof(*session_length), stdin);
  *session_length[strcspn(*session_length, "\n")] = '\0';

  char* token = strtok(*session_length, ":");
  int seconds = 0;
  
  for(int i = 0; i < 3; i++)
    {
      switch(i)
	{
	  // Hours
	case 0:
	  seconds +=  atoi(token) * 3600;
	  break;
	  // Minutes
	case 1:
	  seconds += atoi(token) * 60;
	  break;
	  // Seconds
	case 2:
	  seconds += atoi(token);
	  break;
	}
      token = strtok(NULL, ":");
    }

  return seconds;
}

int main()
{
  automatic_insertion();
}
