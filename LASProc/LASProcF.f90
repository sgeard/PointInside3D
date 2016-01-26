program LASProc
    use Polyhedron
    implicit none
    
    character(len=*), parameter :: infile = '../LASInput/cube.las'
    integer             :: u, o, i, j, pcount
    integer(4)          :: offset_to_point_data_value, record_num, record_loc
    integer(4)          :: p(3)
    integer(2)          :: record_len
    real(8)             :: scale(3), offset(3)
    real(8)             :: q(3)
    type(polyhedron_t)  :: geo_poly
    
    ! Define the polyhedron
    call create_geo_poly
    
    open(newunit=u,file=infile,access='stream',form='unformatted')

    ! Read 4 bytes from pos 97
    read(u,pos=97) offset_to_point_data_value
    
    ! Skip 5 bytes so read 2 bytes from (97+4+1)
    read(u,pos=106) record_len
    
    ! Read 2 bytes from (97+4+1+2)
    read(u,pos=108) record_num
    
    ! Read scale and offset starting at pos 132
    read(u,pos=132) scale, offset
   
    open(newunit=o,file='../CubeOutput/cube-inpoints.data',status='replace',access='sequential')
    pcount = 0
    records: do i=0, record_num-1
        record_loc = offset_to_point_data_value + record_len * i + 1
        
        read(u,pos=record_loc) p
        q = [(p(j)*scale(j)+offset(j),j=1,3)]
        
        if (q .isinside. geo_poly) then
            pcount = pcount + 1
            write(o,'(3f15.6)') q
        end if
    end do records
    close(o)
    close(u)
    write(*,'(i0,a)') pcount,' points inside'
    
    ! Commands to run gnuplot
    open(newunit=u,file='../CubeOutput/cube.gpl',status='replace',access='sequential')
    write(u,'(a)') "splot 'cube-inpoints.data' with points, 'cube.data' with linespoints"
    close(u)
    write(*,'(a/a)') 'Now run','pushd ../CubeOutput && gnuplot -p cube.gpl && popd'
    
    stop
    
contains
    ! Create an instance of the polyhedron, in this case a rotated cube
    subroutine create_geo_poly()
        real(8) :: cube_points(3,8)
        integer :: u
        
        ! 1) Set the points
        cube_points(:,1) = [-27.28046,         37.11775,        -39.03485]
        cube_points(:,2) = [-44.40014,         38.50727,        -28.78860]
        cube_points(:,3) = [-49.63065,         20.24757,        -35.05160]
        cube_points(:,4) = [-32.51096,         18.85805,        -45.29785]
        cube_points(:,5) = [-23.59142,         10.81737,        -29.30445]
        cube_points(:,6) = [-18.36091,         29.07707,        -23.04144]
        cube_points(:,7) = [-35.48060,         30.46659,        -12.79519]
        cube_points(:,8) = [-40.71110,         12.20689,        -19.05819]
        
        ! 2) Create the polyhedron instance
        call geo_poly%create(12,cube_points)
        
        ! 3) Set the triangulation
        ! Face 1: 1,2,3,4
        call geo_poly%add_quad([1,2,3,4])
        
        ! Face 2: 1,2,7,6
        call geo_poly%add_quad([1,2,7,6])
        
        ! Face 3: 2,3,8,7
        call geo_poly%add_quad([2,3,8,7])
        
        ! Face 4: 3,8,5,4
        call geo_poly%add_quad([3,8,5,4])
        
        ! Face 5: 1,4,5,6
        call geo_poly%add_quad([1,4,5,6])
    
        ! Face 6: 5,6,7,8
        call geo_poly%add_quad([5,6,7,8])
        
        ! Write polyhedron data for gnuplot
        open(newunit=u,file='../CubeOutput/cube.data',status='replace',access='sequential')
        write(u,'(dt)') geo_poly
        close(u)
    end subroutine create_geo_poly
    
end program LASProc