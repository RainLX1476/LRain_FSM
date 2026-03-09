#测试源代码放这里
set(TEST_SOURCES
	test1.c
)

foreach(src ${TEST_SOURCES})
	get_filename_component(target_name ${src} NAME_WE)
	add_executable(${target_name} ${src})
	target_link_libraries(${target_name} PRIVATE LRain_RB)
	add_test(NAME ${target_name} COMMAND ${target_name})
endforeach()