/***************************************************************************
    begin........: June 2017
    copyright....: Sebastian Fedrau
    email........: sebastian.fedrau@gmail.com
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License v3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License v3 for more details.
 ***************************************************************************/

/***************************************************************************
   Tools for searching text files.
 ***************************************************************************/
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#include "extension-interface.h"

#define NAME        "Text-Tools"
#define VERSION     "0.1.0"
#define DESCRIPTION "Tools for searching text files."

void
registration(RegistrationCtx *ctx, RegisterExtension fn)
{
	fn(ctx, NAME, VERSION, DESCRIPTION);
}

void
discover(RegistrationCtx *ctx, RegisterCallback fn)
{
	fn(ctx, "text_contains", 1, CALLBACK_ARG_TYPE_STRING);
	fn(ctx, "head_contains", 2, CALLBACK_ARG_TYPE_STRING, CALLBACK_ARG_TYPE_INTEGER);
	fn(ctx, "tail_contains", 2, CALLBACK_ARG_TYPE_STRING, CALLBACK_ARG_TYPE_INTEGER);
	fn(ctx, "text_startswith", 1, CALLBACK_ARG_TYPE_STRING);
	fn(ctx, "head_startswith", 2, CALLBACK_ARG_TYPE_STRING, CALLBACK_ARG_TYPE_INTEGER);
	fn(ctx, "tail_startswith", 2, CALLBACK_ARG_TYPE_STRING, CALLBACK_ARG_TYPE_INTEGER);
	fn(ctx, "text_endswith", 1, CALLBACK_ARG_TYPE_STRING);
	fn(ctx, "head_endswith", 2, CALLBACK_ARG_TYPE_STRING, CALLBACK_ARG_TYPE_INTEGER);
	fn(ctx, "tail_endswith", 2, CALLBACK_ARG_TYPE_STRING, CALLBACK_ARG_TYPE_INTEGER);
}

enum { SEEK_RESULT_ABORT, SEEK_RESULT_CONTINUE, SEEK_RESULT_FOUND };

static int
_seek_and_count(FILE *fp, size_t pos, char *buffer, size_t buf_len, int *lines, int tail)
{
	size_t read;

	/* seek to `pos` */
	if(fseek(fp, pos, SEEK_SET) < 0)
	{
		perror("fseek()");
		return SEEK_RESULT_ABORT;
	}

	/* fill buffer */
	read = fread(buffer, 1, buf_len, fp);

	if(read < 0)
	{
		perror("fread()");
		return SEEK_RESULT_ABORT;
	}

	if(read != buf_len)
	{
		fprintf(stderr, "read() failed, expected %ld bytes but received only %ld\n", buf_len, read);
		return SEEK_RESULT_ABORT;
	}

	/* count line-break */
	for(int i = buf_len - 1; i >= 0; --i)
	{
		if(buffer[i] == '\n')
		{
			if(*lines == tail)
			{
				fseek(fp, pos + i + 1, SEEK_SET);
				return SEEK_RESULT_FOUND;
			}
	
			++*lines;
		}
	}

	return SEEK_RESULT_CONTINUE;
}

static bool
_seek_tail(FILE *fp, int tail)
{
	char buffer[8192];
	size_t size;
	int lines = 0;
	size_t offset = 0;
	size_t blocks_n;
	bool success = false;
	int result;

	/* get file size */
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	/* how often can we fill the buffer? */
	blocks_n = size / sizeof(buffer);
	result = SEEK_RESULT_CONTINUE;

	for(size_t i = 1; i <= blocks_n && result == SEEK_RESULT_CONTINUE; ++i)
	{
		/* fill buffer until `tail` line-breaks have been found */
		offset = size - sizeof(buffer) * i;
		result = _seek_and_count(fp, offset, buffer, sizeof(buffer), &lines, tail);
	}

	if(result == SEEK_RESULT_CONTINUE)
	{
		/* not enough line-breaks found... */
		size_t until = offset;

		if(blocks_n)
		{
			/* last block is smaller than buffer */
			until = offset;
		}
		else
		{
			/* buffer is larger than file */
			until = size;
		}

		result = _seek_and_count(fp, 0, buffer, until, &lines, tail);
	}

	/* not enough line-breaks found, document contains less than `tail` lines */
	if(result == SEEK_RESULT_CONTINUE || lines < tail)
	{
		fseek(fp, 0, SEEK_SET);
		success = true;
	}
	else
	{
		success = result == SEEK_RESULT_FOUND;
	}

	return success;
}

