program pasrotate
    implicit none
    character(255) :: inputGeomFile, inputTensorFile
    character(2), allocatable :: atomList(:)
    character(1) :: JOBZ='V', UPLO='U'
    double precision, allocatable :: coordList(:,:), rotatedCoords(:,:)
    double precision :: negUnitMat(3,3)=0
    double precision :: efgTensor(3,3), efgEigVals(3), WORK(8)
    integer :: LWORK=8, INFO, narg, inpXYZ=55, inpEFG=56, nAtoms, i
    integer :: inverse=0
    !Check for user input
    narg = iargc()
    
    if (narg .ne. 2) then
        stop 'Usage: ./pas-rotate inputXYZFile EFGTensor'
    end if

    !Get args
    call getarg(1,inputGeomFile)
    call getarg(2,inputTensorFile)

    if (inputTensorFile .eq. 'inv' .or. inputTensorFile .eq. 'inverse') then
        do i=1,3
            negUnitMat(i,i)=-1.00
        end do
        inverse=1
    end if

    open(inpXYZ, status='old', form='formatted', file=inputGeomFile, err=9001)
    if (inverse .eq. 0) then
        open(inpEFG, status='old', form='formatted', file=inputTensorFile, err=9002)
    end if

    !Read XYZ File
    read(inpXYZ,*) nAtoms
    read(inpXYZ,*) !Blank Line
    !Allocate some arrays and read in rest of data
    allocate(atomList(nAtoms))
    allocate(coordList(nAtoms,3))
    allocate(rotatedCoords(nAtoms,3))
    do i=1,nAtoms
        read(inpXYZ,*) atomList(i), coordList(i,:)        
    end do
    !If no inverse switch read efgTensor and diagnolize
    if (inverse .eq. 1) then
       rotatedCoords=MATMUL(coordList, negUnitMat)
    else
        do i=1,3
            read(inpEFG,*) efgTensor(i,:)
        end do
        close(inpXYZ)
        close(inpEFG)
        !Now I need to diagonolize the efgTensor with LAPACK call
        call dsyev(JOBZ, UPLO, 3, efgTensor, 3, efgEigVals, WORK, LWORK, INFO)
        !efgTensor now contains eigenvectors for PAS rotation :)
        !Rotate the coordList with the efgTensor and print the new coords :)
        rotatedCoords = MATMUL(coordList, efgTensor)
    end if
    !Print Out 
    write(*,*) nAtoms
    write(*,*) ''
    do i=1,nAtoms
        write(*,*) atomList(i), rotatedCoords(i,:)
    end do
    deallocate(atomList, coordList, rotatedCoords)
    stop
!Error Codes
9001 stop 'Cannot open inputXYZFile, does it exist?'
9002 stop 'Cannot open EFGTensor, does it exist?'
end program pasrotate      
