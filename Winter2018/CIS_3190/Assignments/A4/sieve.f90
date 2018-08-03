module dependencies
contains

subroutine runSieve()
integer :: upper_lim
integer, dimension (0:1000000) :: int_array, prime_array
integer :: i, j, prime_counter
logical :: exist
integer(kind=8) start,finish,rate


    upper_lim = -1

    do
        write(*, '(''Please enter an int from 2-1000000 as the upper limit'')')
        read(*, '(I7)', iostat=ierror) upper_lim
        
        if(upper_lim > 2 .and. upper_lim <= 1000000) then
            if(ierror == 0) then
                exit
            else
                write(*,'(''Please enter an int from 2-1000000 as the upper limit'')')
            end if
        else    
            write(*,'(''Please enter an int from 2-1000000 as the upper limit'')')
        end if
    end do

    call system_clock(count_rate=rate)
    call system_clock(start)

    i = 0
    do while (i < upper_lim)
        int_array(i) = i
        prime_array(i) = -1
        i = i + 1
    end do

    i = 2
    do while(i*i < upper_lim)
       if(int_array(i) /= -1) then
            j = i*2
            do while(j < upper_lim)
                int_array(j) = -1
                j = j + i
            end do
        end if
        i = i + 1
    end do

    prime_counter = 0

    i = 2
    do while(i < upper_lim)
        if(int_array(i) /= -1) then
            prime_array(prime_counter) = int_array(i)
            prime_counter = prime_counter + 1
        end if
        i = i + 1
    end do

    write(*, *) prime_counter

    inquire(file='output.txt', exist=exist)
    if(exist) then
        open(1, file='output.txt', status='old', position='rewind', action='write')
    else
        open(1, file='output.txt', status='new', action='write')
    end if

    do i = 0,prime_counter-1
        write(1,*) prime_array(i)
    end do
    close(1)

    call system_clock(finish)
    write(6,*) 'Elasped time in seconds', float(finish-start)/rate

end subroutine runSieve


end module dependencies

program sieve
    use dependencies
    implicit none
    
    call runSieve()
end program sieve