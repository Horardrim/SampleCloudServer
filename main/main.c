#include <stdio.h>
#include <string.h>

#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>

#include <matrix_c/hm_char_matrix.h>

int main(int argc, char ** argv)
{
    struct event_base *base;
    base = event_base_new();

    event_base_free(base);
    base = 0;

    hm_char_matrix_t * test_matrix = _hm_char_matrix_t(10, 10);
    test_matrix->set_matrix_value(test_matrix, '%', 5, 5);
    test_matrix->display_matrix(test_matrix);
    hm_char_matrix_t_(test_matrix);
    return 0;
}
