#include <stdio.h>

#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>

int main(int argc, char ** argv)
{
    struct event_base *base;
    base = event_base_new();

    event_base_free(base);
    base = 0;
    return 0;
}
