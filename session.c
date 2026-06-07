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
	  if(strstr("done", buf) == 0)
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
void printing_command_line(char session[16], char topic[1024], char goal[1024], char achieved[1024])
{
  printf("=== New Entry ===\n");
  printf("Session Time: %02d:%02d:%02d\n", session[0], session[1], session[2]);
  printf("Topic: %s\n", topic);
  printf("Goal: %s\n", goal);
  printf("Achieved: %s\n", achieved);
}

// Inserting into the database
int insert_session(char session[16], char topic[256], char goal[256], char achieved[256])
{

  // SET THIS UP LATER
  
  return 0;
}
