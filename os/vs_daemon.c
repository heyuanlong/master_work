#include "vs_daemon.h"
#include "vs_core.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int vs_daemon()
{
	pid_t			pid;
	int				fd;


	pid = fork();
	if (pid < 0)
	{
		return VS_ERROR;
	}
	else if (pid > 0)
	{
		exit(0);		//parent exit;
	}

	if (setsid() == -1) {
		return VS_ERROR;
	}

	//umask( 0 );
	fd = open("/dev/null", O_RDWR);
	if (fd == -1) {
		return VS_ERROR;
	}

	if (dup2(fd, STDIN_FILENO) == -1) {
		return VS_ERROR;
	}

	if (dup2(fd, STDOUT_FILENO) == -1) {
		return VS_ERROR;
	}
	if (dup2(fd, STDERR_FILENO) == -1) {
		return VS_ERROR;
	}

	return VS_OK;
}