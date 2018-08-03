identification division.
program-id. romannumerals.
environment division.input-output section.
file-control.
    select standard-input assign to keyboard.
    select standard-output assign to display.
    select read-file assign to wFile-folder organization is line sequential.

data division.
file section.
fd standard-input.
    01 stdin-record picture x(80).
fd standard-output.
    01 stdout-record picture x(80).
fd read-file.
    01 file-text.
        02 file-text-s picture x(1) occurs 30 times.
working-storage section.
01 wFile-folder picture x(256).
77 n picture s99 usage is computational.
77 l picture s99 usage is computational.
77 temp picture s9(8) usage is computational.
77 ret picture s9 usage is computational-3.
01 array-area.
    02 r picture x(1) occurs 30 times.
01 array-text.
    02 s picture x(1) occurs 30 times.
01 switch.
    05 eof-switch picture x value "N".
01 input-area.
    02 in-r picture x(1).
    02 filler picture x(79).
01 title-line.
    02 filler picture x(11) value spaces.
    02 filler picture x(24) value 'roman number equivalents'.
01 underline-1.
    02 filler picture x(45) value
' --------------------------------------------'.
01 col-heads.
    02 filler picture x(9) value spaces.
    02 filler picture x(12) value 'roman number'.
    02 filler picture x(13) value spaces.
    02 filler picture x(11) value 'dec. equiv.'.
    01 underline-2.
    02 filler picture x(45) value
    ' ------------------------------ -----------'.
01 print-line.

    02 filler picture x value space.
02 out-r picture x(30).
    02 filler picture x(3) value spaces.
02 out-eq picture z(9).

procedure division.
    open input standard-input, output standard-output.

    write stdout-record from title-line after advancing 0 lines.
    write stdout-record from underline-1 after advancing 1 line.
    write stdout-record from col-heads after advancing 1 line.
    write stdout-record from underline-2 after advancing 1 line.
    display " "

    perform until r(1) = 'Q'
        display "Enter F for file input, T for text input, or Q to quit"
        read standard-input into array-area
            at end exit perform
        end-read

        if (r(1) is equal to 'T')
            read standard-input into array-area
                at end exit perform
            end-read
            move 0 to l
            inspect function reverse (array-area) tallying l for leading spaces
            compute n = length of array-area - l
            call "conv" using array-area, n, ret, temp
            if (ret is equal to 1)
                move temp to out-eq
                move array-area to out-r
                write stdout-record from print-line after advancing 1 line
                display " "
            else
                display " "
            end-if
        end-if

        if(r(1) is equal to 'F')
            display "Enter File name:"
            read standard-input into array-area
                at end exit perform
            end-read

            move array-area to wFile-folder
            open input read-File
            move "N" to eof-switch
            perform until eof-switch = "Y"
                read read-file
                    at end
                        move "Y" to eof-switch
                end-read
                move 0 to l
                inspect function reverse (file-text) tallying l for leading spaces
                compute n = length of file-text - l
                call "conv" using file-text, n, ret, temp
                if (ret is equal to 1)
                    move temp to out-eq
                    move file-text to out-r
                    write stdout-record from print-line after advancing 1 line
                    display " "
                else
                    display " "
                end-if
            end-perform

            close read-File
        end-if
    end-perform

    close standard-output. close standard-input.
    stop run.
