module cubefile
    implicit none
    
    contains

    subroutine cubcub(cub1, cub2, operation)
        implicit none    
        character(len=255) :: cub1, cub2
        character(len=3) :: operation
        integer :: fone=11, ftwo=12, fout=13 !File specifiers
        integer :: natoms, npts(3), i, j, k
        integer, allocatable :: atomicnum(:)
        real :: startp(3), ptvec(3, 3)
        real, allocatable :: xyzpos(:,:), atomicmass(:)
        double precision, allocatable :: cube1(:,:,:), cube2(:,:,:)
        double precision, parameter :: zero = 0.0
        !Lets yank information from cub1
        open(fone, file=cub1, status='old', ERR=7000)
        read(fone, *, err=7005, end=7005) !Molecule title
        read(fone, *, err=7005, end=7005) !Field title
        read(fone, *, err=7005, end=7005) natoms, startp(:)
        do i=1, 3
            read(fone, *, err=7005, end=7005) npts(i), ptvec(:,i)
        end do
    
        !Header read, now we allocate data
        allocate(atomicnum(natoms), xyzpos(3,natoms), atomicmass(natoms), &
        cube1(npts(1), npts(2), npts(3)), cube2(npts(1), npts(2), npts(3)))
        
        !Read geometry
        do i=1, natoms
            read(fone, *, err=7005, end=7005) atomicnum(i), atomicmass(i), xyzpos(:,i)
        end do
    
        !Read cube data from file one and close it!
        do i=1, npts(1)
            do j=1, npts(2)
                read(fone, *, err=7005, end=7005) cube1(i,j,:)
            end do
        end do
        close(fone)
    
        !Lets yank information from cub2
        open(ftwo, file=cub2, status='old', ERR=7001)
        do i=1, 6
            read(ftwo, *, err=7006, end=7006) !Skip first 6 lines
        end do
    
        !Read geometry, skipping this time!
        do i=1, natoms
            read(ftwo, *, err=7006, end=7006)
        end do
        
        !Read cube data from file two and close it!
        do i=1, npts(1)
            do j=1, npts(2)
                read(ftwo, *, err=7006, end=7006) cube2(i,j,:)
            end do
        end do
        close(ftwo)
        !Now we can process our operation, we can store in cube1 vector
        select case (operation)
            case ('add')
                print *, 'Adding ', TRIM(cub1), ' and ', TRIM(cub2), ' > result.cube'
                do i=1, npts(3)
                    do j=1, npts(2)
                        cube1(i,j,:)=cube1(i,j,:)+cube2(i,j,:)   
                    end do
                end do
            case ('sub')
                print *, 'Subtracting ', TRIM(cub2), ' from ', TRIM(cub1), '> result.cube'
                do i=1, npts(3)
                    do j=1, npts(2)
                        cube1(i,j,:)=cube1(i,j,:)-cube2(i,j,:)   
                    end do
                end do
            case ('mul')
                print *, 'Multipling ', TRIM(cub1), ' and ', TRIM(cub2), ' > result.cube' 
                do i=1, npts(3)
                    do j=1, npts(2)
                        cube1(i,j,:)=cube1(i,j,:)*cube2(i,j,:)   
                    end do
                end do
            case ('div')
                print *, 'Dividing ', TRIM(cub1), ' by ', TRIM(cub2), ' > result.cube'
                do i=1, npts(3)
                    do j=1, npts(2)
                        do k=1, npts(1)
                            if(cube2(i,j,k) .eq. zero) then
                                cube1(i,j,k)=0
                            else
                                cube1(i,j,k)=cube1(i,j,k)/cube2(i,j,k)
                            end if
                        end do
                    end do
                end do
            case ('exp')
                print *, 'Taking ', TRIM(cub1), ' to the ', TRIM(cub2), "'th power > result.cube"
                do i = 1, npts(3)
                    do j = 1, npts(2)
                        cube1(i,j,:)=cube1(i,j,:)**cube2(i,j,:)   
                    end do
                end do
        end select
    
        !Lets write a new output file, result.cube
        open(fout, file='result.cube', status='unknown')
        write(fout, *) '1'
        write(fout, *) 'Output of manipulatecube'
        write(fout, *) natoms, startp(:)
        do i=1, 3
            write(fout, *) npts(i), ptvec(:,i)
        end do
    
        !Write geometry
        do i=1, natoms
            write(fout, *) atomicnum(i), atomicmass(i), xyzpos(:,i)
        end do
    
        !Write new cube data
        do i=1, npts(1)
            do j=1, npts(2)
                write(fout, *) cube1(i,j,:)
            end do
        end do
    
        !Finally deallocate
        deallocate(atomicnum, atomicmass, xyzpos, cube1, cube2)
    go to 7010
    7000 stop "Can't open cubefile1, does it exist?"
    7001 stop "Can't open cubefile2, does it exist?"
    7005 stop "Error processing cubefile1!"
    7006 stop "Error processing cubefile2, is it the same size as cubefile1?"
    7010 continue
    end subroutine

    subroutine cubscalar(cub1, scalar, operation)
        implicit none    
        character(len=255) :: cub1
        character(len=3) :: operation
        integer :: fin=11, fout=12
        integer :: natoms, npts(3), i, j, k
        integer, allocatable :: atomicnum(:)
        real :: startp(3), ptvec(3, 3), scalar
        real, allocatable :: xyzpos(:,:), atomicmass(:)
        real, parameter :: zero = 0.0
        double precision :: integration, dxyz
        double precision, allocatable ::  cube1(:,:,:)
        !Let's get data from file1
        open(fin, file=cub1, status='old', ERR=7000)
        read(fin, *, err=7005, end=7005) !Molecule title
        read(fin, *, err=7005, end=7005) !Field title
        read(fin, *, err=7005, end=7005) natoms, startp(:)
        do i=1, 3
            read(fin, *, err=7005, end=7005) npts(i), ptvec(:,i)
        end do
    
        !Header read, now we allocate data
        allocate(atomicnum(natoms), xyzpos(3,natoms), atomicmass(natoms), &
        cube1(npts(1), npts(2), npts(3)))
        
        !Read geometry
        do i=1, natoms
            read(fin, *, err=7005, end=7005) atomicnum(i), atomicmass(i), xyzpos(:,i)
        end do
    
        !Read cube data from file one and close it!
        do i=1, npts(1)
            do j=1, npts(2)
                read(fin, *, err=7005, end=7005) cube1(i,j,:)
            end do
        end do
        close(fin)
    
        !Now we can process our operation, we can store in cube1 vector
        select case (operation)
            case ('add')
                print *, 'Adding ', scalar, ' to ', TRIM(cub1), ' > result.cube'
                do i=1, npts(3)
                    do j=1, npts(2)
                        cube1(i,j,:)=cube1(i,j,:)+scalar
                    end do
                end do
            case ('sub')
                print *, 'Subtracting ', scalar, ' from ', TRIM(cub1), ' > result.cube'
                do i=1, npts(3)
                    do j=1, npts(2)
                        cube1(i,j,:)=cube1(i,j,:)-scalar
                    end do
                end do
            case ('mul')
                print *, 'Multiplying ', TRIM(cub1), ' by ', scalar, ' > result.cube'
                do i=1, npts(3)
                    do j=1, npts(2)
                        cube1(i,j,:)=cube1(i,j,:)*scalar
                    end do
                end do
            case ('div')
                print *, 'Dividing ', TRIM(cub1), ' by ', scalar, ' > result.cube'
                do i=1, npts(3)
                    do j=1, npts(2)
                        do k=1, npts(1)
                            if(scalar .eq. zero) then
                                cube1(i,j,k)=0
                            else
                                cube1(i,j,k)=cube1(i,j,k)/scalar
                            end if
                        end do
                    end do
                end do
            case ('exp')
                print *, 'Taking ', TRIM(cub1), ' to the ', scalar, "'th power > result.cube"
                do i = 1, npts(3)
                    do j = 1, npts(2)
                        cube1(i,j,:)=cube1(i,j,:)**scalar  
                    end do
                end do
            case ('abs')
                print *, 'Taking the absolute value of ', TRIM(cub1), ' > result.cube'
                do i = 1, npts(3)
                    do j = 1, npts(2)
                        cube1(i,j,:)=abs(cube1(i,j,:))
                    end do
                end do
            case ('int')
                print *, 'Integrating ', TRIM(cub1)
                dxyz=ptvec(1,1)*ptvec(2,2)*ptvec(3,3)
                integration=0
                do i = 1, npts(3)
                    do j = 1, npts(2)
                        do k = 1, npts(1)
                            integration=integration+cube1(i,j,k)
                        end do
                    end do
                end do
                integration=integration*dxyz
        end select
    
        if (operation == 'int') then
            print *, 'Integrated cube value ->', integration
        else
            !Lets write a new output file, result.cube
            open(fout, file='result.cube', status='unknown')
            write(fout, *) '1'
            write(fout, *) 'Output of manipulatecube'
            write(fout, *) natoms, startp(:)
            do i=1, 3
                write(fout, *) npts(i), ptvec(:,i)
            end do
    
            !Write geometry
            do i=1, natoms
                write(fout, *) atomicnum(i), atomicmass(i), xyzpos(:,i)
            end do
    
            !Write new cube data
            do i=1, npts(1)
                do j=1, npts(2)
                    write(fout, *) cube1(i,j,:)
                end do
            end do
        end if
    
        !Finally deallocate
        deallocate(atomicnum, atomicmass, xyzpos, cube1)
    go to 7010
    7000 stop "Can't open cubefile1, does it exist?"
    7005 stop "Error processing cubefile1!"
    7010 continue
    end subroutine
end module        
