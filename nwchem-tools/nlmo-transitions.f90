program nlmotrans

 use Vartypes  
 implicit none

 integer(KINT) :: i, j, k, nbas, nmo, nocc, nvir, nex, &
      iocc, jvir, ll, iex, ip, iq, ir, is, narg, nov
 character(LEN=255) :: nbo, civecs
 integer :: innbo=15, inciv=16
 character(LEN=11) :: kwnbas
 character(LEN=14) :: kwnocc
 character(LEN=5)  :: kwnex
 real(KREAL) :: dummy
 real(KREAL) :: rtemp, contrib
 real(KREAL), allocatable :: nlmomo(:,:), tmxpy(:,:,:), &
      tmxmy(:,:,:), g_x(:,:), g_y(:,:), nlmomoD(:,:), &
      virtvirt(:,:), &
      occoccT(:,:), xpynlmo(:,:), xmynlmo(:,:) 
 
 character*(LCHARS) :: cstemp
 
 real(KREAL), parameter :: zero = 0d0
 
 logical debug, slow
 logical, external :: contns
 
 ! ===========================================================================
 
 debug = .true.
 slow = .false.
 
 write (iuout,'(/1X,a/)') 'NWCHEM - NLMO-TRANSITIONS'
 write (iuout,'(/1X,a/)') 'CURRENTLY ASSUMES CLOSED SHELL'

 !Get number of arguments
 narg = iargc()

 if ( narg .lt. 2 .or. narg .gt. 2 ) then
  stop 'Usage: ./nw-nlmo-transitions nbodata.47 nwchem.civecs'
 end if

 ! the first cube file in the list is special in the sense that
 ! we'll read the grid specs from that file
 call getarg(1,nbo)
 call getarg(2,civecs)

 ! -----------
 ! open files:
 ! -----------

 open (inciv, file=civecs, STATUS="OLD", ERR=6660) 
 open (innbo, file=nbo, STATUS="OLD", ERR=6661)
 goto 100
6660 stop 'Error opening civecs, does it exist?'
6661 stop 'Error opening nbo file, does it exist?'
100 continue

 ! --------------
 ! parse civecs 
 ! --------------
 
 read(inciv,*) !TDA line either true or false 
 read(inciv,*) !integer open=2 or closed=1 shell 
 read(inciv,*) nex ! number of excitations
 read(inciv,*) nocc, dummy ! number of occupied orbitals
 read(inciv,*) nmo, dummy ! number of molecular orbitals
 read(inciv,*) ! frozen cores
 read(inciv,*) ! frozen virtuals
 read(inciv,*) nov
      
 ! A quick bit of math here:
 
 nvir = nmo - nocc
 nbas = nmo
 
 write (iuout,'(a10,i5)') 'nocc',nocc, 'nvir', nvir, 'nmo', nmo, 'nbas', nbas
 write (iuout,*)
 
 ! ----------------
 ! parse NBODATA.47
 ! ----------------


 allocate(nlmomo(nbas,nbas),nlmomoD(nbas,nbas))
 
 ! Set up our array for NBODATA.47
 ! "MOs in the NLMO basis" 
 ! SHOULD BE BLOCK DIAGONLIZED
 
 ! 1st 3 lines are useless 
 read (innbo,*)
 read (innbo,*)
 read (innbo,*)
 
 ! Read in NBO Data
 do j = 1,nbas
    read(innbo,*) (nlmomo(i,j), i=1,nbas)
 end do ! j

 ! We are done with file "NBODATA.47"
 close (innbo)
 
 write (iuout,*) 'finished reading NBO data file'
 
 ! -------------------
 ! Finish parsing civecs
 ! -------------------
 
 allocate(tmxpy(nvir,nocc,nex),tmxmy(nvir,nocc,nex),g_x(nex,nov),g_y(nex,nov))
  
 do iex = 1,nex
  do j=1,nov
    read (inciv,*) g_x(iex,j)
  enddo
 end do ! iex
 do iex = 1,nex
  do j=1,nov
    read (inciv,*) g_y(iex,j)
  enddo
 end do ! iex
 
 !We are finished with file civecs
 close (inciv)
 write (iuout,*) 'finished reading data file civecs'
 
 ! --------------------------------------------------------
 ! We are now all done with the files, we only need to work
 ! with what we have read in.
 ! --------------------------------------------------------
 
 do iex = 1, nex
   ll=0
   do iocc = 1,nocc
     do jvir = 1,nvir
          ll=ll+1
          tmxpy(jvir,iocc,iex)=g_x(iex,ll)+g_y(iex,ll)
          tmxmy(jvir,iocc,iex)=g_x(iex,ll)-g_y(iex,ll)
     end do ! jvir
    end do ! iocc
 end do ! iex
  
 if ( ll .eq. nov ) then
   write(iuout,*) 'Done Reading civecs'
 endif

 deallocate (g_x,g_y)

 if (debug) then
    ! Let's see if nlmomo is unitary
    nlmomoD = MATMUL(nlmomo, TRANSPOSE(nlmomo))
    write (iuout,*) '=============DEBUG==================='
    write (iuout,*) 'MATMUL(nlmomo, TRANSPOSE(nlmomo))'
    write (iuout,*) 'Printing Diagonal Elements of Inner Product'
    write (iuout,*) 'Total number of elements =',nbas
    do i=1,nbas
     do j=1,nbas
      if ( i .eq. j ) then
       write (iuout,*) i,j,nlmomoD(i,j)
      endif
     enddo
    enddo
    write (iuout,*) '=============END DEBUG================'
 end if ! debug
 
 
 ! ----------------
 ! compute results:
 ! ----------------
 
 allocate(virtvirt(nvir,nvir),occoccT(nocc,nocc),&
          xpynlmo(nvir,nocc),xmynlmo(nvir,nocc))

 ! Let's split up the big T into its parts.
 
 ! The virt-virt block
 do i = 1, nvir
    do j = 1, nvir
       virtvirt(i,j) = nlmomo(nocc+i,nocc+j)
    end do
 end do
 
