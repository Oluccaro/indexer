#!/bin/bash
file_total="";
for file in testes/*.txt; do
  file_total+=$file" ";
done

echo $file_total;