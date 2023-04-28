The project was developed according to a test task for Basealt.



The program performs the following actions:

- Downloads packages from two branches (the names of the branches are specified by the user);

- Finds all unique packages from the first branch (criterion 1);

- Finds all unique packages from the second branch (criterion 2);

- Finds all packages from the first branch, the version of which is greater than the packages of the second branch (criterion 3).

- Data is saved in JSON format in the 'output.json'  file.



Output.json file structure:

The 'unique_branch1_length'  field is the number of packages found by the first search criterion.

The 'unique_branch1'  field is an array of packages according to the first criterion.

The 'unique_branch2_length'  field is the number of packages found by the second search criterion.

The 'unique_branch2 field'  is an array of packets according to the second criterion.

The 'max_versions_length'  field is the number of packages found by the third search criterion.

The 'max_versions' field is an array of packages according to the third criterion.



Installing the program.

The curl library is required for the program to work.

You also need to place a file 'lib_data_getter.so' to the directory with the executable file or to the 'usr/lib'  directory.



How to use.

The Data_getter class uses the get_raw_data method to get the page at the address, the method requires a string with the full address of the page.



The finder structure allows you to find the desired substrings in a string.

The reset method sets the initial state of the structure, the method requires a string to search.

The text_between method finds a substring between the begin and begin strings in the set string. This method returns the found string. The found string is also saved in the buffer. In case of failure, an empty string is returned and the last position found is reset to zero.

The get_last_pos method returns the position of the last substring found.

The get_buffer method returns a buffer.

The get_raw_data method returns the string in which the search is being performed.



The package structure allows you to store information about packages.

The init method initializes a structure from a given, raw string. Requires a raw string as an argument.

The get_json method returns a string formatted as JSON with data from this structure.

The sum method adds all the primary fields of the structure into a string and returns it. It is needed for a quick comparison of structures.
