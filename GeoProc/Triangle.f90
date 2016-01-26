module Triangle
    
    type triangle_t
        integer :: nodes(3) = 0
        real(8) :: normal(3) = 0
    contains
        procedure, public :: set_normal => set_normal_triangle_t
    end type triangle_t

contains

    subroutine set_normal_triangle_t(this, normal)
        class(triangle_t), intent(inout) :: this
        real(8), intent(in)              :: normal(3)
        this%normal = normal
    end subroutine set_normal_triangle_t

end module Triangle