enum { SEARCH_START, SEARCH_ALL, SEARCH_END };

int
_text_search(const char *filename, const char *query, int mode, int limit)
{
	FILE *fp;
	int found = 0;

	if(!query || !*query)
	{
		return 0;
	}

	if((fp = fopen(filename, "r")))
	{
		char *line = (char *)malloc(128);
		ssize_t bytes;
		size_t size = sizeof(line);
		int lineno = 0;
		size_t query_len = strlen(query);

		if(limit < 0)
		{
			/* user wants only to read last `-limit` lines */
			if(!_seek_tail(fp, -limit))
			{
				return 0;
			}
		}

		do
		{
			bytes = getline(&line, &size, fp);

			if(lineno < INT_MAX)
			{
				++lineno;
			}
			else
			{
				fprintf(stderr, "%s: integer overflow detected.\n", __func__);
			}

			/* user wants only to read first `limit` lines */
			if(limit > 0 && lineno > limit)
			{
				break;
			}

			if(bytes > 0)
			{
				size_t line_len;

				if(bytes > 2 && line[bytes - 2] == '\r')
				{
					line[bytes - 2] = '\0';
					line_len = bytes - 2;
				}
				else
				{
					line[bytes - 1] = '\0';
					line_len = bytes - 1;
				}

				switch(mode)
				{
					case SEARCH_START:
						if(line_len >= query_len && !strncmp(line, query, query_len))
						{
							found = lineno;
						}
						break;

					case SEARCH_ALL:
						if(strstr(line, query))
						{
							found = lineno;
						}
						break;

					case SEARCH_END:
						if(line_len >= query_len && !strncmp(line + (line_len - query_len), query, query_len))
						{
							found = lineno;
						}
						break;
				}
			}
		} while(!found && bytes > 0);

		free(line);
		fclose(fp);
	}
	else
	{
		perror("fopen()");
	}

	return found;
}

int
text_contains(const char *filename, int argc, char *argv[])
{
	return _text_search(filename, *argv, SEARCH_ALL, 0);
}

int
head_contains(const char *filename, int argc, char *argv[])
{
	char *query = argv[0];
	int head = *((int *)argv[1]);

	return _text_search(filename, query, SEARCH_ALL, head);
}

int
tail_contains(const char *filename, int argc, char *argv[])
{
	char *query = argv[0];
	int tail = *((int *)argv[1]);

	return _text_search(filename, query, SEARCH_ALL, -tail);
}

int
text_startswith(const char *filename, int argc, char *argv[])
{
	return _text_search(filename, *argv, SEARCH_START, 0);
}

int
head_startswith(const char *filename, int argc, char *argv[])
{
	char *query = argv[0];
	int head = *((int *)argv[1]);

	return _text_search(filename, query, SEARCH_START, head);
}

int
tail_startswith(const char *filename, int argc, char *argv[])
{
	char *query = argv[0];
	int tail = *((int *)argv[1]);

	return _text_search(filename, query, SEARCH_START, -tail);
}

int
text_endswith(const char *filename, int argc, char *argv[])
{
	return _text_search(filename, *argv, SEARCH_END, 0);
}

int
head_endswith(const char *filename, int argc, char *argv[])
{
	char *query = argv[0];
	int head = *((int *)argv[1]);

	return _text_search(filename, query, SEARCH_END, head);
}

int
tail_endswith(const char *filename, int argc, char *argv[])
{
	char *query = argv[0];
	int tail = *((int *)argv[1]);

	return _text_search(filename, query, SEARCH_END, -tail);
}

