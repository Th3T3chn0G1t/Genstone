#include <gencommon.h>
#include <genformat.h>
#include <genlog.h>

#include <stdlib.h>

gen_error_t* gen_main(void) {
    gen_tooling_push(GEN_FUNCTION_NAME, GEN_FILE_NAME);
    GEN_TOOLING_AUTO gen_error_t* error;

    gen_error_t* test = gen_error_attach_backtrace(
            GEN_ERROR_UNKNOWN, GEN_LINE_STRING, "Bah humbug!");

    gen_size_t sz = 0;
    error = gen_format(
            GEN_NULL, &sz, GEN_SIZE_MAX, "%t %tz %c %cz %uz %ui %sz %si %e",
            "Hello", "World", GEN_FORMAT_COUNT(2),
            'a', 'b', GEN_FORMAT_COUNT(3),
            64362358ull, 98978, -5ll, -4, test);
    if(error) gen_error_abort();

    char* x = calloc(sz + 1, sizeof(char));
    if(!x) gen_error_abort();

    error = gen_format(
            x, GEN_NULL, GEN_SIZE_MAX, "%t %tz %c %cz %uz %ui %sz %si %e",
            "Hello", "World", GEN_FORMAT_COUNT(2),
            'a', 'b', GEN_FORMAT_COUNT(3),
            64362358ull, 98978, -5ll, -4, test);
    if(error) gen_error_abort();

    error = gen_log(GEN_LOG_LEVEL_INFO, "main", "%t", x);
    if(error) gen_error_abort();

    free(x);

    return GEN_NULL;
}

int main(void) {
    gen_tooling_push(GEN_FUNCTION_NAME, GEN_FILE_NAME);
    GEN_TOOLING_AUTO gen_error_t* error = gen_main();
    if(error) {
        gen_error_abort();
    }
}
