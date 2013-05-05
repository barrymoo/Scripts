subroutine moutr (a, nra, nca, mode, aname)

!========================================================================
!  purpose: print a rectangular matrix.
!
!  input  : a     - matrix to be printed
!           nra   - number of rows of a
!           nca   - number of columns of a
!           mode  - print mode (character*(*))
!                   if mode=R: a is printed in regular form
!                   if mode=T: a is printed in transposed form
!           aname - text used as header (character*(*))
!
!  remark * if mode is not equal to r or t the regular mode is used.
!
!  ======================================================================
!toc.matvec.output
!========================================================================


   use Vartypes
!   implicit real(KREAL) (a-h, o-z)
!   implicit integer(KINT) (i-n)

   character*(*) aname,mode

!   integer, parameter :: iuout=6

   dimension a(nra,nca)

   call moutrp (a, nra, 1, nra, nca, 1, nca, mode, aname)

end subroutine moutr


subroutine moutrp (a, nra, nra1, nra2, nca, nca1, nca2, mode, aname)

!========================================================================
!  purpose: print (part of) a rectangular matrix.
!
!  input  : a     - matrix to be printed
!           nra   - number of rows of a
!           nra1  - first row to be printed
!           nra2  - last row to be printed
!           nca   - number of columns of a
!           nca1  - first column to be printed
!           nca2  - last column to be printed
!           mode  - print mode (character*(*))
!                   if mode = r a is printed in regular form
!                   if mode = t a is printed in transposed form
!           aname - text used as header (character*(*))
!
!  method : if the matrix is printed in regular form, its columns appear
!           as columns on the output file. this is accomplished by
!           looping over groups of columns, with in each group at most
!           the number of columns that fit on a line.
!           if the matrix is printed in transposed form, its rows appear
!           looping over groups of rows, with in each group at most the
!           number of rows that fit on a line.
!
!  remark * if mode is not equal to r or t the regular mode is used.
!
!  ======================================================================
!toc.matvec.output
!========================================================================

!
   use Vartypes
!   implicit real(KREAL) (a-h, o-z)
!   implicit integer(KINT) (i-n)

   character*(*) aname,mode

!   integer(KINT), parameter :: iuout=6

   parameter (lgroup = 4)

   dimension a(nra,nca)

!   call csend (aname, ls)
   write (iuout,6010) trim(aname)

!  -------------------------------
!  print matrix in transposed form
!  -------------------------------

   if (mode=='T') then

      do i1 = nra1, nra2, lgroup
         i2 = min(i1+(lgroup-1),nra2)
         write (iuout,6025) 'row   ', (i,i = i1,i2)
         write (iuout,6020) 'column'
         do j = nca1, nca2
            write (iuout,6030) j, (a(i,j),i = i1,i2)
         end do 
      end do 

   else

!     ----------------------------
!     print matrix in regular form
!     ----------------------------

      do j1 = nca1, nca2, lgroup
         j2 = min(j1+(lgroup-1),nca2)
         write (iuout,6025) 'column', (j,j = j1,j2)
         write (iuout,6020) 'row   '
         do i = nra1, nra2
            write (iuout,6030) i, (a(i,j),j = j1,j2)
         end do 
      end do 

   end if

 6010 format(//' ======  ',a)
 6020 format(1x,a7,i12,4i20)
 6025 format(/1x,a7,i12,4i20)
 6030 format(1x,i4,2x,1p,4e22.14)
end subroutine moutrp

