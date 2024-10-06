include(${CMAKE_SOURCE_DIR}/cmake/pr_common.cmake)

set(PROJ_NAME util_unicode)
pr_add_library(${PROJ_NAME} SHARED)

pr_add_external_dependency(${PROJ_NAME} icu HEADER_ONLY)
pr_add_external_dependency(${PROJ_NAME} icu_icuuc LIBRARY)

pr_init_module(${PROJ_NAME})

pr_add_compile_definitions(
	${PROJ_NAME}
		-DUUNIC_DLL
)

pr_finalize(${PROJ_NAME})
