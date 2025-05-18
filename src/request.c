#pragma once

#include <stdio.h>
#include <string.h>

#include "error.c"
#include "string.c"
#include "stack-arena.c"

typedef enum {
    UNKNOWN,
    GET,
    HEAD,
} Method;

typedef struct {
    Method method;
    String url;
    String version;
} Request;

bool parse_method(String str, Method *method) {
    if (str_eq_cstr(str, "GET")) {
        *method = GET;
    } else if (str_eq_cstr(str, "HEAD")) {
        *method = HEAD;
    } else {
	*method = UNKNOWN;
    }
 
    return true;
}

const char *get_method_cstr(Method method)
{
    switch (method) {
	case HEAD:
	    return "HEAD";
	case GET:
	    return "GET";
	case UNKNOWN:
	    return "UNKNOWN";
	default:
	    error_fail("Unknown method in get_method_cstr");
    }
}

bool process_request(StackArena *sa, FILE *stream, Request *req)
{
    char *linebuf = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    if ((linelen = getline(&linebuf, &linecap, stream)) < 0) return perror_false("getline");

#ifdef LOG_REQUEST
	fprintf(stderr, "\n");
	fprintf(stderr, "============= Request =============\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "%s", linebuf);
#endif // LOG_REQUEST

    String token;
    String line = str_from(linebuf, linelen);
    // parse method
    if (!str_try_chop_by_delim(&line, ' ', &token)) return error_false("Cannot get method");
    parse_method(token, &req->method); 

    // parse URL
    if (!str_try_chop_by_delim(&line, ' ', &token)) return error_false("Cannot get url");
    if (!str_clone_with_stack_arena(sa, &req->url, &token)) return false;

    // parse version
    if (!str_try_chop_by_delim(&line, '\n', &token)) return error_false("Cannot get version");
    if (!str_clone_with_stack_arena(sa, &req->version, &token)) return false;

    while ((linelen = getline(&linebuf, &linecap, stream)) > 0) {
#ifdef LOG_REQUEST
	fprintf(stderr,"%s", linebuf);
#endif // LOG_REQUEST

        if (strcmp("\r\n", linebuf) == 0) break;

        line = str_from(linebuf, linelen);
    }

    free(linebuf);
    linebuf = NULL;

    if (ferror(stream) < 0) return perror_false("getline");

    return true;
}

void print_request(Request *req) 
{
    fprintf(stderr, "============= Parsed Response =============\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Version: %.*s\n", req->version.len, req->version.ptr);
    fprintf(stderr, " Method: %s (%d)\n", get_method_cstr(req->method), req->method);
    fprintf(stderr, "    URL: %.*s\n", req->url.len, req->url.ptr);
    fprintf(stderr, "\n");
}
