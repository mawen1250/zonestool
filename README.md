zonestool
=========

Generate x264 --zones parameter for OP/ED according to chapter and qpfile



Function:

    Accept a chapter and a QP file as input files, then convert the frame numbers of OP and ED to --zones parameter for x264, and output it to a text file.
    Chapter name should be OP/Opening or ED/Ending. (not case sensitive)

Usage:

    zonestool [options] <output>

General options:

    -h, --help                List help information.
Input/Output options:

    -c, --chapter <string>    Specify path for chapter file. (Essential option)
    -q, --qpfile <string>     Specify path for QP file. (Essential option)
    -o, --output <string>     Specify path for output file. (Essential option, -o/--output can be omitted)
    -a, --append <bool>       If the output file exists, whether to append the file or overwrite it. [true]
Optional options:

    -O, --op <string>         Options to be used in zones for OP
    -E, --ed <string>         Options to be used in zones for ED
