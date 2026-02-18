cmake_minimum_required(VERSION 3.10)
project(luajit)

add_ci_program(luajit.elf
	state.hpp state.cpp
	state/gd.hpp
	state/gdarray.hpp state/gdarray.cpp
	state/gdcallable.hpp state/gdcallable.cpp
	state/gdcolor.hpp state/gdcolor.cpp
	state/gdobject.hpp state/gdobject.cpp
	state/gdquaternion.hpp state/gdquaternion.cpp
	state/gdrect2.hpp state/gdrect2.cpp
	state/gdrect2i.hpp state/gdrect2i.cpp
	state/gdtransform2d.hpp state/gdtransform2d.cpp
	state/gdtransform3d.hpp state/gdtransform3d.cpp
	state/gdvector2.hpp state/gdvector2.cpp
	state/gdvector2i.hpp state/gdvector2i.cpp
	state/gdvector3.hpp state/gdvector3.cpp
	state/gdvector3i.hpp state/gdvector3i.cpp
	state/gdvector4.hpp state/gdvector4.cpp
	state/gdvector4i.hpp state/gdvector4i.cpp
	state/mode.hpp
	luamod.hpp luamod.cpp
	utils.hpp utils.cpp
	entry.hpp entry.cpp
)

if (ZIG_COMPILER)
	target_link_libraries(luajit.elf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libluajit/zig-libluajit.a)
else()
	target_link_libraries(luajit.elf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libluajit/libluajit.a)
endif()

target_include_directories(luajit.elf PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/ext/LuaJIT/src
	${CMAKE_SOURCE_DIR}/ext/sol2/include
)
