#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "request.c"
#include "arena.c"

typedef uint16_t StatusCode;

typedef struct {
    StatusCode code;
    const char *msg;
} Status;

typedef struct {
    Status status;
    String body;
    String contentType;
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
	case 501:
	    res->status.msg = "Not implemented";
	    break;;
	default:
	    fprintf(stderr, "Unknown status code %d\n", code);
	    return false;
    }

    res->status.code = code;
    return true;
}

bool create_501_not_implemented_response(Response *res)
{
    if (!create_status(501, res)) return false;
    return true;
}

bool create_response(Arena *arena, Request *req, Response *res)
{
    if (req->method == UNKNOWN) {
	create_501_not_implemented_response(res);
	return true;
    }

    if (!create_status(200, res)) return false;

    str_clone_cstr_with_arena(arena, &res->body, "<div>Hello world<div>");
    str_clone_cstr_with_arena(arena, &res->contentType, "text/html");

    return true;
}

bool write_response(FILE* stream, Response *res) {
    fprintf(stream, "HTTP/1.1 %d %s\r\n", res->status.code, res->status.msg);
    // fprintf(stream, "Content-Type: %.*s\r\n", res->contentType.len, res->contentType.ptr);
    // fprintf(stream, "Content-Length: %d\r\n", res->body.len);
    fprintf(stream, "\r\n");
    fprintf(stream, "%.*s", res->body.len, res->body.ptr);
    return true;
}

bool send_response(Arena *arena, FILE *stream, Request *req) {
    Response res = {0};
    if (!create_response(arena, req, &res)) return false;

#ifdef LOG_RESPONSE
    fprintf(stderr, "============= Response =============\n");
    fprintf(stderr, "\n");
    write_response(stderr, &res);
    fprintf(stderr, "\n");
#endif // LOG_RESPONSE

    write_response(stream, &res);

    return true;
}

