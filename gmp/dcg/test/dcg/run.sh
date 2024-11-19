#!/bin/bash

while read l; do
  echo "$l $(shuf -i 0-100 -n 1)"
done<sbig.dcg
