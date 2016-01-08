#! /bin/bash

set -e -u -o pipefail

tempfile="$(mktemp)"

for infile in vectors/*.in ; do
  outfile="${infile%.in}.out"
  ./base62 < "$infile" > "$tempfile"
  cmp "$outfile" "$tempfile"
done

for outfile in vectors/*.out ; do
  infile="${outfile%.out}.in"
  ./base62 -d < "$outfile" > "$tempfile"
  cmp "$infile" "$tempfile"
done
