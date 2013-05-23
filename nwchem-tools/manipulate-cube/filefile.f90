subroutine filefile(file1, file2, operation)
    implicit none    
    character(len=255) :: file1, file2
    character(len=3) :: operation
    integer :: fone=11, ftwo=12, fout=13
    !Lets yank information from file1
    open(fone, file=file1, status='old', ERR=7000)
    close(fone)
    !Lets yank information from file1
    open(ftwo, file=file2, status='old', ERR=7000)
    close(ftwo)
    !Now we can write output
    write(*,*) 'Hello from files!'
    write(*,*) operation
go to 7001
7000 stop 'Error in file'    
7001 continue
end subroutine filefile
