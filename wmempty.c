#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DOCK_SIZE 64

int main(int argc, char **argv)
{
    Display *dpy;
    Window win, iconwin;
    XWMHints wmhints;
    XClassHint classhint;
    XSizeHints sizehints;
    XEvent ev;
    int screen;
    unsigned long bg;
    char name[64];

    if (argc > 1)
        snprintf(name, sizeof(name), "%s", argv[1]);
    else
        snprintf(name, sizeof(name), "wmempty-%ld", (long)getpid());

    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "%s: cannot open display\n", argv[0]);
        return 1;
    }

    screen = DefaultScreen(dpy);
    bg = BlackPixel(dpy, screen);

    win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen),
                               0, 0, DOCK_SIZE, DOCK_SIZE, 0, bg, bg);

    iconwin = XCreateSimpleWindow(dpy, RootWindow(dpy, screen),
                                   0, 0, DOCK_SIZE, DOCK_SIZE, 0, bg, bg);

    memset(&sizehints, 0, sizeof(sizehints));
    sizehints.flags = PPosition | PSize | PMinSize | PMaxSize;
    sizehints.x = 0;
    sizehints.y = 0;
    sizehints.width  = sizehints.min_width  = sizehints.max_width  = DOCK_SIZE;
    sizehints.height = sizehints.min_height = sizehints.max_height = DOCK_SIZE;
    XSetWMNormalHints(dpy, win, &sizehints);

    memset(&wmhints, 0, sizeof(wmhints));
    wmhints.flags = StateHint | IconWindowHint | WindowGroupHint;
    wmhints.initial_state = WithdrawnState;
    wmhints.icon_window = iconwin;
    wmhints.window_group = win;
    XSetWMHints(dpy, win, &wmhints);

    classhint.res_name = name;
    classhint.res_class = "DockApp";
    XSetClassHint(dpy, win, &classhint);
    XSetClassHint(dpy, iconwin, &classhint);

    XStoreName(dpy, win, name);
    XStoreName(dpy, iconwin, name);

    XSelectInput(dpy, win, StructureNotifyMask);
    XSelectInput(dpy, iconwin, ExposureMask | ButtonPressMask | StructureNotifyMask);

    XMapWindow(dpy, win);
    XMapWindow(dpy, iconwin);
    XFlush(dpy);

    for (;;) {
        XNextEvent(dpy, &ev);
        switch (ev.type) {
        case Expose:
            if (ev.xexpose.count == 0)
                XClearWindow(dpy, ev.xexpose.window);
            break;
        case DestroyNotify:
            XCloseDisplay(dpy);
            return 0;
        default:
            break;
        }
    }
}
