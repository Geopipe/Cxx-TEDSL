project(libCxxTEDSL)

add_library(CxxTEDSL SHARED
	Cxx-TEDSL.cpp
	../include/Cxx-TEDSL/Cxx-TEDSL.hpp
	../include/Cxx-TEDSL/Cxx-TEDSL-Private.hpp
	../include/Cxx-TEDSL/Core.hpp)
target_link_libraries(CxxTEDSL fmt)
