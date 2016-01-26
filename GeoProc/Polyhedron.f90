module Polyhedron
    use Triangle
    use Vector

    implicit none
    
    type polyhedron_t
        private
        real(8), allocatable          :: nodes(:,:)              ! Points defining the boundary
        type(triangle_t), allocatable :: faces(:)                ! The triangular faces 
        integer                       :: face_count = 0          ! The number of faces
        real(8)                       :: centroid(3) = 0         ! A point inside the polyhedron
        real(8)                       :: bmax(3) = -huge(1.0d0)  ! All points are smaller (or equal to) this
        real(8)                       :: bmin(3) = huge(1.0d0)   ! All points are greater (or equal to) this
    contains
        procedure, public             :: create => create_poly_t
        procedure, public             :: add_triangle => add3_poly_t
        procedure, public             :: add_quad => add4_poly_t
        generic, public               :: operator(.isinside.) => point_in_poly_t
        procedure, private, pass(f)   :: point_in_poly_t
        generic                       :: write(formatted) => write_poly_t
        procedure, public             :: write_poly_t
    end type polyhedron_t
    
contains

    subroutine write_poly_t(this, u, iotype, v_list, iostat, iomsg)
        use iso_fortran_env
        class(polyhedron_t), intent(in) :: this
        integer, intent(in)           :: u
        character(*), intent(in)      :: iotype
        integer, intent(in)           :: v_list(:)
        integer, intent(out)          :: iostat
        character(*), intent(inout)   :: iomsg
        character(len=:), allocatable :: fmt
        integer :: i, j, k
        iostat = 0
        iomsg = ''
        do i=1,this%face_count
            associate (fnodes => this%faces(i)%nodes)
                do j=1,4
                    k = merge(1,j,j==4)
                    write(u,'(/3f12.6)') this%nodes(:,fnodes(k))
                end do
            end associate
        end do
    end subroutine write_poly_t

    
    subroutine create_poly_t(this, num_faces, points)
        class(polyhedron_t), intent(out) :: this
        integer, intent(in)               :: num_faces
        real(8), intent(in)               :: points(:,:)
        integer :: i
        
        ! Store the points
        allocate(this%nodes,source=points)
        
        ! Make space for all the triangular faces
        allocate(this%faces(num_faces))
        
        ! Store the min and max extents of the boundary points
        do i=1,3
            associate (pts => points(i,:))
                this%centroid(i) = sum(pts)
                where (pts > this%bmax)
                    this%bmax = pts
                end where
                where (pts < this%bmin)
                    this%bmin = pts
                end where
            end associate
        end do
        
        ! Store the centroid
        this%centroid = this%centroid/size(points,2)
    end subroutine create_poly_t
    
    ! Add a triangular face
    subroutine add3_poly_t(this,n)
        class(polyhedron_t), intent(inout) :: this
        integer,  intent(in) :: n(3)
        type(triangle_t)     :: tri
        real(8) ::  p(3), q(3), r(3)
        
        ! Create a triangle from the given node sequence
        tri = triangle_t(n)
        
        ! The two edge vectors starting at node n(1)
        p = this%nodes(:,n(2))-this%nodes(:,n(1))
        q = this%nodes(:,n(3))-this%nodes(:,n(1))
        
        ! Calculate a perpendicular to the face
        r = (p .cross. q)
        
        ! Reverse if it is pointing towards the centroid so that the normal
        ! is directed out of the polyhedron
        if (dot_product(this%centroid-this%nodes(:,n(1)),r) > 0) r = -r
        
        ! Set the outward normal for this face
        call tri%set_normal(r)
        
        ! Store this face as part of the polyhedron
        this%face_count = this%face_count+1
        this%faces(this%face_count) = tri
    end subroutine add3_poly_t
    
    ! Add a quadrilateral face by splitting it into two triangles
    subroutine add4_poly_t(this,n)
        class(polyhedron_t), intent(inout) :: this
        integer,  intent(in) :: n(4)
        call this%add_triangle([n(1),n(2),n(3)])
        call this%add_triangle([n(1),n(3),n(4)])
    end subroutine add4_poly_t
    
    ! Do the '.isinside.' work
    logical function point_in_poly_t(p, f)
        class(polyhedron_t), intent(in) :: f
        real(8),  intent(in) :: p(3)
        integer :: i
        real(8) :: v(3)
        
        ! The default is 'outside'
        point_in_poly_t = .false.
        
        ! Do nothing if the point is outside the bounding box
        if (all(p < f%bmin) .or. all(p > f%bmax)) then
            return
        end if
        
        all_faces: do i=1,f%face_count
            associate (face => f%faces(i))
                ! Create a vector from the first node of this face to the given point
                v = p - f%nodes(:,face%nodes(1))
                
                ! If this is in the inward direction relative to the face normal then the point is outside
                if ((v .dot. face%normal) >  0) then
                    return
                end if
            end associate
        end do all_faces
        point_in_poly_t = .true.
    end function point_in_poly_t
    
end module Polyhedron

