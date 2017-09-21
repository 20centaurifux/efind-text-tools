#!/bin/bash
#
#  efind-text-tools test suite.
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License v3 as published by
#  the Free Software Foundation.
#
#  This program is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#  General Public License v3 for more details.

PYTHON=/usr/bin/python2

download_books()
{
	echo "Downloading books"

	if [ ! -d ./books ]; then
		mkdir ./books
	fi

	while read -r f
	do
		local dst=./books/$f

		if [ ! -f $dst ]; then
			local n=$(echo $f | sed -r "s/[a-z]*([0-9]*).*\.txt/\1/")

			wget -P ./books "http://www.gutenberg.org/files/$n/$f"

			if [ $? -ne 0 ]; then
				wget -P ./books "http://www.gutenberg.org/ebooks/$f"
			fi
		fi

	done < SOURCES
}
run_test()
{
	# run Python script:
	echo "Running test suite."
        $PYTHON ./test.py
}

cleanup()
{
	# delete generated test files:
	echo "Deleting test files."
	rm -fr ./books
}

download_books && run_test && cleanup
