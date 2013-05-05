logical function contns (string,sub)


   use Vartypes
   implicit real(KREAL) (a-h, o-z)
   implicit integer(KINT) (i-n)


   character(*), intent(in) :: string, sub

!========================================================================
!  purpose: check whether string contains sub, with neglect of
!           leading and trailing blanks.
!
!  in     : string - input string
!           sub    - substring to match
!
!  out    : contns - sub found in string, or not
!
!  ======================================================================
!toc.cs.search
!========================================================================

   integer(KINT) :: ls

   ls = len_trim(sub)
   if (ls<=0) then
      contns = .true.
   else
      call csbegn (sub, is)
      if (is<=0) stop 'BUG CONTNS'
      contns = index(string,sub(is:ls))>0
   end if

end function contns

