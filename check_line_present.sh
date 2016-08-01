#!/bin/bash

# check which lines of the first passed file are present also in the second
# NOTE: the main mechanism is using grep
# all but the first two arguments are passed to grep 'verbatim', -x is already set
# leading and trailing whitespaces are ignored by pre-/appending the regex searched by grep

# input arguments handling and basic error checking
if [ $# -lt 2 ]; then
  echo "Need at least two arguments. First is a file with lines that are then searched in the file specified by the second"
fi
# remove the files from the argument list to be able to pass it on to grep in the next step
infile=${1}; shift
checkfile=${1}; shift

if [ $# -gt 0 ]; then
  echo "options passed to grep: "$@
fi

## actual work is done here
while read line; do
  echo ${line}" is present "$(grep -xc $@ "\s*${line}\s*" ${checkfile})" times"
done < ${infile}
