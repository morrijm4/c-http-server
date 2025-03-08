#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "dynamic-string.c"
#include "request.c"
#include "arena.c"

typedef uint16_t StatusCode;

typedef struct {
    StatusCode code;
    const char *msg;
} Status;

typedef struct {
    Status status;
    DynamicString body;
    DynamicString contentType;
} Response;

bool create_status(StatusCode code, Response *res) 
{
    switch (code) {
	case 200:
	    res->status.msg = "OK";
	    break;
	case 204:
	    res->status.msg = "No Content";
	    break;

	case 400:
	    res->status.msg = "Bad Request";
	    break;

	default:
	    fprintf(stderr, "Unknown status code %d\n", code);
	    return false;
    }

    res->status.code = code;
    return true;
}

bool create_response(Arena *arena, Request *req, Response *res)
{
    if (!create_status(200, res)) return false;

    ds_arena_clone_cstr(arena, &res->body, "<div>Hello world<div>");
    ds_arena_clone_cstr(arena, &res->contentType, "text/html");

    return true;
}

bool write_response(FILE* stream, Response *res) {
    fprintf(stream, "HTTP/1.1 %d %s\r\n", res->status.code, res->status.msg);
    fprintf(stream, "Content-Type: %.*s\r\n", res->contentType.len, res->contentType.ptr);
    fprintf(stream, "Content-Length: %d\r\n", res->body.len);
    fprintf(stream, "\r\n");
    fprintf(stream, "%.*s", res->body.len, res->body.ptr);
    return true;
}

bool send_response(Arena *arena, FILE *stream, Request *req) {
    Response res = {0};
    if (!create_response(arena, req, &res)) return false;

    write_response(stderr, &res);
    fprintf(stderr, "\n");

    write_response(stream, &res);

    return true;
}

