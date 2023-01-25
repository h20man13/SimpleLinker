# Syntax Overview

This File is to describe the Expected Sytax of the File...

The First Line of an object file in this Linker Language is the Magic Number
The Magic Number of the File is the Literal Word LINK

An example File is shown here

LINK
NumSegs NSyms NRels
- Segments -
-  Synbols -
-  Rels -
-  Data -

As you can see by the Data above the Grammar ca not be context free.
We are Given the Number of Segments the Number of Tables and the Number of Relocation Entries
With that being said we should be able to have Headers with any number of spaces inside the Minuses and We should also assume there should be a NewLine after every header for sytax reasons



