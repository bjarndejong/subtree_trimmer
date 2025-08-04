#!/bin/bash

ulimit -s unlimited
ulimit -v $((100 * 1024 * 1024))

GROUP=$1

for graphfile in ../../mwis_tree_decomposition/data/complete_set/"${GROUP}"*.graph
do
        filename=$(basename "$graphfile")

        filename=$(basename "$graphfile" .graph)
        safe_filename=$(echo "$filename" | sed 's/_/\\_/g')

        NUMB_OF_NODES="-"
        NUMB_OF_EDGES="-"
        WD="-"
        NUMB_OF_BAGS="-"
        MWIS="-"
        TIME="-"
        
        PLINE=$(head -n 1 "${graphfile}")
        NUMB_OF_NODES=$(echo "$PLINE" | awk '{print $1}')
        NUMB_OF_EDGES=$(echo "$PLINE" | awk '{print $2}')

        tdfile="${graphfile%graph}td"
        if [[ -s "${tdfile}" ]]
        then
                SLINE=$(grep -m 1 '^s td' "${tdfile}")
                #echo "${SLINE}"
                if [[ -n "$SLINE" ]]
                then

                        WD=$(echo "$SLINE" | awk '{print $4}')
                        NUMB_OF_BAGS=$(echo "$SLINE" | awk '{print $3}')
                        #if [ "${WD}" -lt 128 ]
			if [ "${NUMB_OF_BAGS}" -lt 2 ]
			then
				echo "Skip $(basename "${graphfile}")"
			else
                ../../subtree_trimmer/build/subtree-trimmer "${graphfile}" "${tdfile}" > "${tdfile%.td}_trimmed.td"
                SLINE_TRIMMED=$(grep -m 1 '^s td' "${tdfile%.td}_trimmed.td")
                WD_TRIMMED=$(echo "$SLINE_TRIMMED" | awk '{print $4}')
                echo "$(basename "$graphfile"): ${WD}/${WD_TRIMMED}"
			fi
                else
                        echo "${safe_filename} contains no sline"
                fi
        else
                echo "${safe_filename} is an empty file or doesn't exist"
        fi
done