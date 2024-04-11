set(includedirs
  .
  )

set( srcs
  main.cpp
  )

idf_component_register(SRCS ${srcs} 
                  INCLUDE_DIRS ${includedirs} 
                  REQUIRES BeeNeXT)
