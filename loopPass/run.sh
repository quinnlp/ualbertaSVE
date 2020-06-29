# arg1 : path to test
# arg2 : pass number to use for bisect-limit


CC=clang CXX=clang++ cmake --build  .                                   # build the pass     
clang -c -O0 -g -emit-llvm $1 -o ${1%.c}.bc                             # compile the test into llvm binary 
opt -O3 -opt-bisect-limit=$2 ${1%.c}.bc -o ${1%.c}-temp.bc              # run the first $2 passes on the test and save the temporary binary
opt -load SVELoopPass.dylib -SVELoopPass ${1%.c}-temp.bc -o /dev/null   # run loopPass on the temporary binary
rm -f ${1%.c}-temp.bc                                                   # remove the temporary binary
rm -f ${1%.c}.bc                                                        # remove of the llvm binary

# get the llvm IR of the test case
#clang -S -O0 -g -emit-llvm $1 -o ${1%.c}.ll

# Add this to the second opt command 
#-dot-cfg --cfg-dot-filename-prefix=${1%.c}

# Transform the dot files into png files
#for f in tests/*.dot; do dot -Tpng $f -o ${f%.dot}.png; echo $f; rm -f $f; open ${f%.dot}.png; done    

# Remove all png files in the test directory
#for f in tests/*.png; do rm -f $f; done