#pragma once

#include <stdio.h>
#include <string.h>

#include "dynamic-string.c"

typedef enum {
    ERROR,
    GET,
    HEAD,
} Method;

typedef struct {
    Method method;
    DynamicString url;
    DynamicString version;
} Request;

bool parse_method(String str, Method *method) {
    if (String_eq(str, String_from_cstr("GET"))) {
      *method = GET;
    } else if (String_eq(str, String_from_cstr("HEAD"))) {
      *method = HEAD;
    } else {
      // TODO: log unknown header
      return false;
    }
 
    return true;
}

bool process_request(Arena *arena, FILE *stream, Request *req) 
{
    char *linebuf = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    if ((linelen = getline(&linebuf, &linecap, stream)) < 0) return perror_false("getline");

    String token;
    String line = String_from(linebuf, linelen);

    // parse method
    if (!String_try_chop_by_delim(&line, ' ', &token)) return error_false("Cannot get method");
    parse_method(token, &req->method); 

    // parse URL
    if (!String_try_chop_by_delim(&line, ' ', &token)) return error_false("Cannot get url");
    if (!ds_arena_clone(arena, &req->url, &token)) return false;

    // parse version
    if (!String_try_chop_by_delim(&line, '\n', &token)) return error_false("Cannot get version");
    if (!ds_arena_clone(arena, &req->version, &token)) return false;


    while ((linelen = getline(&linebuf, &linecap, stream)) > 0) {
        if (strcmp("\r\n", linebuf)) break;

        line = String_from(linebuf, linelen);
    }

    free(linebuf);
    if (fclose(stream) == EOF) return perror_false("fclose");
    if (ferror(stream)) return perror_false("getline");

    return true;
}

void print_request(Request *req) 
{
    printf("\n");
    printf("(Request)  Method: %d\n", req->method);
    printf("(Request)     URL: %.*s\n", req->url.len, req->url.ptr);
    printf("(Request) Version: %.*s\n", req->version.len, req->version.ptr);
    printf("\n");
}
