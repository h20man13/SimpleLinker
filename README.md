# SimpleLinker
This Repository is the corresponding Linker Project in the Linkers and Loaders Textbook written by John R Levine. In his book the Linker is designed in Perl however this Linker will be desinged in C.

# Building the Linker
The Linker is controled by a MakeFile located in the base directory. No files are manually put inside the makefile however several wilcard statements are used to specify the Input, Output and a patsubst command is used to specify the intermediate object files.

To build the Linker you need to just run "make" and the project will build.

To clean up all of the object files and executables the person just needs to run the clean command or make clean.

# Adding New Source
All new source to the project needs to be added to the Src Folder. No Sub folders are allowed.

## Object Source

Each object file with the name src/<name without extension>.c gets moved to 
obj/<name without extension>.o



# Build Output

The Object Files will be placed in the obj folder as described above. The executable will be placed inside the bin folder.

# Running the Linker

To run the Linker Type in a command like the following:

Linker.exe -o <OutputFile> <Input Files>

The Linker will run and make the output file specified. Any errors will also be reported to standard output.

