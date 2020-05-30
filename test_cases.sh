#!/bin/bash
# Tests B Minor compiler against test cases found in Tests/

for entry in $(ls Tests); do
	echo "Testing: $entry";
	./bminor "Tests/$entry";
done
