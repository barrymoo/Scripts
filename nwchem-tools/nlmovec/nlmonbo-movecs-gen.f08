      program nlmonbomovecsgen
      implicit none

      ! File names from command line
      character(LEN=255) :: movecs, nlmonbo, output

      ! Spin variables used to check input
      character(LEN=11) :: spincheck, checkaspin=" ALPHA SPIN", checkbspin=" BETA  SPIN"

      ! Character variables needed for reading movecs file
      character(LEN=255) :: title, basis_name, line       
      
      ! Some 8 bit integer variables 
      integer*8 :: narg, i, j, posit, nbf, nsets, jset, lentit, lenbas
      integer, parameter :: zero = 0

      ! Not 100% sure what this is doing
      integer*8, parameter :: ldnmo=10
      integer*8 :: nmo(ldnmo)

      ! Unit numbers for files
      integer*8, parameter :: inf=21, nlmonbof=22, outf=23

      ! Various character variables for reading movecs
      character(32) :: geomsum, basissum
      character(26) :: date
      character(20) :: scftype20

      ! Some double precision variables to store
      double precision, allocatable :: dvec(:)
      double precision :: energy, enrep

      ! Use iargc() function to check command line arguments
      narg = iargc()
      if (narg .lt. 3 .or. narg .gt. 3) then
         stop 'Usage: nlmonbo-movecs-gen input.movecs nbodata.47(NLMONBO) outfile.movecs'
      end if

      ! Use getarg function to store file names 
      call getarg(1, movecs)
      call getarg(2, nlmonbo)
      call getarg(3, output)

      ! inf is binary file read type
      open(inf, status='old', form='unformatted', file=movecs)
      open(nlmonbof, status='unknown', form='formatted', file=nlmonbo)
      open(outf, status='unknown', form='unformatted', file=output)

      ! Reading and writing header information from in movecs to out movecs 
      read(inf) basissum, geomsum, scftype20, date 
      write(outf) basissum, geomsum, scftype20, date

      read(inf) scftype20
      write(outf) scftype20

      read(inf) lentit
      write(outf) lentit

      title = ' '
      read(inf) title(1:lentit)
      write(outf) title(1:lentit)      

      read(inf) lenbas
      write(outf) lenbas

      basis_name=' '
      read(inf) basis_name(1:lenbas)
      write(outf) basis_name(1:lenbas)

      read(inf) nsets
      write(outf) nsets 
        
      read(inf) nbf
      write(outf) nbf

      read(inf) (nmo(i),i=1,nsets)
      write(outf) (nmo(i),i=1,nsets)

      ! dvec used to hold nbf by nbf matrix data
      allocate ( dvec(nbf) )
     
      ! first three lines of nbodata.47 are useless
      do i = 0, 2
        read(nlmonbof,'(A)') line
      end do
      
      ! check the spin for NLMONBO
      if ( nsets .eq. 2 ) then
        read(nlmonbof,'(A)') spincheck
        if( spincheck .ne. checkaspin ) then
          write(*,'(A)') "NLMONBO Failure: Unrestricted movecs input file", &
    &     " with restricted NLMONBO data file"
        end if
      end if

      do jset = 1, nsets

        read(inf) (dvec(j),j=1,nbf) ! Occupation numbers 
        write(outf) (dvec(j),j=1,nbf) ! Occupation numbers
      
        read(inf) (dvec(j),j=1,nbf) ! Eigenvalues
        write(outf) (dvec(j),j=1,nbf) ! Eigenvalues
      
        !if jset is two we need to skip to beta spin section and continue reading
        if ( jset .eq. 2 ) then
   40     read(nlmonbof,'(A)',end=50) line
          posit = index (line, checkbspin)
          if ( posit .ne. zero ) then
            goto 50
          endif
          goto 40
   50     continue
        end if

        ! Read nlmomo eigen vectors and write them to the output file
        do i = 1, nmo(jset)
          read(nlmonbof, *) (dvec(j),j=1,nbf)
          write(outf) (dvec(j),j=1,nbf)
        end do

        ! Read the eigenvectors from the movecs file and print them to the outfile
        ! This should become a debug feature at some point
        do i = 1, nmo(jset)
          read(inf) (dvec(j),j=1,nbf)
          !write(outf) (dvec(j),j=1,nbf)
        end do
      end do ! end jset

      read(inf,err=901,end=901) energy, enrep
      write(outf) energy, enrep
      goto 902
 901  write(outf) 0.0,0.0
 902  continue

      deallocate(dvec)      

      close(inf)
      close(outf)
      close(nlmonbof)
    end !end program
