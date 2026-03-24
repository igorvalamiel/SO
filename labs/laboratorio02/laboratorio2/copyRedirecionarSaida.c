#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

//gcc hello.c -o hello

int main(void){

	pid_t pid;
	int mypipe[2];

	/* Create the pipe. */
	if (pipe (mypipe))
		{
		fprintf (stderr, "Pipe failed.\n");
		return EXIT_FAILURE;
		}


	/* Create the child process. */
	pid_t pid = fork();
	int fd;
	char *filename = "saida.out";

	if (pid == (pid_t) 0)
		{
		/* This is the child process.
			Close other end first. */
		close (mypipe[1]);
		read_from_pipe (mypipe[0]);
		return EXIT_SUCCESS;
		}
	else if (pid < (pid_t) 0)
		{
		/* The fork failed. */
		fprintf (stderr, "Fork failed.\n");
		return EXIT_FAILURE;
		}
	else
		{
		/* This is the parent process.
			Close other end first. */
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(mypipe[1], STDOUT_FILENO);
		execlp("/bin/ping","ping","localhost","-c","10",NULL);
		close (mypipe[0]);
		write_to_pipe (mypipe[1]);
		return EXIT_SUCCESS;
		}
}
