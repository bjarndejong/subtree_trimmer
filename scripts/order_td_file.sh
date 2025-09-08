#!/bin/bash

TD_FILE=$1

# Temp files
tmp_dir=$(mktemp -d)
trap 'rm -rf "$tmp_dir"' EXIT
comments="$tmp_dir/comments"
solution="$tmp_dir/solution"
edges="$tmp_dir/edges"
sorted_bags="$tmp_dir/sorted_bags"

# Initialize temp files
> "$comments"
> "$solution"
> "$edges"
> "$sorted_bags"

# Parse input file
while IFS= read -r line
do
    if [[ "$line" =~ ^c ]]
    then
        echo "$line" >> "$comments"
    elif [[ "$line" =~ ^s ]]
    then
        echo "$line" >> "$solution"
    elif [[ "$line" =~ ^b ]]
    then
        read -r -a fields <<< "$line"
        bag_id=${fields[1]}
        bag_array=("${fields[@]:2}")
        sorted_contents=$(printf "%s\n" "${bag_array[@]}" | sort -n | paste -sd ' ' -)
        echo "b $bag_id $sorted_contents" >> "$sorted_bags"
    elif [[ "$line" =~ ^[1-9] ]]
    then
        echo "$line" >> "$edges"
    fi
done < "$TD_FILE"


# Write output
{
    cat "$comments"
    cat "$solution"
    cat "$sorted_bags"
    cat "$edges"
} > "${TD_FILE%.td}_sorted.td"

echo "Written sorted output to ${TD_FILE%.td}_sorted.td"