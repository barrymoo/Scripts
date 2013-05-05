subroutine csgeti (string, is, ival)

   use Vartypes
   implicit real(KREAL) (a-h, o-z)
   implicit integer(KINT) (i-n)

!
   character*(*) string

!========================================================================
!  purpose: get the next integer from a string
!
!  in     : string - string to get integer from
!
!  in-out : is     - on input, position to start searching.
!                    if is <= 0: start at 1
!                    on output (only when  positive on input) the 
!                    position after the integer, or 0 if no integer found
!
!  out    : ival   - the next integer (if found)
!
!  remarks: any preceding non-numeric data is skipped
!
!  ======================================================================
!toc.cs.get
!========================================================================

   character*(*) digits
!
   parameter (digits = '0123456789')

   istart = max(1,is)
   lx     = len(string)
   ip     = 0

!  ----------------------
!  locate the first digit
!  ----------------------

   do i0 = istart, lx
      if (index(digits,string(i0:i0))>0) then

!        ----------------------------------------------------------------------
!        scan the integer, by transforming all successive digits from character
!        into numerical value, and updating the total integer value
!        ----------------------------------------------------------------------

         m0   = ichar('0')
         ival = 0
!
         ip = i0
 10      ival = 10*ival + ichar(string(ip:ip)) - m0
         ip = ip + 1
!
         if (ip<=lx) then
            if (index(digits,string(ip:ip))>0) goto 10
         end if

!        ---------------------------------------------------------------------
!        check for a preceding minus sign (if the integer did not start at the
!        first character of the input string)
!        ---------------------------------------------------------------------

         if (i0>istart) then
            if (string(i0-1:i0-1)=='-') ival = - ival
         end if
!
         goto 60
!
      end if
   end do 
!
 60 if (is>0) is = ip
!
   return
end

