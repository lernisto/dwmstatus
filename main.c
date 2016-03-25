
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <X11/Xlib.h>

#include "config.h"


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


int main()
{
	char buffer[BUFFMAX];
	struct tm *now;
	time_t t;
	unsigned int wait;
	size_t b0;

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
		setname(buffer);

		wait = 60 - now->tm_sec;
		while (wait > 0) {
			wait = sleep(wait);
		}

	}
	exit(EXIT_SUCCESS);
}
