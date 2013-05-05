module Vartypes
      implicit none
                
!     kind parameters for integers and reals             
      integer, parameter :: KINT=KIND(0)
      integer(KINT), parameter :: KREAL=KIND(0D0)

!     some common variables:
      integer(KINT), parameter :: iuout =6
      integer(KINT), parameter :: LCHARS=256
end module Vartypes

