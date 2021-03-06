!-------------------------------------------------------------------------------
! Copyright (c) 2016 The University of Tokyo
! This software is released under the MIT License, see LICENSE.txt
!-------------------------------------------------------------------------------
!> \brief  This module contains functions for interpolation in 2 node
!!     line element   (Langrange  interpolation)
module shape_line2n
  integer, parameter, private :: kreal = kind(0.0d0)

contains
  subroutine ShapeFunc_line2n(lcoord,func)
    real(kind=kreal), intent(in) :: lcoord(1)
    real(kind=kreal) :: func(2)
    func(1) = 0.5d0*(1.d0-lcoord(1))
    func(2) = 0.5d0*(1.d0+lcoord(1))
  end subroutine

  subroutine ShapeDeriv_line2n(func)
    real(kind=kreal) :: func(2,1)
    func(1,1) = -0.5d0
    func(2,1) = 0.5d0
  end subroutine

  subroutine Shape2ndDeriv_line2n(func)
    real(kind=kreal) :: func(2,1,1)
    func(:,:,:) = 0.d0
  end subroutine

end module
