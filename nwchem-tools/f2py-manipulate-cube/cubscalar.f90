subroutine cubscalar(cub1, scalar, operation)
    implicit none    
    character(len=255) :: cub1
    character(len=3) :: operation
    integer :: fin=11, fout=12
    integer :: natoms, npts(3), i, j, k
    integer, allocatable :: atomicnum(:)
    real :: startp(3), ptvec(3, 3), scalar
    real, allocatable :: xyzpos(:,:), atomicmass(:)
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
            print *, 'Adding scalar to cubefile!'
            do i=1, npts(3)
                do j=1, npts(2)
                    cube1(i,j,:)=cube1(i,j,:)+scalar
                end do
            end do
        case ('sub')
            print *, 'Subtracting scalar from cubefile!'
            do i=1, npts(3)
                do j=1, npts(2)
                    cube1(i,j,:)=cube1(i,j,:)-scalar
                end do
            end do
        case ('mul')
            print *, 'Multiply cube files!'
            do i=1, npts(3)
                do j=1, npts(2)
                    cube1(i,j,:)=cube1(i,j,:)*scalar
                end do
            end do
        case ('div')
            print *, 'Divide cube files!'
            do i=1, npts(3)
                do j=1, npts(2)
                    do k=1, npts(1)
                        if(scalar .eq. 0.) then
                            cube1(i,j,k)=0
                        else
                            cube1(i,j,k)=cube1(i,j,k)/scalar
                        end if
                    end do
                end do
            end do
        case ('exp')
            print *, 'cube1 ^ cube2!'
            do i = 1, npts(3)
                do j = 1, npts(2)
                    cube1(i,j,:)=cube1(i,j,:)**scalar  
                end do
            end do
        case ('abs')
            print *, 'Absolute value of cube1!'
            do i = 1, npts(3)
                do j = 1, npts(2)
                    cube1(i,j,:)=abs(cube1(i,j,:))
                end do
            end do
        case ('int')
            print *, 'Integrate cube1!'
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
        print *, 'Integrated cube:', integration
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
