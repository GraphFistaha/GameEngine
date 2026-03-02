set(this_target "Launcher")

add_executable(${this_target})

target_sources(${this_target}
PRIVATE
	"main.cpp"
)

target_link_libraries(${this_target}
PRIVATE
	GameFramework
)

target_compile_definitions(${this_target} 
	PRIVATE
		-DLAUNCHER_BUILD
)


target_include_directories(${this_target} 
PUBLIC
	"${SOURCE_DIR}/Launcher"
)

install(TARGETS ${this_target}
  EXPORT ${this_target}
  RUNTIME DESTINATION Bin
  LIBRARY DESTINATION Lib
  ARCHIVE DESTINATION Lib
  INCLUDES DESTINATION include
)