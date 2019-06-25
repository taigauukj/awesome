/* exec_argv[1]にコマンドのオプション、
exec_argv[2]にNULLを代入*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 80
#define PATH_LENGTH 5

void lntrim(char *str) {
	char *p;
	p = strchr(str, '\n');
	if (p != NULL) {
		*p = '\0';
	}
}

int main() {
	char buf[BUF_SIZE];
	char cmdname[PATH_LENGTH + BUF_SIZE];
	char *exec_argv[3];
	int childpid, status;
	char *tp;

	printf("-- parent pid: %d\n", getpid());
	printf("> ");
	while (fgets(buf, BUF_SIZE, stdin) != NULL) {
		lntrim(buf);
		strcpy(cmdname, "/bin/");
		sscanf(buf, "%s", &cmdname[PATH_LENGTH]);
		exec_argv[0] = cmdname;
		tp = strtok(buf, " ");
		exec_argv[1] = NULL; exec_argv[2] = NULL;
		while (tp != NULL) {
			tp = strtok(NULL, " ");
			if (tp != NULL) exec_argv[1] = tp;
		}
		switch (childpid = fork()) {
		case 0:
			if (exec_argv[1] != NULL) {
				printf("-- forked process %d: %s %s\n", getpid(), exec_argv[0], exec_argv[1]);
			}
			else {
				printf("-- forked process %d: %s \n", getpid(), exec_argv[0]);
			}
			execve(exec_argv[0], exec_argv, NULL);
			perror("exec failure");
			exit(-1);

		case -1:
			perror("fork failure");
			exit(-1);

		default:
			wait(&status);
		}
		printf("> ");
	}
	printf("parent terminated\n");
	return (0);
}



