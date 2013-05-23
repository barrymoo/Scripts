      program nlmovec
      Implicit NONE

      character(LEN=1008) :: movecs, nlmodat, output
      character(LEN=11) :: spincheck, checkaspin=" ALPHA SPIN", &
     & checkbspin=" BETA  SPIN"
      character(LEN=255) :: title       ! Returns title of job that created vectors
      character(LEN=255) :: basis_name  ! Returns name of basis set
      character(LEN=255) :: line        !tmp string to hold current line
      integer*8 :: narg, i, j, posit  !number of arguments should be 2 with dummy counters
      integer,parameter :: zero = 0
      integer*8 :: nbf               ! Returns no. of functions in basis
      integer*8 :: nsets, jset            ! Returns no. of functions in each set
      integer*8, parameter :: ldnmo=10             ! Inputs size of nmo
      integer*8 nmo(ldnmo)        ! Returns no. of vectors in each set

      integer*8, parameter ::  binlu=21, nlmolu=22, outlu=23      ! Unit no. for reading ! These need to be managed !!!
      integer*8 :: lentit
      integer*8 :: lenbas
      character(32) :: geomsum, basissum
      character(26) :: date
      character(20) :: scftype20
      double precision, allocatable :: dvec(:)
      double precision :: energy, enrep

      !Version 1 will just input movecs and output the same movecs to some user defined output file
      narg = iargc()
      if (narg .lt. 3 .or. narg .gt. 3) then
         stop 'Usage: nlmovec nwchem.movecs data.nlmo outfile.movecs'
      end if

      !gets arguments for user
      call getarg(1,movecs)
      call getarg(2,nlmodat)
      call getarg(3,output)

      !binlu is binary file read type
      open(binlu, status='old', form='unformatted', file=movecs)
      open(nlmolu,status='unknown',form='formatted', file=nlmodat)
      open(outlu, status='unknown', form='unformatted', file=output)

      read(binlu) basissum, geomsum, scftype20, date 
      write(outlu) basissum, geomsum, scftype20, date

      read(binlu) scftype20
      write(outlu) scftype20

      read(binlu) lentit
      write(outlu) lentit

      title = ' '
      read(binlu) title(1:lentit)
      write(outlu) title(1:lentit)      

      read(binlu) lenbas
      write(outlu) lenbas

      basis_name=' '
      read(binlu) basis_name(1:lenbas)
      write(outlu) basis_name(1:lenbas)

      read(binlu) nsets
      write(outlu) nsets 
        
      read(binlu) nbf
      write(outlu) nbf

      read(binlu) (nmo(i),i=1,nsets)
      write(outlu) (nmo(i),i=1,nsets)

      !dummy vector to hold temp data
      allocate ( dvec(nbf) )
     
      do i = 0, 2
        read(nlmolu,'(A)') line
      end do
      
      if ( nsets .eq. 2 ) then
        read(nlmolu,'(A)') spincheck
        if( spincheck .ne. checkaspin ) then
         write(*,'(A)') "NLMOVEC Failure: Unrestricted Movecs File", &
     & " with Restricted NLMO/NBO File"
        end if
      end if

      do jset = 1, nsets

      read(binlu) (dvec(j),j=1,nbf) ! Occupation numbers 
      write(outlu) (dvec(j),j=1,nbf) ! Occupation numbers
      
      read(binlu) (dvec(j),j=1,nbf) ! Eigenvalues
      write(outlu) (dvec(j),j=1,nbf) ! Eigenvalues
      
      !if jset is two we need to skip to beta spin section and continue reading
      if ( jset .eq. 2 ) then
 
   40  read(nlmolu,'(A)',end=50) line
       posit = index (line, checkbspin)
        if ( posit .ne. zero ) then
         goto 50
        endif
       goto 40
   50 continue
     
      end if

      !imbedded if deals with first line having an extra space 
      do i = 1, nmo(jset)
        read(nlmolu, *) (dvec(j),j=1,nbf)
        write(outlu) (dvec(j),j=1,nbf)
      end do

     !Enter New Eigen Vectors here
      do i = 1, nmo(jset)
        read(binlu) (dvec(j),j=1,nbf) ! Do nothing with this, skipping this section of the movecs file     
       end do
      end do

     read(binlu,err=901,end=901) energy, enrep
     write(outlu) energy, enrep
      goto 902
 901 write(outlu) 0.0,0.0
 902 continue

     deallocate(dvec)      

     close(binlu)
     close(outlu)
     close(nlmolu)

      end
