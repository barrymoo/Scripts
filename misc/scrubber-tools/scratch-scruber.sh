#!/usr/bin/env bash

function generate_scrubber () {
    group=$(groups | awk '{print $1}')
    echo "#!/bin/bash" > scrub-$1.slurm
    echo "#SBATCH --time=00:05:00" >> scrub-$1.slurm
    echo "#SBATCH --nodes=1" >> scrub-$1.slurm
    echo "#SBATCH --ntasks-per-node=1" >> scrub-$1.slurm
    echo "#SBATCH --partition=$group" >> scrub-$1.slurm
    echo "#SBATCH --cluster=chemistry" >> scrub-$1.slurm
    echo "#SBATCH --job-name=scrub-$1" >> scrub-$1.slurm
    echo "#SBATCH --output=/dev/null" >> scrub-$1.slurm
    echo "#SBATCH --error=/dev/null" >> scrub-$1.slurm
    echo "#SBATCH --mail-type=END" >> scrub-$1.slurm
    echo "#SBATCH --nodelist=$1" >> scrub-$1.slurm
    echo "find /scratch -maxdepth 1 -type f -user $USER -exec rm -rf {} \;" >> scrub-$1.slurm
}


user=$USER
group=$(groups | awk '{print $1}')

# Make sure we have a nodelist
if [ $# -lt 1 ]; then
    echo "Usage: scratch-scrubber.sh <node> (<node>...)"
fi 

# Generate current node list
array=$(sinfo -M chemistry -p $group -o "%N" | tail -1)
node_lists=($(echo $array | sed 's/],/] /g'))
current_node_list=()

for i in ${node_lists[*]}; do
    prefix=($( echo $i | sed 's/\(.*\)\[\(.*\)\]/\1 \2/' ))
    for j in $(echo ${prefix[1]} | sed 's/,/ /'); do
         if [[ "$j" == *"-"* ]]; then
            array=($(echo $j | sed 's/-/ /'))
            for k in $(eval echo {${array[0]}..${array[1]}}); do
                current_node_list+=(${prefix[0]}$k)
            done
         else
            current_node_list+=(${prefix[0]}$k)
         fi
    done
done

# Generate scrubbers
for i in $*; do
    if [[ "${current_node_list[@]}" =~ " $i " ]]; then
        generate_scrubber $i
    fi    
done
