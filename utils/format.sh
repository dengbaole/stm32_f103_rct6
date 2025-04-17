find . | grep '.*\.[ch]$' | xargs clang-format --verbose -style=file -i 
# End
