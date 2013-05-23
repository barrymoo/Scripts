program cub2t41

  ! converts Gaussian cube files to ADF TAPE41 (ascii) to be viewed with
  ! adfview. See README file for usage information.

  implicit none 

  integer(kind(0)), parameter :: KREAL=kind(1.0d0), KINT=kind(1), LCHARS=255

  character*(LCHARS) :: cubfile, long

  integer(KINT), parameter :: iucub=7, iuinp=8, iuout=6, iu41=9

  integer(KINT) :: nnuc, npts(3), i, j, k, ii, ngrid, ndum, nread, nn, iel, is

  real(KREAL) :: vectr(3,3), startp(3), dummy, dV, cube_int

  real(KREAL), allocatable :: xyznuc(:,:), cubvalue(:,:,:), qtch(:), froc(:), eps(:)
  integer(KINT), allocatable :: nuctyp(:)

  integer :: icube, ncubes

  integer(KINT), parameter :: nelmnt=118
  character(3) :: symbol(0:nelmnt)


  ! ==========================================================================

  ! initialize element symbols

  do iel = 0, nelmnt
    symbol(iel) = '***'
  end do
  
  nread = 0
  
  long = 'XX H  He Li Be B  C  N  O  F  Ne '//'Na Mg Al Si P  S  Cl Ar K  Ca '//                  &
     'Sc Ti V  Cr Mn Fe Co Ni Cu Zn '//'Ga Ge As Se Br Kr Rb Sr Y  Zr '//                     &
     'Nb Mo Tc Ru Rh Pd Ag Cd In Sn '
  
  nn = (1+nelmnt) - nread
  is = 1
  call csgtns (long, is, nn, symbol(nread))
  nread = nread + nn
  
  long = 'Sb Te I  Xe Cs Ba La Ce Pr Nd '//'Pm Sm Eu Gd Tb Dy Ho Er Tm Yb '//                     &
     'Lu Hf Ta W  Re Os Ir Pt Au Hg '//'Tl Pb Bi Po At Rn Fr Ra Ac Th '//                     &
     'Pa U  Np Pu Am Cm Bk Cf Es Fm '//'Md No Lr Rf Db Sg Bh Hs Mt Ds '//                     &
     'Rg Cn Uut Uuq Uup Uuh Uus Uuo'
  
  nn = (1+nelmnt) - nread
  is = 1
  call csgtns (long, is, nn, symbol(nread))
  nread = nread + nn

  if (nread/=(1+nelmnt)) stop 'wrong nr. of elements'


  open (iucub,file='cubefiles',status='old',err=7001)
  read  (iucub,*,err=7002) ncubes
  write (iuout,'(1x,a,i3,a)') 'will process ',ncubes,' cube files'
  if (ncubes.lt.1) stop 'ncubes<1. Nothing to do'

  ! the first cube file in the list is special in the sense that
  ! we'll read the grid specs from that file

  read (iucub,*) cubfile ! name of first cube file
  open (iuinp,file=trim(cubfile),status='old',err=7003)

  read (iuinp, *, err=7004, end=7004)! Molecule title
  read (iuinp, *, err=7004, end=7004)! Field title
  ! Natoms, grid origin
  read (iuinp, *, err=7004, end=7004) nnuc, startp(1), startp(2), startp(3)
  read (iuinp, *, err=7004, end=7004) npts(1), vectr(1,1), vectr(2,1), vectr(3,1)
  read (iuinp, *, err=7004, end=7004) npts(2), vectr(1,2), vectr(2,2), vectr(3,2)
  read (iuinp, *, err=7004, end=7004) npts(3), vectr(1,3), vectr(2,3), vectr(3,3)

  
  ! allocate memory

  allocate(nuctyp(nnuc))
  allocate(xyznuc(3,nnuc),qtch(nnuc))

  do ii = 1, nnuc
    read (iuinp, *, err=7004, end=7004) nuctyp(ii), qtch(ii), xyznuc(1,ii),           &
       xyznuc(2,ii), xyznuc(3,ii)
    !       write (iuout,*) nuctyp(ii), xyznuc(1,ii), xyznuc(2,ii), xyznuc(3,ii)
  end do ! ii

  ngrid = npts(1)*npts(2)*npts(3)

  write (iuout,*) 'Grid specs and molecular coords read from ',trim(cubfile)
  write (iuout,'(a,i8,a,i8,a,i8,a,i8)') ' Number of points:', npts(1), ' x ', npts(2), ' x ',  &
     npts(3), ' = ', ngrid
  write (iuout,*)

  ! set up grid header of TAPE41 ascii file. 
  ! convert to binary using
  ! $ADFBIN/udmpkf < ascii.t41 binary.t41

  open (iu41, file='ascii.t41',status='unknown')
  write (iu41,'(a)') 'Grid'
  write (iu41,'(a)') 'Start_point'
  write (iu41,'(a)') '         3         3         2'
  write (iu41,'(3e28.18)') startp(1:3)

  write (iu41,'(a)') 'Grid'
  write (iu41,'(a)') 'nr of points x'
  write (iu41,'(a)') '         1         1         1'
  write (iu41,'(i10)') npts(1)

  write (iu41,'(a)') 'Grid'
  write (iu41,'(a)') 'nr of points y'
  write (iu41,'(a)') '         1         1         1'
  write (iu41,'(i10)') npts(2)

  write (iu41,'(a)') 'Grid'
  write (iu41,'(a)') 'nr of points z'
  write (iu41,'(a)') '         1         1         1'
  write (iu41,'(i10)') npts(3)

  write (iu41,'(a)') 'Grid'
  write (iu41,'(a)') 'x-vector'
  write (iu41,'(a)') '         3         3         2'
  write (iu41,'(3e28.18)') vectr(:,1)

  write (iu41,'(a)') 'Grid'
  write (iu41,'(a)') 'y-vector'
  write (iu41,'(a)') '         3         3         2'
  write (iu41,'(3e28.18)') vectr(:,2)

  write (iu41,'(a)') 'Grid'
  write (iu41,'(a)') 'z-vector'
  write (iu41,'(a)') '         3         3         2'
  write (iu41,'(3e28.18)') vectr(:,3)

  write (iu41,'(a)') 'Grid'
  write (iu41,'(a)') 'total nr of points'
  write (iu41,'(a)') '         1         1         1'
  write (iu41,'(i10)') ngrid

  write (iu41,'(a)') 'Grid'
  write (iu41,'(a)') 'nr of symmetries'
  write (iu41,'(a)') '         1         1         1'
  write (iu41,'(i10)') 1

  write (iu41,'(a)') 'Grid'
  write (iu41,'(a)') 'labels'
  write (iu41,'(a)') '       160       160         3'
  write (iu41,'(a1,79x/80x)') 'A'

  write (iu41,'(a)') 'Grid'
  write (iu41,'(a)') 'unrestricted'
  write (iu41,'(a)') '         1         1         4'
  write (iu41,'(a)') 'F'

  write (iu41,'(a)') 'SCF_A'
  write (iu41,'(a)') 'nr of orbitals'
  write (iu41,'(a)') '         1         1         1'
  write (iu41,'(i10)') ncubes

  write (iu41,'(a)') 'SCF_A'
  write (iu41,'(a)') 'Occupations'
  write (iu41,'(3i10)') ncubes,ncubes,2
  write (iu41,'(3e28.18)') (2d0,i=1,ncubes)

  write (iu41,'(a)') 'SCF_A'
  write (iu41,'(a)') 'Eigenvalues'
  write (iu41,'(3i10)') ncubes,ncubes,2
  write (iu41,'(3e28.18)') (real(i),i=1,ncubes)

  ! write geometry info

  write (iu41,'(a)') 'Geometry'
  write (iu41,'(a)') 'nnuc'
  write (iu41,'(a)') '         1         1         1'
  write (iu41,'(i10)') nnuc

  write (iu41,'(a)') 'Geometry'
  write (iu41,'(a)') 'labels'
  write (iu41,'(3i10)') nnuc*160,nnuc*160,3
  write (iu41,'(a3,77x/80x)') (symbol(nuctyp(i)),i=1,nnuc)

  write (iu41,'(a)') 'Geometry'
  write (iu41,'(a)') 'xyznuc'
  write (iu41,'(3i10)') nnuc*3,nnuc*3,2
  write (iu41,'(3e28.18)') xyznuc(:,:)

  write (iu41,'(a)') 'Geometry'
  write (iu41,'(a)') 'qtch'
  write (iu41,'(3i10)') nnuc,nnuc,2
  write (iu41,'(3e28.18)') qtch(:)

  write (iu41,'(a)') 'Geometry'
  write (iu41,'(a)') 'unit of length'
  write (iu41,'(3i10)') 1,1,2
  write (iu41,'(3e28.18)') 1.0_KREAL

  close (iuinp)
  rewind(iucub)
  read(iucub,*) ndum


  ! grid specs and molecule info read. Now process the cube files one by one


  allocate (cubvalue(npts(1),npts(2),npts(3)))

  do icube=1,ncubes

    read (iucub,*) cubfile 
    open (iuinp,file=trim(cubfile),status='old',err=7003)

    ! read grid section and molecule. This time we don't use the data
    
    read (iuinp, *, err=7004, end=7004)! Molecule title
    read (iuinp, *, err=7004, end=7004)! Field title
    read (iuinp, *, err=7004, end=7004) nnuc, startp(1), startp(2), startp(3)
    read (iuinp, *, err=7004, end=7004) npts(1), vectr(1,1), vectr(2,1), vectr(3,1)
    read (iuinp, *, err=7004, end=7004) npts(2), vectr(1,2), vectr(2,2), vectr(3,2)
    read (iuinp, *, err=7004, end=7004) npts(3), vectr(1,3), vectr(2,3), vectr(3,3)
    do ii = 1, nnuc
      read (iuinp, *, err=7004, end=7004) nuctyp(ii), qtch(ii), xyznuc(1,ii),           &
         xyznuc(2,ii), xyznuc(3,ii)
      !       write (iuout,*) nuctyp(ii), xyznuc(1,ii), xyznuc(2,ii), xyznuc(3,ii)
    end do ! ii

    cubvalue = 0

    do i = 1, npts(1)
      do j = 1, npts(2)
        read (iuinp, *, err=7005, end=7005) cubvalue(i,j,:)
      end do
    end do

    ! assuming that the grid is rectangular, take an approximate
    ! integral of the function (beware that one huge density value can
    ! throw it all off, so this is not very accurate)
    
    dV = vectr(1,1) * vectr(2,2) * vectr(3,3)
    cube_int = 0d0
    do i = 1, npts(1)
      do j = 1, npts(2)
        do k = 1,npts(3)
          if (abs( cubvalue(i,j,k)).lt.1e3) then
            cube_int = cube_int + cubvalue(i,j,k) * dV
          end if
        end do
      end do
    end do
    write (iuout,'(1x,a,i3,a,f12.3)') 'CubeFile ',icube,': int = ',cube_int

    ! write cube data to ascii TAPE41
    
    write (iu41,'(a)') 'SCF_A'
    write (iu41,'(i10)') icube
    write (iu41,'(3i10)') ngrid,ngrid,2
    write (iu41,'(3e28.18)') ((cubvalue(:,j,i),j=1,npts(2)),i=1,npts(3))

    close (iuinp)

  end do ! icube

  ! all done. Clean up, and exit gracefully

  close(iucub)
  close (iu41,status='keep')

  deallocate(cubvalue, nuctyp, xyznuc, qtch)

  stop 'normal termination'

