#!/bin/sh
i=0;
for icon in $(ls)
do
echo "<file alias=\"icon"$i"\">IconsPNG/"$icon"</file>"
i=$((i+1));
done
