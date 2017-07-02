!-------------------------------------------------------------------------------
! Copyright (c) 2016 The University of Tokyo
! This software is released under the MIT License, see LICENSE.txt
!-------------------------------------------------------------------------------

module hecmw_precond_44
  use hecmw_util
  implicit none

  private

  public :: hecmw_precond_44_setup
  public :: hecmw_precond_44_clear
  public :: hecmw_precond_44_apply

contains

  !C
  !C***
  !C*** hecmw_precond_44_setup
  !C***
  !C
  subroutine hecmw_precond_44_setup(hecMAT, hecMESH, sym)
    use hecmw_util
    use hecmw_matrix_misc
    use hecmw_precond_BILU_44
    use hecmw_precond_DIAG_44
    use hecmw_precond_SSOR_44
    use hecmw_precond_nn
    implicit none
    type (hecmwST_matrix),     intent(inout) :: hecMAT
    type (hecmwST_local_mesh), intent(inout) :: hecMESH
    integer(kind=kint), intent(in) :: sym

    integer(kind=kint ) :: PRECOND, iterPREmax

    iterPREmax = hecmw_mat_get_iterpremax( hecMAT )
    if (iterPREmax.le.0) return

    PRECOND = hecmw_mat_get_precond( hecMAT )

    SELECT CASE(PRECOND)
      CASE(1,2)
        call hecmw_precond_SSOR_44_setup(hecMAT)
      CASE(3)
        call hecmw_precond_DIAG_44_setup(hecMAT)
      CASE(10,11,12)
        call hecmw_precond_BILU_44_setup(hecMAT)
      CASE DEFAULT
        call hecmw_precond_nn_setup(hecMAT, hecMESH, sym)
    END SELECT

  end subroutine hecmw_precond_44_setup

  !C
  !C***
  !C*** hecmw_precond_44_clear
  !C***
  !C
  subroutine hecmw_precond_44_clear(hecMAT)
    use hecmw_util
    use hecmw_matrix_misc
    use hecmw_precond_BILU_44
    use hecmw_precond_DIAG_44
    use hecmw_precond_SSOR_44
    use hecmw_precond_nn

    implicit none
    type (hecmwST_matrix), intent(inout) :: hecMAT

    integer(kind=kint ) :: PRECOND, iterPREmax

    iterPREmax = hecmw_mat_get_iterpremax( hecMAT )
    if (iterPREmax.le.0) return

    PRECOND = hecmw_mat_get_precond( hecMAT )

    SELECT CASE(PRECOND)
      CASE(1,2)
        call hecmw_precond_SSOR_44_clear(hecMAT)
      CASE(3)
        call hecmw_precond_DIAG_44_clear()
      CASE(10,11,12)
        call hecmw_precond_BILU_44_clear()
      CASE DEFAULT
        call hecmw_precond_nn_clear(hecMAT)
    END SELECT
    
  end subroutine hecmw_precond_44_clear

  !C
  !C***
  !C*** hecmw_precond_44_apply
  !C***
  !C
  subroutine hecmw_precond_44_apply(hecMESH, hecMAT, R, Z, ZP, time_precond, COMMtime)
    use hecmw_util
    use hecmw_matrix_misc
    use hecmw_precond_BILU_44
    use hecmw_precond_DIAG_44
    use hecmw_precond_SSOR_44
    use hecmw_solver_las_44
    use hecmw_precond_nn

    implicit none
    type (hecmwST_local_mesh), intent(in) :: hecMESH
    type (hecmwST_matrix), intent(in)     :: hecMAT
    real(kind=kreal), intent(in) :: R(:)
    real(kind=kreal), intent(out) :: Z(:), ZP(:)
    real(kind=kreal), intent(inout) :: time_precond
    real(kind=kreal), intent(inout) :: COMMtime

    integer(kind=kint ) :: N, NP, NNDOF, NPNDOF
    integer(kind=kint ) :: PRECOND, iterPREmax
    integer(kind=kint ) :: i, iterPRE
    real(kind=kreal) :: START_TIME, END_TIME

    N = hecMAT%N
    NP = hecMAT%NP
    NNDOF = N * 4
    NPNDOF = NP * 4
    PRECOND = hecmw_mat_get_precond( hecMAT )
    iterPREmax = hecmw_mat_get_iterpremax( hecMAT )

    if (iterPREmax.le.0) then
      do i= 1, NNDOF
        Z(i)= R(i)
      enddo
      return
    endif

    !C===
    !C +----------------+
    !C | {z}= [Minv]{r} |
    !C +----------------+
    !C===

    do i= 1, NNDOF
      ZP(i)= R(i)
    enddo
    do i= NNDOF+1, NPNDOF
      ZP(i) = 0.d0
    enddo

    do i= 1, NPNDOF
      Z(i)= 0.d0
    enddo

    do iterPRE= 1, iterPREmax

      START_TIME = hecmw_Wtime()

      SELECT CASE(PRECOND)
        CASE(1,2)
          call hecmw_precond_SSOR_44_apply(ZP)
        CASE(3)
          call hecmw_precond_DIAG_44_apply(ZP)
        CASE(10,11,12)
          call hecmw_precond_BILU_44_apply(ZP)
        CASE DEFAULT
          call hecmw_precond_nn_apply(hecMESH, hecMAT, R, Z, ZP, time_precond, COMMtime)
          return 
      END SELECT
      END_TIME = hecmw_Wtime()
      time_precond = time_precond + END_TIME - START_TIME

      !C
      !C-- additive Schwartz
      !C
      do i= 1, NNDOF
        Z(i)= Z(i) + ZP(i)
      enddo

      if (iterPRE.eq.iterPREmax) exit

      !C--    {ZP} = {R} - [A] {Z}

      call hecmw_matresid_44 (hecMESH, hecMAT, Z, R, ZP, COMMtime)

    enddo

  end subroutine hecmw_precond_44_apply

end module hecmw_precond_44