7001 stop 'Error opening file ''cubfiles'''
7002 stop 'Error readinf number of cube files from file ''cubfile'''
7003 write (iuout,*) 'error opening cube file '//trim(cubfile)
     stop 'aborting'
7004 write (iuout,*) 'error reading grid or XYZ from cube file '//trim(cubfile)
     stop 'aborting'
7005 write (iuout,*) 'error reading cube data from cube file '//trim(cubfile)
     stop 'aborting'

contains

subroutine csgtns (string, is, n, text)

   implicit real(KREAL) (a-h, o-z)
   implicit integer(KINT) (i-n)

!
   character*(*) string,text
   dimension text(*)

!------------------------------------------------------------------------
!  **====================================================================
!  PURPOSE: find a sequence of substrings in a string 
!           see routine csgets for the definition of a substring
!
!  IN: string: string to get substrings from
!
!  IN-OUT: n: number of items (to be found / actually found)
!          is: position to start searching/ after n-th item
!              if is <= 0 on input the search starts at position 1
!              and no value is assigned to is on output
!
!  OUT: text: array of substrings
!  *=====================================================================
!toc.cs.get
!------------------------------------------------------------------------

   ip = max(1,is)
   nx = n
   n  = 0
 10 if (n<nx) then
      call csgets (string, ip, text(n+1))
      if (ip>0) then
         n = n + 1
         goto 10
      end if
   end if
