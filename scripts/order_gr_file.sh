#!/bin/bash

GR_FILE=$1

# Temp files
tmp_dir=$(mktemp -d)
trap 'rm -rf "$tmp_dir"' EXIT
comments="$tmp_dir/comments"
problem="$tmp_dir/problem"
edges="$tmp_dir/edges"
sorted_edges="$tmp_dir/sorted_edges"

# Initialize temp files
> "$comments"
> "$problem"
> "$edges"

# Parse input file
while IFS= read -r line
do
    if [[ "$line" =~ ^c ]]
    then
        echo "$line" >> "$comments"
    elif [[ "$line" =~ ^p ]]
    then
        echo "$line" >> "$problem"
    elif [[ "$line" =~ ^[1-9] ]]
    then
        read -r a b <<< "$line"
        if (( a > b ))
        then
            echo "$b $a" >> "$edges"
        else
            echo "$a $b" >> "$edges"
        fi
    fi
done < "$GR_FILE"

sort -n -k1,1 -k2,2 "$edges" > "$sorted_edges"

# Write output
{
    cat "$comments"
    cat "$problem"
    cat "$sorted_edges"
} > "${GR_FILE%.gr}_sorted.gr"

echo "Written sorted output to ${GR_FILE%.gr}_sorted.gr"