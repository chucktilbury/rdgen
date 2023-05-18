
# Global TODO Stuff
- Create global configuration system that uses the preprocessor.
- Implement string comparisons
- Assembler shell needs more options to control the locations of files.
    - Specifically the output file.
- Test using the preprocessor for function definitions.
- Test recursive functions. Direct access to vars on the stack.
- Listing should show symbol names.
- Think about when a symbol is defined and when it receives a value.
    - Check labels to be sure that they all have a value when finished parsing.
    - Make sure defined symbols have references.

# Finished
- Assembler needs to use preprocessor.
- Need to layer the value printer so that it can know about indexes and stores.
- Remove the "relative" jump instructions.
- Implement strings
    - Implement string formatting
- Implement more traps
- Figure out how to properly link the GC to a program.

# Ignored
- Command line parser to make lists based on repeat of certain items.
- Set a build parameter to enable and disable using GC (for debugging)

# Bugs
- JMP to undefined symbol is not an error.
- LOAD an address symbol that is defined after the instruction is encountered
    does not work
