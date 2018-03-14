module dependencies
contains
! Subprograms dependencies:

! will return an array of empty indexes 
! the array will contain index values as their values
! up until -1's which refers to the end of empty values
! an array returned with only -1's means a draw
function getEmptyIndexes(board)
integer, intent(in), dimension(9) :: board
integer, dimension(9) :: getEmptyIndexes
integer :: index, current_index

    current_index = 1
    do index = 1,9
        getEmptyIndexes(index) = -1
    end do

    do index = 1,9
        if(board(index) == -2) then 
            getEmptyIndexes(current_index) = index
            current_index = current_index + 1
        end if
    end do
end function getEmptyIndexes

! curr_player should be -1 for computer
! 0 for player
recursive function getBestMove(board, curr_player, final_score, winnable) result(best_move)
integer, intent(in), dimension(9) :: board
integer, intent(inout) :: final_score
integer, dimension(9) :: empty, new_board, scores
integer :: index, temp_final_score, highest_score, winner, score, next_move, next_player, best_move
integer, intent(in) :: curr_player
logical :: over
logical, intent(in) :: winnable

    index = 1
    scores = -2
    empty = getEmptyIndexes(board)
    if(empty(1) /= -1) best_move = empty(1)

    do while(empty(index) /= -1)
        new_board = board
        new_board(empty(index)) = curr_player

        call CHKOVR(new_board, over, winner)

        if(over .eqv. .true.) then
            if(winner == curr_player) then 
                score = 1
            else if(winner == -2) THEN ! draw
                score = 0 
            else
                score = -1
            end if
        else 
            if(curr_player == -1) next_player = 0
            if(curr_player == 0) next_player = -1
            next_move = getBestMove(new_board, next_player, temp_final_score, winnable)
            if (winnable) THEN
                score = score - temp_final_score
            else
                score = -1 * temp_final_score
            end if
        end if
        if(score == 1) then 
            best_move = empty(index)
            final_score = 1
            return
        end if 

        ! the scores array will keep index and score
        ! the index at which the score is stored will be the index in the
        ! board array that returns that score
        scores(empty(index)) = score 
        
        index = index + 1
    end do

    highest_score = -1
    do index = 1,9 
        if(scores(index) > highest_score) then
            highest_score = scores(index)
            best_move = index
        end if
    end do
    
    final_score = highest_score
end function getBestMove

! Game loop also game init as there is not init function
subroutine playTicTacToe()

    ! board value of -2 = empty, -1 = computer, 0 = player
    ! board indexes look like this 
    ! 1 2 3
    ! 4 5 6
    ! 7 8 9
    integer, dimension(9) :: board
    integer :: index, winner
    logical :: game_over = .false., can_move, winnable
    integer :: player_move, comp_move

    ! change this to true to dumb down the AI similar to the assignment spec
    ! the computer will choose to win if it can, otherwise will just pick a spot 
    ! to play
    winnable = .false.

    ! init for display
    do index = 1,9
        board(index) = index
    end do

    write (*,'(A)') 'TIC-TAC-TOE'
    write (*,'(A)') 'Enter 1-9 to select which square you want to play into'

    call showBoard(board)

    ! init for game
    do index = 1,9
        board(index) = -2
    end do

    ! game loop

    do while (game_over .eqv. .false.)
        write (*, '(A)') 'Enter your move:'
        player_move = getMove()

        can_move = CHKPLAY(board, player_move)
        if(can_move .eqv. .false.) then
            write(*, '(''You cant move there, try again.'')')
        else 

            board(player_move) = 0
            
            write (*, '(''After your move...'')')
            call showBoard(board)

            call CHKOVR(board, game_over, winner)
            if(game_over) then
                if(winner == 0) then
                    write(*,'(''You have won!!'')')
                    return
                else if (winner == -1) then
                    write(*,'(''You lost.. :('')')
                    return
                else
                    write(*,'(''Draw'')')
                    return
                end if
            end if

            call pickMove(board, comp_move, winnable)
            board(comp_move) = -1

            write (*, '(''After the computers move...'')')
            call showBoard(board)

            call CHKOVR(board, game_over, winner)
            if(game_over) then
                if(winner == 0) then
                    write(*,'(''You have won!!'')')
                    return
                else if (winner == -1) then
                    write(*,'(''You lost.. :('')')
                    return
                else
                    write(*,'(''Draw'')')
                    return
                end if
            end if            
        end if
    end do

end subroutine playTicTacToe

