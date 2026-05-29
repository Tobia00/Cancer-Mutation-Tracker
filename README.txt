Levenshtein-Based Cancer Mutation Tracker: Detecting and Annotating Mutations in DNA Sequences

You can find in here a simple brief on how to interact with the mentioned Tool.



Features
- Efficient DNA sequence comparison using the Levenshtein Distance algorithm.
- Identification and classification of mutation types: substitutions, insertions, deletions.
- Mutation position localization within sequences.
- Annotation of mutations by linking to the COSMIC mutation database.
- Output of mutation details and annotation results to text and CSV files.



Requirements
- fstream library
- iostream library
- sstream library
- string library
- Mutation database CSV file formatted with columns: Gene, Accession Number, URL, CDS mutation



##Note
#The tool is optimized for working with genome up to 50000 longer than that it might crash depending on your computational resources




1. For the levenshtien algorithm 
The data is processed through the function forming two matrices one matrix is the distance matrix that will report the number of mutations the through out the data and another one which is the path matrix that will be annotated based on the action that happened whether the matrix is going diagonally, up, or left. At the end result in a file that contain all the mutations detected formatted to be used in the HashMap and annotation using the COSMIC data

2. HashMap
The output from the alignment is processed and searched for through out the whole data set of mutation to build an array that contain all the genes that have this mutation linked to accession number of mutation, name of gene, and URL. The mutations then go through the hashing function to build the HashMap and determine the indexing for each mutation and then connect them to the array corresponding to their mutation in the data set


Input files:
- Input the filename of the normal genome FASTA file.
- Input the filename of the mutant genome FASTA file.
- Database containing all the known mutations.


Output files:
- mutations_output.txt containing the list of detected mutations and Levenshtein distance.
- mutation_hashmap_structure.csv containing mutations linked with their gene name, accession number, and URL from the mutation database.


Contact Info
s-ahmed.eid@zewailcity.edu.eg