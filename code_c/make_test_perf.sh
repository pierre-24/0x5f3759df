opts=$1
stripped="$(echo $opts | tr -d ' ')"
out_prog=test"$stripped"
out_csv=test"$stripped".csv

# remove result
rm -R $out_csv

# compile
gcc -lm -o "$out_prog" perf.c $opts

# execute
for i in `seq 0 1000`; do 
    echo $(./"$out_prog") >> $out_csv
done
