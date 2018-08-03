identification division.
program-id. conv.
environment division.
input-output section.

file-control.
    select standard-output assign to display.

data division.
file section.
fd standard-output.
    01 stdout-record picture x(80).

working-storage section.
77 i picture s99 usage is computational.
77 prev picture s9(8) usage is computational.
77 d picture s9(4) usage is computational.
77 found_value picture s9 usage is computational-3.
01 error-mess.
    02 filler picture x(22) value ' illegal roman numeral'.

linkage section.
77 m picture s99 usage is computational.
77 err picture s9 usage is computational-3.
77 sum1 picture s9(8) usage is computational.
01 array-area.
    02 s picture x(1) occurs 30 times.

procedure division using array-area, m, err, sum1.
    move zero to sum1. move 1001 to prev.
    perform loop thru end-loop varying i from 1 by 1
      until i is greater than m.
    move 1 to err. goback.
loop.
    move zero to found_value.
    if (s(i) is equal to 'i') or (s(i) is equal to 'I')
        move 1 to d move 1 to found_value.
    if (s(i) is equal to 'v') or (s(i) is equal to 'V')
        move 5 to d move 1 to found_value.
    if (s(i) is equal to 'x') or (s(i) is equal to 'X')
        move 10 to d move 1 to found_value.
    if (s(i) is equal to 'l') or (s(i) is equal to 'L')
        move 50 to d move 1 to found_value.
    if (s(i) is equal to 'c') or (s(i) is equal to 'C')
        move 100 to d move 1 to found_value.
    if (s(i) is equal to 'd') or (s(i) is equal to 'D')
        move 500 to d move 1 to found_value.
    if (s(i) is equal to 'm') or (s(i) is equal to 'M')
        move 1000 to d move 1 to found_value.

    if (found_value is equal to 1)
        add d to sum1.

    if (found_value is equal to 0)
        open output standard-output
        write stdout-record from error-mess after advancing 1 line move 2 to err
        close standard-output goback.

    if (d is greater than prev) and (found_value)
       compute sum1 = sum1 - 2 * prev.
end-loop. move d to prev.
