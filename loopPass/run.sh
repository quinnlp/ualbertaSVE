# arg1 : path to test
# arg2 : pass number to use for bisect-limit

# LOOP PASS
CC=clang CXX=clang++ cmake --build  .                                   # build the pass     
clang -c -O0 -g -emit-llvm $1 -o ${1%.c}.bc                             # compile the test into llvm binary 
opt -O3 -opt-bisect-limit=$2 ${1%.c}.bc -o ${1%.c}-temp.bc              # run the first $2 passes on the test and save the temporary binary
opt -load SVELoopPass.dylib -SVELoopPass ${1%.c}-temp.bc -o /dev/null 2> ${1%.c}.lines # run loopPass on the temporary binary
rm -f ${1%.c}-temp.bc ${1%.c}.bc                                        # remove binaries

# get the llvm IR of the test case
#clang -S -O0 -g -emit-llvm $1 -o ${1%.c}.ll

# Add this to the second opt command 
#-dot-cfg --cfg-dot-filename-prefix=${1%.c}

# Transform the dot files into png files
#for f in tests/*.dot; do dot -Tpng $f -o ${f%.dot}.png; echo $f; rm -f $f; open ${f%.dot}.png; done    

# Remove all png files in the test directory
#for f in tests/*.png; do rm -f $f; done

# PROFILING
clang -g -fprofile-instr-generate=./${1%.c}.prof -fcoverage-mapping $1 -Wall -o ${1%.c}     # compile the test with llvm profiling enabled
./${1%.c} &> /dev/null                                                                      # run the compiled file
llvm-profdata merge -output=${1%.c}.merge -instr ./${1%.c}.prof                             # merge profiling data
llvm-cov show ${1%.c} -instr-profile=${1%.c}.merge > ${1%.c}.full                           # show profiling stuff
echo " LINE|  COUNT|"
while read line 
do 
    sed -n $line'p' < ${1%.c}.full 
done < ${1%.c}.lines
rm -f ${1%.c} ${1%.c}.merge ${1%.c}.prof ${1%.c}.full ${1%.c}.lines                         # remove files     
