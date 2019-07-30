"""
    project............: efind-text-tools
    description........: efind-text-tools test suite.
    copyright..........: Sebastian Fedrau

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License v3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License v3 for more details.
"""
import subprocess, os, random, string

def test_search(argv, expected, success=True):
    cmd = ["efind", "books"] + argv

    print("Running efind, argv=[%s]" % ", ".join(cmd[1:]))

    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    result = list(filter(lambda l: l != "", proc.stdout.read().decode("utf-8").split("\n")))
    proc.wait()

    if not expected is None:
        assert(set(result) == set(expected))

    assert((success and proc.returncode == 0) or (not success and proc.returncode != 0))

def random_string(length=32):
    return "".join(random.choice(string.ascii_uppercase + string.digits) for _ in range(length))

INVALID_SEARCH_ARGS = [['text_contains("%s") or tail_startswith("%s")' % (random_string(), random_string(512))],
                       ['text_contains(41231) and count_lines="%s"' % random_string(2048)],
                       ['count_prefix("%s")="%s"' % (random_string(), random_string())]]

SEARCH_ARGS = [[['name="*.txt" and text_contains("Peter") and text_contains("Pan")'], ["books/2701-0.txt", "books/4300-0.txt", "books/829-0.txt", "books/16-0.txt"]],
               [['type=file and head_contains("PRIDE", 40) or head_contains("CRUSOE", 50)'], ["books/521-0.txt", "books/1342-0.txt"]],
               [['type=file and count_lines()>23000'], ["books/996.txt.utf-8", "books/4300-0.txt"]],
               [['type=file and tail_contains("Wendy", 1000)'], ["books/16-0.txt"]],
               [['type=file and (count_match("Chapter") = 61 or count_prefix("the") = 1101) and count_suffix("in")>0'], ["books/1342-0.txt"]],
               [['type=file and head_startswith("Author", 20) and tail_endswith("in", 20)'], ["books/521-0.txt", "books/4300-0.txt"]],
               [['type=file and text_startswith("Author") and text_endswith("Pan")'], ["books/16-0.txt"]],
               [['type=file and head_startswith("diligently", 5000) or tail_startswith("Accordingly,", 5000)'], ["books/521-0.txt", "books/2701-0.txt", "books/829-0.txt"]]]

if __name__ == "__main__":
    for argv, expected in SEARCH_ARGS:
        test_search(argv, expected)

    for argv in INVALID_SEARCH_ARGS:
        test_search(argv, None, False)
