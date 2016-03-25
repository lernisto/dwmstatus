
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <X11/Xlib.h>

#include "config.h"

const size_t BUFFMAX = 250;
const char *sysbat = "/sys/class/power_supply/BAT1/capacity";

static char *display_name = NULL;

void setname(const char *text)
{
	Display *dpy;
	int screen;
	Window root;

	dpy = XOpenDisplay(display_name);
	if (!dpy) {
		fprintf(stderr, "unable to open display '%s'\n",
			XDisplayName(display_name));
		exit(111);
	}
	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);

	XStoreName(dpy, root, text);
	XCloseDisplay(dpy);
}

int slurp(char *buffer,size_t maxlen,const char* filename){
	FILE* f;
	size_t cbread;
	if((f=fopen(filename,"r"))==NULL){
		return 0;
	}
	cbread = fread(buffer,sizeof(char),maxlen,f);
	fclose(f);
	return cbread;
}

int main()
{
	char buffer[BUFFMAX];
	struct tm *now;
	time_t t;
	unsigned int wait;
	size_t b0;
	size_t b1;

	display_name = getenv("DISPLAY");
	if (!display_name) {
		fputs("DISPLAY not set", stderr);
		exit(100);
	}

	for (;;) {
		t = time(NULL);
		now = localtime(&t);
		if (now == NULL) {
			perror("localtime");
			exit(111);
		}

		if ((b0 = strftime(buffer, BUFFMAX, TIMEFMT, now))== 0) {
			perror("strftime");
			exit(100);
		}
		b1 = slurp(&buffer[b0],BUFFMAX-b0,sysbat);
		if(b1>1){
            // -1 because we want to chomp off the '\n'
			buffer[b0+b1-1]='\0';
		}
		setname(buffer);

		wait = 60 - now->tm_sec;
		while (wait > 0) {
			wait = sleep(wait);
		}

	}
	exit(EXIT_SUCCESS);
}
