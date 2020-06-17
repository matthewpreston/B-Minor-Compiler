#!/bin/bash
# Tests B Minor compiler against test cases found in Tests/

for entry in $(ls Tests/*.bminor); do
	echo "Testing: $entry";
	./bminor "$entry";
	
	# Test if an non-user defined error occurred (i.e. segmentation fault)
	return_code=$?
	if [ $return_code -ne 0 ] && [ $return_code -ne 1 ]; then
		echo "Non-user defined error occurred, exit code $return_code";
		exit $?; # Exit with same return code
	fi
	echo;
done
