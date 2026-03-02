set(this_target "SimpleGame")

add_library(${this_target} SHARED
	"dllmain.cpp"
)

target_link_libraries(${this_target}
PUBLIC
	GameFramework
)

install(TARGETS ${this_target}
  EXPORT ${this_target}
  RUNTIME DESTINATION Bin/Examples
  LIBRARY DESTINATION Lib/Examples
  ARCHIVE DESTINATION Lib/Examples
  INCLUDES DESTINATION include/Examples
)