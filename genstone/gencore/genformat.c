// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genformat.h"

gen_error_t* gen_format(
        char* const restrict out_buffer, gen_size_t* const restrict out_len,
        const gen_size_t limit, const char* const restrict format, ...) {

    gen_tooling_push(GEN_FUNCTION_NAME, GEN_FILE_NAME);
    GEN_TOOLING_AUTO gen_error_t* error;

    GEN_VARIADIC_LIST_AUTO gen_variadic_list_t list;
    gen_variadic_list_start(list, format);

    return gen_format_variadic_list(out_buffer, out_len, limit, format, list);
}

// NOTE: Be very careful with errors emitted here as a bad format specifier
//       Can cause an infinite recurse
gen_error_t* gen_format_variadic_list(
        char* const restrict out_buffer, gen_size_t* const restrict out_len,
        const gen_size_t limit, const char* const restrict format,
        gen_variadic_list_t list) {

    gen_tooling_push(GEN_FUNCTION_NAME, GEN_FILE_NAME);
    GEN_TOOLING_AUTO gen_error_t* error;

    if(!format) {
        return gen_error_attach_backtrace(
                GEN_ERROR_INVALID_PARAMETER, GEN_LINE_STRING,
                "`format` was `GEN_NULL`");
    }

    gen_size_t pos = 0;

    for(gen_size_t i = 0; format[i]; ++i) {
        char c = format[i];
        if(c != '%') {
            if(out_buffer && pos < limit) out_buffer[pos] = c;
            ++pos;
            continue;
        }

        gen_bool_t sign = gen_false;
        switch(format[++i]) {
            default: {
                return gen_error_attach_backtrace(
                        GEN_ERROR_BAD_CONTENT, GEN_LINE_STRING,
                        "Invalid format specifier at position %uz", i);
            }

            case '%': {
                if(out_buffer && pos < limit) out_buffer[pos] = c;
                ++pos;

                break;
            }

            case 's': sign = gen_true; GEN_FALLTHROUGH;
            case 'u': {
                gen_size_t x;
                gen_bool_t wide = gen_true;
                switch(format[++i]) {
                    default: {
                        return gen_error_attach_backtrace(
                                GEN_ERROR_BAD_CONTENT, GEN_LINE_STRING,
                                "Invalid format specifier at position %uz", i);
                    }

                    case 'l': GEN_FALLTHROUGH;
                    case 'z': {
                        x = gen_variadic_list_argument(list, gen_ulong_t);
                        break;
                    }

                    case 'c': GEN_FALLTHROUGH;
                    case 's': GEN_FALLTHROUGH;
                    case 'i': {
                        wide = gen_false;
                        x = gen_variadic_list_argument(list, gen_uint_t);
                        break;
                    }
                }

                // "18446744073709551616" -> 20 chars
                char numbuf[20] = {0};
                gen_size_t numbuf_pos = 0;

                sign = sign && (
                   (wide && (gen_ssize_t) x < 0) ||
                   (!wide && (int) x < 0)
                );
                if(sign && wide) x = ~x + 1;
                if(sign && !wide) x = ~(gen_uint_t)x + 1;

                do {
                    numbuf[numbuf_pos++] = (char) ('0' + (x % 10));
                    x /= 10;
                } while(x);

                if(sign) numbuf[numbuf_pos++] = '-';

                for(gen_size_t j = 1; j <= numbuf_pos; ++j) {
                    if(out_buffer && pos < limit) {
                        out_buffer[pos] = numbuf[numbuf_pos - j];
                    }
                    ++pos;
                }

                break;
            }

            case 'p': {
                gen_uintptr_t p = gen_variadic_list_argument(list, gen_uintptr_t);

                // "FFFFFFFFFFFFFFFF" -> 16
                char buf[16];
                for(gen_size_t j = 0; j < sizeof(buf); ++j) {
                    buf[i] = "0123456789ABCDEF"[p % 16];
                    p /= 16;
                }

                for(gen_size_t j = 1; j <= sizeof(buf); ++j) {
                    if(out_buffer && pos < limit) {
                        out_buffer[pos] = buf[sizeof(buf) - j];
                    }
                    ++pos;
                }

                break;
            }

            case 'e': {
                gen_error_t* p_error =
                        gen_variadic_list_argument(list, gen_error_t*);

                // GEN_ERROR_BLAH (A blah occurred): "I died!" at `foo.c:43`
                const char* s[] = {
                    gen_error_type_name(p_error->type),
                    " (", gen_error_type_description(p_error->type), "): \"",
                    p_error->context, "\" at `",
                    p_error->backtrace[p_error->backtrace_length - 1].file,
                    ":", p_error->line, "`"
                };

                for(gen_size_t j = 0; j < GEN_ARRAY_LENGTH(s); ++j) {
                    for(gen_size_t k = 0; s[j][k]; ++k) {
                        if(out_buffer && pos < limit) {
                            out_buffer[pos] = s[j][k];
                        }
                        ++pos;
                    }
                }

                break;
            }

            case 'c': {
                const int a = gen_variadic_list_argument(list, int);

                gen_format_count_t count = 1;
                if(format[++i] == 'z') {
                    count = gen_variadic_list_argument(
                                    list, gen_format_count_t);
                }
                else --i;

                for(gen_size_t j = 0; j < count; ++j) {
                    if(out_buffer && pos < limit) out_buffer[pos] = (char) a;
                    ++pos;
                }

                break;
            }

            case 't': {
                const char* s = gen_variadic_list_argument(list, const char*);

                gen_format_count_t s_limit = GEN_SIZE_MAX;
                if(format[++i] == 'z') {
                    s_limit = gen_variadic_list_argument(
                                list, gen_format_count_t);
                }
                else --i;

                for(gen_size_t j = 0; j < s_limit && s[j]; ++j) {
                    if(out_buffer && pos < limit) out_buffer[pos] = s[j];
                    ++pos;
                }

                break;
            }

            case 'f': {
//                switch(format[++i]) {
//                    default: {
//                        return gen_error_attach_backtrace(
//                            GEN_ERROR_BAD_CONTENT, GEN_LINE_STRING,
//                            "Invalid format specifier at position %uz", i);
//                    }
//
//                    case 's': // float
//                    case 'd': // double
//                    case 'e': // long double
//                }
                return gen_error_attach_backtrace(
                        GEN_ERROR_NOT_IMPLEMENTED,
                        GEN_LINE_STRING,
                        "Format specifier `f` not yet implemented");
            }
        }
    }

    if(out_len) *out_len = pos;

    return GEN_NULL;
}
