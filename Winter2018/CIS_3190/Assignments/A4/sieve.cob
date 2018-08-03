identification division.
program-id. sieve.
environment division. input-output section.
file-control.
    select standard-input assign to keyboard.
    select standard-output assign to display.

data division.
file section.
fd standard-input.
    01 stdin-record picture x(80).
fd standard-output.
    01 stdout-record picture x(80).

working-storage section.
    01 int-array.
       02 int-element pic s99 usage is computational occurs 1000000 times. 
    01 prime-array.
       02 prime-element pic s99 usage is computational occurs 1000000 times.
    77 i pic s99 usage is computational.
    77 j pic s99 usage is computational.
    77 done pic s99 usage is computational.

procedure division.
    open input standard-input, output standard-output.

    move 0 to done.
    perform until done = 1
       
    end-perform
       
