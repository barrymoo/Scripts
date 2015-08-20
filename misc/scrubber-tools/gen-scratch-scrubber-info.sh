#!/usr/bin/env bash

function generate_slurm_file () {
    group=$(groups | awk '{print $1}')
    echo "#!/bin/bash" > info-$1.slurm
    echo "#SBATCH --time=00:05:00" >> info-$1.slurm
    echo "#SBATCH --nodes=1" >> info-$1.slurm
    echo "#SBATCH --ntasks-per-node=1" >> info-$1.slurm
    echo "#SBATCH --partition=$group" >> info-$1.slurm
    echo "#SBATCH --cluster=chemistry" >> info-$1.slurm
    echo "#SBATCH --job-name=info-$1" >> info-$1.slurm
    echo "#SBATCH --output=info-$1.out" >> info-$1.slurm
    echo "#SBATCH --error=/dev/null" >> info-$1.slurm
    echo "#SBATCH --mail-type=END" >> info-$1.slurm
    echo "#SBATCH --nodelist=$1" >> info-$1.slurm
    echo "find /scratch -maxdepth 1 -type f -exec ls -l {} \;" >> info-$1.slurm
}

# Generate current node list
group=$(groups | awk '{print $1}')
array=$(sinfo -M chemistry -p $group -o "%N" | tail -1)
node_lists=($(echo $array | sed 's/],/] /g'))

for i in ${node_lists[*]}; do
    prefix=($( echo $i | sed 's/\(.*\)\[\(.*\)\]/\1 \2/' ))
    for j in $(echo ${prefix[1]} | sed 's/,/ /'); do
         if [[ "$j" == *"-"* ]]; then
            array=($(echo $j | sed 's/-/ /'))
            for k in $(eval echo {${array[0]}..${array[1]}}); do
                generate_slurm_file ${prefix[0]}$k
            done
         else
            generate_slurm_file ${prefix[0]}$j
         fi
    done
done
