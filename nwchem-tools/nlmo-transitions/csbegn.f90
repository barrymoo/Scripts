subroutine csbegn (string, is)

   use Vartypes
   implicit real(KREAL) (a-h, o-z)
   implicit integer(KINT) (i-n)

!
   character*(*) string

!========================================================================
!  purpose: determine the position of the first non-blank character
!           in a string.
!
!  in     : string - character string to be analyzed
!
!  out    : is     - position of first non-blank (0 if not found)
!
!  ======================================================================
!toc.cs.search
!========================================================================

   parameter (nchop = 8)
   character*(nchop) space8
   parameter (space8 = '        ')

   l2 = len(string)
   l1 = 0
 10 if (l2-l1>=nchop) then
      if (string(l1+1:l1+nchop)/=space8) goto 20
      l1 = l1 + nchop
      goto 10
   end if
!
 20 l1 = l1 + 1
   if (l1>l2) then
      l1 = 0
   else if (string(l1:l1)==' ') then
      goto 20
   end if
!
   is = l1
!
   return
end

