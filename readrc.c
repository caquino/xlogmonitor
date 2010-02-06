/*	Copyright (c) 1998, Alex Fiori
 * 
 *	readrc.c file for chaosmaker's xlogmonitor.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include "readrc.h"

#define RC_FILE		".xlogmonitorrc"
#define LOG_1		"/var/log/messages"
#define LOG_2		"/var/log/syslog"
#define LOG_3		"/var/log/icmpinfo"

char *xlogmonitor_log1;
char *xlogmonitor_log2;
char *xlogmonitor_log3;

FILE *fp;
char rcpath[128];

int read_conf(void)
{
	FILE *temp;

	char log1[128];
	char log2[128];
	char log3[128];
	
	char line[100];
	char *confcmd, *confval;

	int howfar;

        int uid;
        struct passwd *ent;

        uid = getuid();
        ent = getpwuid(uid);
        endpwent();

        sprintf (rcpath, "%s/%s", ent->pw_dir, RC_FILE);

	if (access(rcpath,0) == -1)
	{
		printf ("Can't open %s file. Creating default...\n", rcpath);
		temp = fopen (rcpath, "w");
		fprintf (temp, "log1=%s\n", LOG_1);
		fprintf (temp, "log2=%s\n", LOG_2);
		fprintf (temp, "log3=%s\n", LOG_3);
		fclose(temp);
	}

	if (access(rcpath,0) != -1)
		fp=fopen(rcpath,"r");
		
	howfar=0;

	while(!feof(fp))	
	{
		memset(line, 0, 100);
		fgets(line, 100, fp);
		howfar++;

		if(!strlen(line)) continue;

		if(strncmp(line,"#",1)) {
			confcmd=NULL;
			confval=NULL;

			confcmd=strtok(line,"=");
			if(!confcmd) continue;
			confval=strtok(NULL,"\n");
			if(!confval) continue;

			if(!strcasecmp(confcmd,"log1"))
				strcpy(log1,confval);

			if(!strcasecmp(confcmd,"log2"))
				strcpy(log2,confval);

			if(!strcasecmp(confcmd,"log3"))
				strcpy(log3,confval);
		} 
	}

	fclose (fp);

	xlogmonitor_log1 = strdup(log1);
	xlogmonitor_log2 = strdup(log2);
	xlogmonitor_log3 = strdup(log3);

	return 0;
}
