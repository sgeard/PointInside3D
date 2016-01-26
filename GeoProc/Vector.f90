! Functions for vectors
module Vector

    interface operator(.cross.)
        module procedure cross_product
    end interface

    interface operator(.dot.)
        module procedure d_product
    end interface
    
contains
    
    function cross_product(u, v) result(r)
        real(8), intent(in) :: u(3), v(3)
        real(8)    :: r(3)
        integer(1) :: i, j, k
        do i=1,3
            j = merge(1,i+1,i==3)
            k = merge(1,j+1,j==3)
            r(i) = u(j)*v(k)-u(k)*v(j)
        end do
    end function cross_product

    function d_product(u, v) result(r)
        real(8), intent(in) :: u(3), v(3)
        real(8)    :: r
        r = dot_product(u,v)
    end function d_product

end module Vector