! returns 1 if the values are the same and player or computer
! returns 0 if the value are not the same
function same(first, second, third)
    integer, intent(in) :: first, second, third
    logical :: same

    if(first == second .and. second == third) then
        if(first == -1 .or. first == 0) then
            same = .true.
        else 
            same = .false.
        end if
    else
        same = .false.
    end if
end function same

! receives player input, an int between 1-9
! if input is invalid returns -1
! else returns input
function getMove()
    integer :: player_move = 0, ierror, getMove

    do
        read (*,'(I1)', iostat=ierror) player_move

        if(ierror == 0) THEN
            exit
        else
            write(*,'(''Please enter 1-9'')')
        end if
    end do
   
    if (player_move <= 0 .and. player_move >= 10) then
        write(*,'(''Incorrect input, please enter an integer between 1-9'')')
        getMove = -1
    else
        getMove = player_move
    end if
end function getMove

subroutine pickMove(board, best_move, winnable)
integer, intent(in), dimension(9) :: board
integer :: final_score
integer, intent(out) :: best_move
logical, intent(in) :: winnable 

    ! board = board
    ! -1 for computer move
    ! final_score used internaly for recursion
    ! returns the index of the best move
    best_move = getBestMove(board, -1, final_score, winnable)    
end subroutine pickMove 

subroutine showBoard(board)

    integer, intent(in) :: board(9)
    write(*,*)
    write(*,'(5X, ''|'', 5X, ''|'')')

    ! board value of -2 = empty, -1 = computer, 0 = player, anything else just print that 
    ! value to the board (if < 10 so that it doesnt mess up formating)
    do index = 1, 9
        if (board(index) == -2) then 
            write(*, '(5X)', advance='no')
        else if (board(index) == -1) then
            write(*, '(2X, ''O'', 2X)', advance='no')
        else if (board(index) == 0) then
            write(*, '(2X, ''X'', 2X)', advance='no')
        else if (board(index) < 10) then
            write(*, '(2X, I1, 2X)', advance='no') board(index)
        else
            write(*, '(5X)', advance='no')
        end if

        if (mod(index,3) == 0) then ! newline breaks at the right side of the board
            write(*,*)
            ! formating for the rows if this isnt the last row
            if(index /= 9) then
                write(*, '(''_____+_____+_____'')')
            end if
            write(*,'(5X, ''|'', 5X, ''|'')')
        else ! not at the end of the line so add in the seperators
            write(*, '(''|'')', advance='no') 
        end if
    end do

    write(*,*)   

end subroutine showBoard

! OLD CODE _________________________________

! outputs to winner
! outputs -1 for computer win
! outputs 0 for player win
! outputs -2 for draw
SUBROUTINE CHKOVR(TICTAC, OVER, WINNER)

    integer, intent(inout), dimension(9) :: TICTAC
    integer, intent(out) :: WINNER
    LOGICAL, intent(inout) :: OVER

    LOGICAL :: DSAME
    INTEGER :: IR, IC

    OVER = .TRUE.
   
    DO IR=1, 7, 3
        IF (SAME(TICTAC(IR), TICTAC(IR+1), TICTAC(IR+2))) THEN
            WINNER = TICTAC(IR)
            RETURN
        ENDIF
    end Do

    DO IC=1, 3
        IF (SAME(TICTAC(IC), TICTAC(IC+3), TICTAC(IC+6))) THEN
            WINNER = TICTAC(IC)
            RETURN
        ENDIF
    end do


    DSAME = SAME(TICTAC(1), TICTAC(5), TICTAC(9))
    DSAME = DSAME .OR. SAME(TICTAC(3), TICTAC(5), TICTAC(7))

    IF (DSAME) THEN
        WINNER = TICTAC(5)
        RETURN
    END IF

    DO IR = 1, 9
        IF (TICTAC(IR) == -2) THEN
            OVER = .FALSE.
            RETURN
        END IF
    end do

    WINNER = -2

    RETURN
END subroutine CHKOVR

FUNCTION CHKPLAY(TICTAC, MOVE)
integer, intent(in), dimension(9) :: TICTAC
integer, intent(in) :: MOVE
logical :: CHKPLAY
INTEGER :: index

    CHKPLAY = .false.

    if(TICTAC(MOVE) == -1 .or. TICTAC(MOVE) == 0) return

    do index = 1,9
        if(TICTAC(index) == -2) then
            CHKPLAY = .true.
            return
        end if
    end do
end function

end module dependencies

program tictactoe
    use dependencies
    implicit none

    call playTicTacToe()

end program tictactoe