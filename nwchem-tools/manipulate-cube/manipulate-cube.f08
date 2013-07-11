program manipulatecube
    use string
    use cubefile
    implicit none

    character(len=255) :: arg1, arg2, arg3, cubefile1 = '', cubefile2 = '', operation
    character(len=3) :: op
    real :: scalar
    real, parameter :: zero = 0.0
    integer :: narg, foundop=0

    narg = command_argument_count()
    if ( narg .lt. 2 .or. narg .gt. 3) then
        print *, 'See README for proper usage and examples'
        stop 'Incorrect Usage Error'
    
    !Deal with abs and int cases
    else if ( narg .eq. 2 ) then
        call get_command_argument(1, arg1)
        call get_command_argument(2, arg2)
        if ( INDEX(TRIM(arg1),'-',.false.) .eq. 1 ) then
            foundop = 1
            operation = arg1
        else
            cubefile1 = arg1            
        end if
        if ( INDEX(TRIM(arg2),'-',.false.) .eq. 1 .and. foundop .eq. 1) then
            stop 'Two operations specified? See README'
        else if ( INDEX(TRIM(arg2),'-',.false.) .eq. 1 .and. foundop .ne. 1 ) then
            foundop = 2
            operation = arg2
        else 
            cubefile1 = arg2    
        end if
        if ( foundop .eq. 0 ) then
            stop 'No operation specified? See README'
        end if

        !Now we can see which operation we are dealing with...
        call strip(operation, '-')
        op = TRIM(operation)
        if ( op .eq. 'int' .or. op .eq. 'mod' .or. op .eq. 'abs') then
            call cubscalar(cubefile1, zero, op)
        else if ( LEN(TRIM(op)) .ne. 3 ) then
            stop 'This is not a valid operator, see README' 
        else
            stop 'You can only use integrate or modulus operators on one cubefile, see README!' 
        end if

    !Deal with add, sub, mul, div, exponentiate cases
    !need to protect against dummies with int and modulus cases
    else if ( narg .eq. 3 ) then
        call get_command_argument(1, arg1)
        call get_command_argument(2, arg2)
        call get_command_argument(3, arg3)
        if ( INDEX(TRIM(arg1),'-',.false.) .eq. 1 ) then
            foundop = 1
            operation = arg1
        else
            cubefile1 = arg1
        end if
        if ( INDEX(TRIM(arg2),'-',.false.) .eq. 1 .and. foundop .eq. 1) then
            stop 'Two operations specified? See README'
        else if ( INDEX(TRIM(arg2),'-',.false.) .eq. 1 .and. foundop .ne. 1) then
            foundop = 2
            operation = arg2
        else
            if ( cubefile1 .eq. '' ) then
                cubefile1 = arg2
            else
                cubefile2 = arg2
            end if
        end if
        if ( INDEX(TRIM(arg3),'-',.false.) .eq. 1 .and. (foundop .eq. 1 .or. foundop .eq. 2) ) then 
            print *, INDEX(TRIM(arg3),'-',.false.)
            stop 'At least two operations specified? See README'
        else if ( INDEX(TRIM(arg3),'-',.false.) .eq. 1 .and. foundop .ne. 1 .and. foundop .ne. 2) then
            foundop = 3
            operation = arg3
        else
            if ( cubefile1 .eq. '' ) then
                cubefile1 = arg3
            else
                cubefile2 = arg3
            end if
        end if

        !Now we can see which operation we are dealing with...
        call strip(operation, '-')
        op = TRIM(operation)
        if ( LEN(TRIM(op)) .ne. 3 ) then
            stop 'This is not a valid operator, see README' 
        else if ( op .eq. 'int' .or. op .eq. 'mod' .or. op .eq. 'abs') then
            call cubscalar(cubefile1, zero, op)
        else
            read (cubefile2, *, err=7000) scalar
            call cubscalar(cubefile1, scalar, op)
            go to 7001
7000        call cubcub(cubefile1, cubefile2, op)
7001        continue
        end if      
    end if        
end program      
