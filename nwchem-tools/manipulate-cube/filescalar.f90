subroutine filescalar(file1, num, operation)
    implicit none    
    character(len=255) :: file1
    character(len=3) :: operation
    integer :: fin=11, fout=12
    real :: num
    !Let's get data from file1
    open(fin, file=file1, status='old', ERR=7000)
    close(fin)
    !Now we can create output
    write(*,*) 'Hello from scalar!'
    write(*,*) operation
go to 7001
7000 stop 'Error in file'    
7001 continue
end subroutine filescalar      
