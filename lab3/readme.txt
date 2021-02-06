
# How to run our Programs ? 

There are three files for each algorithm, namely
1 ) tabuSearch.cc
2 ) beamSearch.cc
3 ) vnd.cc

for generating CNF : clause_generator.cc

run run.sh file using commandline (it will compile all *.cc files)
command : $ ./run.sh

# How to generate CNF ? 
for generating a CNF with 'L' literals and 'C' clauses, 
run following command in commandline in this format:

command : $ ./clause_generator L C > CNF.txt

in our case, L = 4 and C = 5 so, 
command : $ ./clause_generator 4 5 > CNF.txt

you can see generated clauses in readable_CNF.txt (one clause on each line)

# to run tabuSearch
command : $./tabuSearch < CNF.txt

# to run beamSearch 
command : $./beamSearch < CNF.txt

# to run VND
command : $./vnd < CNF.txt