! occ-occ
 do i = 1, nocc
    do j = 1, nocc
       occoccT(j,i) = nlmomo(i,j)
    end do
 end do
 
 do iex = 1,nex
    write (iuout,*)
    write (iuout,'(1X,a,1X,i5//1X,a)') 'Excitation #', iex, &
         'MO Analysis (5% or greater)'
    
    ! first check <X+Y | X-Y> for this transition
    
    rtemp = zero
    do jvir = 1,nvir
       do iocc = 1,nocc
          contrib = tmxpy(jvir,iocc,iex) * tmxmy(jvir,iocc,iex)
          rtemp = rtemp + contrib
          if (abs(contrib).ge.0.05) write (iuout,'(1X,a,i5,a,i5,a,f8.2,a)') &
               'occ ',iocc,' -> vir ', jvir+nocc,' :',(contrib*100d0),'%'
       end do
    end do
    
    ! print <X+Y | X-Y>
    write (iuout,'(1X,a,f15.8)') &
         '                              <X+Y | X-Y> : ',rtemp
    
    ! ---------------------------------------------------------------------
    ! X+Y in NLMO basis, for the transition density in the NLMO basis
    ! X-Y in NLMO basis, needed for the transition current-density
    ! ---------------------------------------------------------------------

    if (slow) then

       ! --------------------------------------------
       ! for debugging: calculate the transformations
       ! explicitly in 4-fold loops.
       ! see below for matrix multiplication code
       ! --------------------------------------------

       xpynlmo = 0 
       do iq = 1,nocc
          do ip = 1,nvir
             rtemp = zero
             do iocc = 1,nocc
                do jvir = 1, nvir
                   rtemp = rtemp + tmxpy(jvir,iocc,iex)  &
                        * nlmomo(iq,     iocc     ) &
                        * nlmomo(ip+nocc,jvir+nocc)
                end do
             end do
             xpynlmo (ip,iq) = rtemp
          end do
       end do

       xmynlmo = 0 
       do iq = 1,nocc
          do ip = 1,nvir
             rtemp = zero
             do iocc = 1,nocc
                do jvir = 1, nvir
                   rtemp = rtemp +   tmxmy(jvir,iocc,iex) &
                        * nlmomo(iq,     iocc     ) &
                        * nlmomo(ip+nocc,jvir+nocc)
                end do
             end do
             xmynlmo (ip,iq) = rtemp
          end do
       end do
       
       if (debug) then ! this is slooow !
          ! for debugging also calculate the tracs of the product of 
          ! tpq and spq in a full 6-fold loop. don't try this at home ...
          rtemp = 0
          do iocc=1,nocc
             do jvir = 1,nvir
                do ir = 1, nvir
                   do is = 1, nocc
                      do ip = 1, nvir
                         do iq = 1, nocc
                            rtemp = rtemp + tmxpy(jvir,iocc,iex) * tmxmy(jvir,iocc,iex) & 
                                 * nlmomo(ir+nocc, ip+nocc) &
                                 * nlmomo(jvir+nocc,ip+nocc) &
                                 * nlmomo(is, iq) &
                                 * nlmomo(iocc,iq)
                         end do
                      end do
                   end do
                end do
             end do
          end do
          write (iuout,'(1X,a,f15.8)') 'this should be one:',rtemp
       end if ! ! debug and slow

    else 

       ! --------------------------------------------------------
       ! Use matrix-multiplications to get X+Y, X-Y in NLMO basis
       ! --------------------------------------------------------
       
       
       xpynlmo = MATMUL(MATMUL(virtvirt,tmxpy(:,:,iex)),occoccT)
       xmynlmo = MATMUL(MATMUL(virtvirt,tmxmy(:,:,iex)),occoccT)
 
    end if ! slow
       
    write (iuout,'(/1X,a)') 'NLMO Analysis (5% or greater)'
    rtemp = zero
    do jvir = 1,nvir
       do iocc = 1,nocc
          contrib = xpynlmo(jvir,iocc)*xmynlmo(jvir,iocc) 
          rtemp = rtemp + contrib
          if (abs(contrib).ge.0.05) then
             write (iuout,'(1X,a,i5,a,i5,a,f8.2,a)') &
                  'occ ',iocc,' -> vir ', jvir+nocc,' :',(contrib*100d0),'%'
          end if
       end do ! iocc
    end do ! jvir
    
    write (iuout,'(1X,a,f15.8)') & 
         'Total from NLMO analysis (should be unity): ',rtemp
    
 end do ! iex
 deallocate (nlmomo,tmxpy, tmxmy, virtvirt, occoccT,&
      xpynlmo, xmynlmo)
 
end program nlmotrans
