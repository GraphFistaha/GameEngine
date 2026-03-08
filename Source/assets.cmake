FUNCTION(TARGET_ASSETS_DIRECTORY)
	set(oneValueArgs TARGET DIRECTORY RESULT_DIRECTORY)
	set(multiValueArgs "")
	cmake_parse_arguments(PARSED_ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	set(RESULT_FOLDER "${CMAKE_INSTALL_PREFIX}/${PARSED_ARG_RESULT_DIRECTORY}")

	# add archive step
	#add_custom_command(
	#	TARGET ${PARSED_ARG_TARGET}
	#	POST_BUILD
	#	COMMAND ...
	#	ARGS ...
	#)

	install(DIRECTORY ${PARSED_ARG_DIRECTORY}/ DESTINATION ${RESULT_FOLDER})
ENDFUNCTION()