!
   if (is>0) is = ip
!
   return
 end subroutine csgtns


subroutine csgets (string, is, text)

!   use Vartypes
   implicit real(KREAL) (a-h, o-z)
   implicit integer(KINT) (i-n)

!
   character*(*) string,text

!========================================================================
!  purpose: get the next non-empty substring from a string (i.e. the part
!           separated from the remaining part by any from a set of
!           delimiters: comma, blank, equal sign)
!
!  in     : string - string to get substring from
!
!  in-out : is     - on input, position to start searching.
!                    if is <= 0: start at 1
!                    on output (only when  positive on input) the 
!                    position after the substring, 
!                    or 0 if no substring found.
!
!  out    : text   - the next non-empty substring (if found)
!
!  *=====================================================================
!toc.cs.get
!========================================================================

   character*(*) delim
   parameter (delim = ' ,=')
   logical inQuotes, quoted

  istart = max(1,is)
   lx     = len(string)
!
   inQuotes = .false.
   quoted = .false.
   ip = 0
   do i = istart, lx
      ! Find first non-delimiter
      if (index(delim,string(i:i))<=0) then
!
         ip = i
         ! Check if the first non-delimiter char is a double quote
         if (string(ip:ip)=='"') then
            inQuotes = .true.
         end if
 10      ip = ip + 1
         if (ip<=lx) then
            if (.not.inQuotes .and. index(delim,string(ip:ip))<=0 .or. &
                inQuotes .and. .not. string(ip:ip)=='"') then
               ! if a delimiter is met and it's not quoted, then exit
               goto 10
            else if (inQuotes .and. string(ip:ip)=='"') then
               ! there is an opening quoted present and now we found the closing one
               inQuotes = .false.
               quoted = .true.
            end if
         end if
!
         if (quoted) then
            text = string(i+1:ip-1)
            if (is>0) is = ip+1
            return
         else
            text = string(i:ip-1)
            if (is>0) is = ip
            return
         end if
      end if
   end do
!
   if (is>0) is = ip
!
   return
 end subroutine csgets


end program cub2t41
  
