# efind-text-tools

## Introduction

This extension for [efind](https://github.com/20centaurifux/efind) makes it
possible to filter text files by their content.

## Available functions

### text\_contains(string: query)

Tests if the text file contains *query*. Returns the line number of the
first occurrence.

	$ efind . 'text_contains("foo") or text_contains("bar")'

### head\_contains(string: query, integer: limit)

Tests if the first *limit* lines of the text file contain *query*. Returns
the line number of the first occurrence.

	$ efind . 'head_contains("baz", 20)'

### tail\_contains(string: query, integer: limit)

Tests if the last *limit* lines of the text file contain *query*. Returns
the line number of the first occurrence.

	$ efind . 'tail_contains("foobar", 20)'

### text\_startswith(string: query)

Tests if the text file contains a line starting with *query*. Returns
the line number of the first occurrence.

	$ efind . 'text_startswith("foo") or text_contains("bar")'

### head\_startswith(string: query, integer: limit)

Tests if the first *limit* lines of text file contain a line starting
with  *query*. Returns the line number of the first occurrence.

	$ efind . 'head_startswith("baz", 20)'

### tail\_startswith(string: query, integer: limit)

Tests if the last *limit* lines of text file contain a line starting
with *query*. Returns the line number of the first occurrence.

	$ efind . 'tail_startswith("foobar", 20)'

### text\_endswith(string: query)

Tests if the text file contains a line ending with *query*. Returns
the line number of the first occurrence.

	$ efind . 'text_endswith("foo") or text_contains("bar")'

### head\_endswith(string: query, integer: limit)

Tests if the first *limit* lines of text file contain a line ending
with  *query*. Returns the line number of the first occurrence.

	$ efind . 'head_endswith("baz", 20)'

### tail\_endswith(string: query, integer: limit)

Tests if the last *limit* lines of text file contain a line ending
with  *query*. Returns the line number of the first occurrence.

	$ efind . 'tail_endswith("foobar", 20)'

### count\_lines(string: query)

Returns the number of lines.

	$ efind . 'count_lines()>100'

### count\_match(string: query)

Returns the number of lines containing *query*.

	$ efind . 'count_match("foo")=5'

### count\_prefix(string: query)

Returns the number of lines starting with *query*.

	$ efind . 'count_prefix("bar")>=17'

### count\_suffix(string: query)

Returns the number of lines ending with *query*.

	$ efind . 'count_suffix("baz")=100'

## Installation

Ensure that GNU Make and GCC (or Clang) is installed on your system. Then
run

	$ make
	$ sudo make install
