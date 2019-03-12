//
// Created by wulei on 19-3-12.
//
// read & parse

#include "header/read.h"
#include "header/error.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAXSIZE 1024

char* read(FILE* in_stream) { /* from in_stream to buffer */
    char *buf, ch;
    int i = 0;

    buf = (char*) malloc(MAXSIZE * sizeof(char));
    if(buf == NULL)
        error_handle(stderr, "out of memory", EXIT_FAILURE);

    while((ch = getc(in_stream)) != EOF && ch != '\n') {
        buf[i++] = ch;

        if(i >= MAXSIZE) {
            buf = (char*) realloc(buf, MAXSIZE * 10 * sizeof(char));
            if(buf == NULL)
                error_handle(stderr, "out of memory", EXIT_FAILURE);
        }
    }
    buf[i] = '\0';

    return buf;
}

char* buf_pre_handle(char* pre_buf) { /* add space and remove comments */
    char* buf;
    buf = (char*) malloc(MAXSIZE * sizeof(char));
    if(buf == NULL)
        error_handle(stderr, "out of memory", EXIT_FAILURE);

    int buf_i = 0, pbuf_i = 0;
    while(pre_buf[pbuf_i] != '\0') {
        if(pre_buf[pbuf_i] == '(') {
            buf[buf_i++] = ' ';
            buf[buf_i++] = '(';
            buf[buf_i++] = ' ';
            pbuf_i++;
        }
        if(pre_buf[pbuf_i] == ')') {
            buf[buf_i++] = ' ';
            buf[buf_i++] = ')';
            buf[buf_i++] = ' ';
            pbuf_i++;
        }
        if(pre_buf[pbuf_i] == ';') {
            while(pre_buf[pbuf_i] != '\n')
                pbuf_i++;
        }
        buf[buf_i++] = pre_buf[pbuf_i++];

        if(buf_i > MAXSIZE - 3) {
            buf = (char*) realloc(buf, MAXSIZE * 10 * sizeof(char));
            if(buf == NULL)
                error_handle(stderr, "out of memory", EXIT_FAILURE);
        }
    }
    buf[buf_i] = '\0';
    return buf;
}

token* gen_token(char* buf) {
    token* token_list, * token_p; /*init token list with head token*/
    token_list = (token*) malloc(sizeof(token));
    if(token_list == NULL)
        error_handle(stderr, "out of memory while parse token", EXIT_FAILURE);
    token_list->value = "Head Token";
    token_p = token_list;

    int i = 0, j = 0;
    while(1) {
        token* t = (token*) malloc(sizeof(token));
        t->value = (char*) malloc(TOKEN_MAX * sizeof(char));
        if(t == NULL || t->value == NULL)
            error_handle(stderr, "out of memory while parse token", EXIT_FAILURE);
        t->next = NULL;

        int len = strlen(buf);
        for(; buf[i] == ' ' || buf[i] == '\n'; i++)
            if(i >= strlen(buf) - 1 )
                return token_list;

        for(j = i; buf[j] != ' '; j++);
        if(j - i >= TOKEN_MAX) {
            t->value = (char*) realloc(t->value, TOKEN_MAX * 10 * sizeof(char));
            if(t->value == NULL) {
                error_handle(stderr, "out of memory while parse token", EXIT_FAILURE);
            }
        }

        int t_i = 0;
        for(int k = i; k < j; k++, t_i++)
            t->value[t_i] = buf[k];
        t->value[t_i] = '\0';
        i = j;

        token_p->next = t;
        token_p = t;
    }

}