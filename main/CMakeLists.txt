idf_component_register(SRCS 
							"Font_Manager.cpp" 
                            "fonts.c"
                    INCLUDE_DIRS 
                    		"include"
                            "fonts"
                    )
                    
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-int-in-bool-context)
