#include <genarguments.h>
#include <gencommon.h>
#include <gendynamiclibrary.h>
#include <generror.h>
#include <genfilesystem.h>
#include <genmathtypes.h>
#include <genmemory.h>
#include <genprocess.h>
#include <genstring.h>
#include <genthreads.h>
#include <gentooling.h>
#include <genlog.h>

static gen_error_t* gen_main(int argc, const char* const restrict * const restrict argv) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_main, GEN_FILE_NAME);
	if(error) return error;
 
    size_t* argument_lengths = NULL;
    error = gen_memory_allocate_zeroed((void**) &argument_lengths, argc - 1, sizeof(size_t));
	if(error) return error;

    for(size_t i = 0; i < argc - 1; ++i) {
        error = gen_string_length((argv + 1)[i], GEN_STRING_NO_BOUNDS, GEN_STRING_NO_BOUNDS, &argument_lengths[i]);
    	if(error) return error;
    }

    static const char short_args[] = {'p', 'h'};
    static const char* long_args[] = {"print", "help"};
    static const size_t long_args_lengths[] = {5, 4};

    gen_arguments_parsed_t parsed = {0};
    error = gen_arguments_parse(argv + 1, argument_lengths, argc - 1, short_args, sizeof(short_args), long_args, long_args_lengths, sizeof(long_args) / sizeof(char*), &parsed);
	if(error) return error;

    for(size_t i = 0; i < parsed.short_argument_count; ++i) {
        if(!parsed.short_argument_parameters[i]) gen_log_formatted(GEN_LOG_LEVEL_INFO, "test-application", "-%c", short_args[parsed.short_argument_indices[i]]);
        else gen_log_formatted(GEN_LOG_LEVEL_INFO, "test-application", "-%c => %tz", short_args[parsed.short_argument_indices[i]], parsed.short_argument_parameters[i], parsed.short_argument_parameter_lengths[i]);
    }
    for(size_t i = 0; i < parsed.long_argument_count; ++i) {
        if(!parsed.long_argument_parameters[i]) gen_log_formatted(GEN_LOG_LEVEL_INFO, "test-application", "--%tz", long_args[parsed.long_argument_indices[i]], long_args_lengths[parsed.long_argument_indices[i]] + 1);
        else gen_log_formatted(GEN_LOG_LEVEL_INFO, "test-application", "--%tz => %tz", long_args[parsed.long_argument_indices[i]], long_args_lengths[parsed.long_argument_indices[i]] + 1, parsed.long_argument_parameters[i], parsed.long_argument_parameter_lengths[i]);
    }
    for(size_t i = 0; i < parsed.raw_argument_count; ++i) {
        gen_log_formatted(GEN_LOG_LEVEL_INFO, "test-application", "%tz", (argv + 1)[parsed.raw_argument_indices[i]], long_args_lengths[parsed.raw_argument_indices[i]]);
    }

    error = gen_arguments_free_parsed(&parsed);
	if(error) return error;

    error = gen_log(GEN_LOG_LEVEL_INFO, "test-application", "Hello, world!\n");
	if(error) return error;

    gen_log(GEN_LOG_LEVEL_TRACE, "test-application", "This is a test");
    gen_log(GEN_LOG_LEVEL_DEBUG, "test-application", "This is a test");
    gen_log(GEN_LOG_LEVEL_INFO, "test-application", "This is a test");
    gen_log(GEN_LOG_LEVEL_WARNING, "test-application", "This is a test");
    gen_log(GEN_LOG_LEVEL_ERROR, "test-application", "This is a test");
    gen_log(GEN_LOG_LEVEL_FATAL, "test-application", "This is a test\n");

    gen_error_t* test_error = gen_error_attach_backtrace(GEN_ERROR_UNKNOWN, GEN_LINE_NUMBER, "A bad thing happened");
    gen_error_print("test-application", test_error, GEN_ERROR_SEVERITY_WARNING);

    gen_log(GEN_LOG_LEVEL_INFO, "test-application", "Errors seem to work!\n");

    error = gen_log_formatted(GEN_LOG_LEVEL_INFO, "test-application", "Blargh %t", NULL);
	if(error) return error;

    return NULL;
}

int main(int argc, char** argv) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) main, GEN_FILE_NAME);
	if(error) {
        gen_error_print("test-application", error, GEN_ERROR_SEVERITY_FATAL);
        gen_error_abort();
    }
    
    GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
    GEN_PRAGMA(GEN_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
    error = gen_main(argc, argv);
    GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)
    if(error) {
        gen_error_print("test-application", error, GEN_ERROR_SEVERITY_FATAL);
        gen_error_abort();
    }
}
