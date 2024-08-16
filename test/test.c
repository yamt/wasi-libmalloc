#include <assert.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * w/o volatile here, clang optimizes
 *
 *  malloc_called = 0;
 *  void *p = malloc(10);
 *  assert(malloc_called == 1);
 *
 * to
 *
 *  assertion_failed("malloc_called == 1");
 */
volatile int malloc_called;
volatile int free_called;

__attribute__((export_name("test_malloc_hook"))) void
test_malloc_hook(size_t size)
{
        int saved_errno = errno;
        printf("%s called\n", __func__);
        malloc_called++;
        errno = saved_errno;
}

__attribute__((export_name("test_free_hook"))) void
test_free_hook(void *p)
{
        int saved_errno = errno;
        printf("%s called %p\n", __func__, p);
        free_called++;
        errno = saved_errno;
}

int
main(int argc, char **argv)
{
        printf("calling duplocale\n");
        malloc_called = 0;
        locale_t loc = duplocale(LC_GLOBAL_LOCALE);
        assert(malloc_called == 1);

        printf("calling freelocale\n");
        free_called = 0;
        freelocale(loc);
        assert(free_called == 1);

        printf("calling malloc\n");
        malloc_called = 0;
        void *p = malloc(10);
        assert(malloc_called == 1);

        printf("calling free %p\n", p);
        free_called = 0;
        free(p);
        assert(free_called == 1);

        printf("calling strdup\n");
        malloc_called = 0;
        p = strdup("foo");
        assert(malloc_called == 1);

        printf("calling free %p\n", p);
        free_called = 0;
        free(p);
        assert(free_called == 1);

        char *cp;
        printf("calling asprintf\n");
        malloc_called = 0;
        asprintf(&cp, "asprintf from %s\n", __func__);
        assert(malloc_called > 0);
        printf("asprintf result %s\n", cp);

        printf("calling free %p\n", p);
        free_called = 0;
        free(cp);
        assert(free_called == 1);

        printf("done\n");
}
