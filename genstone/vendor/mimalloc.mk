MIMALLOC_CFLAGS += -Igenstone/vendor/mimalloc/include
MIMALLOC_LFLAGS += -lmimalloc

MIMALLOC_LIB = lib/$(LIB_PREFIX)mimalloc$(DYNAMIC_LIB_SUFFIX)
ifeq ($(BUILD_MODE),DEBUG)
_MIMALLOC_LIB_TAGGED_OUTPUT = lib/$(LIB_PREFIX)mimalloc-secure-debug$(DYNAMIC_LIB_SUFFIX)
else
_MIMALLOC_LIB_TAGGED_OUTPUT = lib/$(LIB_PREFIX)mimalloc-secure$(DYNAMIC_LIB_SUFFIX)
endif
_MIMALLOC_LIB_INTERNAL = genstone/vendor/tmp/mimalloc/$(LIB_PREFIX)mimalloc$(DYNAMIC_LIB_SUFFIX)
_MIMALLOC_LIB_INTERNAL_PATTERN = $(wildcard genstone/vendor/tmp/mimalloc/$(LIB_PREFIX)mimalloc*$(DYNAMIC_LIB_SUFFIX)*)
_MIMALLOC_LIB_INTERNAL_MAKEFILE = genstone/vendor/tmp/mimalloc/Makefile

_MIMALLOC_CMAKE_FLAGS = -DMI_SECURE=ON -DMI_OVERRIDE=OFF
ifeq ($(BUILD_MODE),DEBUG)
_MIMALLOC_CMAKE_FLAGS += -DMI_DEBUG_FULL=ON
endif

genstone/vendor/tmp:
	-mkdir $@

genstone/vendor/tmp/mimalloc: | genstone/vendor/tmp
	-mkdir $@

$(_MIMALLOC_LIB_INTERNAL_MAKEFILE): | genstone/vendor/tmp/mimalloc
	$(CMAKE) $(GLOBAL_CMAKE_MODULE_FLAGS) $(_MIMALLOC_CMAKE_FLAGS) -Bgenstone/vendor/tmp/mimalloc genstone/vendor/mimalloc

$(_MIMALLOC_LIB_INTERNAL): $(_MIMALLOC_LIB_INTERNAL_MAKEFILE)
	$(MAKE) -Cgenstone/vendor/tmp/mimalloc

$(MIMALLOC_LIB): $(_MIMALLOC_LIB_INTERNAL) | lib
	cp -r $(_MIMALLOC_LIB_INTERNAL_PATTERN) lib
# We want a basename mimalloc lib for linkage
	cp -r $(_MIMALLOC_LIB_TAGGED_OUTPUT) $(MIMALLOC_LIB)

build_message_mimalloc:
	@echo "$(SECTION_PREFIX) Mimalloc"
	@echo "$(INFO_PREFIX) The gods of Microsoft hath bestowed their memory magic on us"

mimalloc: build_message_mimalloc $(MIMALLOC_LIB) ### @Vendor builds Microsoft `mimalloc` as a Genstone module

clean_mimalloc:
	-rm -rf genstone/vendor/tmp/mimalloc
	-rm $(wildcard lib/*mimalloc*)

# src/*.c

# -DMI_SECURE=4
# -DMI_SHARED_LIB
# -DMI_SHARED_LIB_EXPORT
# -ftls-model=local-dynamic
# -pthread
# -Iinclude

# -DMI_DEBUG_FULL=3
# -DMI_SHOW_ERRORS=1

#   target_link_libraries(mimalloc PUBLIC ${mi_libraries})
#   target_include_directories(mimalloc PUBLIC
#       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#       $<INSTALL_INTERFACE:${mi_install_incdir}>
#   )
#   if(WIN32)
#     # On windows copy the mimalloc redirection dll too.
#     if(CMAKE_SIZEOF_VOID_P EQUAL 4)
#       set(MIMALLOC_REDIRECT_SUFFIX "32")
#     else()
#       set(MIMALLOC_REDIRECT_SUFFIX "")
#     endif()

#     target_link_libraries(mimalloc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/bin/mimalloc-redirect${MIMALLOC_REDIRECT_SUFFIX}.lib)
#     add_custom_command(TARGET mimalloc POST_BUILD
#       COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/bin/mimalloc-redirect${MIMALLOC_REDIRECT_SUFFIX}.dll" $<TARGET_FILE_DIR:mimalloc>
#       COMMENT "Copy mimalloc-redirect${MIMALLOC_REDIRECT_SUFFIX}.dll to output directory")
#   endif()

#   install(TARGETS mimalloc EXPORT mimalloc DESTINATION ${mi_install_libdir} LIBRARY)  
#   install(EXPORT mimalloc DESTINATION ${mi_install_cmakedir})
# endif()

# # static library
# if (MI_BUILD_STATIC)
#   add_library(mimalloc-static STATIC ${mi_sources})
#   set_property(TARGET mimalloc-static PROPERTY POSITION_INDEPENDENT_CODE ON)
#   target_compile_definitions(mimalloc-static PRIVATE ${mi_defines} MI_STATIC_LIB)
#   target_compile_options(mimalloc-static PRIVATE ${mi_cflags})
#   target_link_libraries(mimalloc-static PUBLIC ${mi_libraries})
#   target_include_directories(mimalloc-static PUBLIC
#       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#       $<INSTALL_INTERFACE:${mi_install_incdir}>
#   )
#   if(WIN32)
#     # When building both static and shared libraries on Windows, a static library should use a
#     # different output name to avoid the conflict with the import library of a shared one.
#     string(REPLACE "mimalloc" "mimalloc-static" mi_output_name ${mi_basename})
#     set_target_properties(mimalloc-static PROPERTIES OUTPUT_NAME ${mi_output_name})
#   else()
#     set_target_properties(mimalloc-static PROPERTIES OUTPUT_NAME ${mi_basename})
#   endif()

#   install(TARGETS mimalloc-static EXPORT mimalloc DESTINATION ${mi_install_libdir} LIBRARY)
# endif()

# # install include files
# install(FILES include/mimalloc.h DESTINATION ${mi_install_incdir})
# install(FILES include/mimalloc-override.h DESTINATION ${mi_install_incdir})
# install(FILES include/mimalloc-new-delete.h DESTINATION ${mi_install_incdir})
# install(FILES cmake/mimalloc-config.cmake DESTINATION ${mi_install_cmakedir})
# install(FILES cmake/mimalloc-config-version.cmake DESTINATION ${mi_install_cmakedir})

# if(NOT WIN32 AND MI_BUILD_SHARED AND NOT MI_INSTALL_TOPLEVEL)
#   # install a symlink in the /usr/local/lib to the versioned library
#   # note: use delayed prefix expansion as \${CMAKE_INSTALL_PREFIX}
#   set(mi_symlink "${CMAKE_SHARED_MODULE_PREFIX}${mi_basename}${CMAKE_SHARED_LIBRARY_SUFFIX}")
#   set(mi_soname "mimalloc-${mi_version}/${mi_symlink}.${mi_version}")
#   install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${mi_soname} ${mi_symlink} WORKING_DIRECTORY \${CMAKE_INSTALL_PREFIX}/lib)")
#   install(CODE "MESSAGE(\"-- Symbolic link: \${CMAKE_INSTALL_PREFIX}/lib/${mi_symlink} -> ${mi_soname}\")")
#   install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${mi_soname} ${mi_symlink}.${mi_version} WORKING_DIRECTORY \${CMAKE_INSTALL_PREFIX}/lib)")
#   install(CODE "MESSAGE(\"-- Symbolic link: \${CMAKE_INSTALL_PREFIX}/lib/${mi_symlink}.${mi_version} -> ${mi_soname}\")")
# endif()

# # single object file for more predictable static overriding
# if (MI_BUILD_OBJECT)
#   add_library(mimalloc-obj OBJECT src/static.c)
#   set_property(TARGET mimalloc-obj PROPERTY POSITION_INDEPENDENT_CODE ON)
#   target_compile_definitions(mimalloc-obj PRIVATE ${mi_defines})
#   target_compile_options(mimalloc-obj PRIVATE ${mi_cflags})
#   target_include_directories(mimalloc-obj PUBLIC
#       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#       $<INSTALL_INTERFACE:${mi_install_incdir}>
#   )

#   # the following seems to lead to cmake warnings/errors on some systems, disable for now :-(
#   # install(TARGETS mimalloc-obj EXPORT mimalloc DESTINATION ${mi_install_libdir})

#   # the FILES expression can also be: $<TARGET_OBJECTS:mimalloc-obj>
#   # but that fails cmake versions less than 3.10 so we leave it as is for now
#   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/mimalloc-obj.dir/src/static.c${CMAKE_C_OUTPUT_EXTENSION}
#           DESTINATION ${mi_install_libdir}
#           RENAME ${mi_basename}${CMAKE_C_OUTPUT_EXTENSION} )
# endif()

# # -----------------------------------------------------------------------------
# # API surface testing
# # -----------------------------------------------------------------------------

# if (MI_BUILD_TESTS)
#   add_executable(mimalloc-test-api test/test-api.c)
#   target_compile_definitions(mimalloc-test-api PRIVATE ${mi_defines})
#   target_compile_options(mimalloc-test-api PRIVATE ${mi_cflags})
#   target_include_directories(mimalloc-test-api PRIVATE include)
#   target_link_libraries(mimalloc-test-api PRIVATE mimalloc ${mi_libraries})

#   add_executable(mimalloc-test-stress test/test-stress.c)
#   target_compile_definitions(mimalloc-test-stress PRIVATE ${mi_defines})
#   target_compile_options(mimalloc-test-stress PRIVATE ${mi_cflags})
#   target_include_directories(mimalloc-test-stress PRIVATE include)
#   target_link_libraries(mimalloc-test-stress PRIVATE mimalloc ${mi_libraries})

#   enable_testing()
#   add_test(test_api, mimalloc-test-api)
#   add_test(test_stress, mimalloc-test-stress)
# endif()

# # -----------------------------------------------------------------------------
# # Set override properties
# # -----------------------------------------------------------------------------
# if (MI_OVERRIDE)
#   if (MI_BUILD_SHARED)
#     target_compile_definitions(mimalloc PRIVATE MI_MALLOC_OVERRIDE)
#   endif()
#   if(NOT WIN32)
#     # It is only possible to override malloc on Windows when building as a DLL.
#     if (MI_BUILD_STATIC)
#       target_compile_definitions(mimalloc-static PRIVATE MI_MALLOC_OVERRIDE)
#     endif()
#     if (MI_BUILD_OBJECT)
#       target_compile_definitions(mimalloc-obj PRIVATE MI_MALLOC_OVERRIDE)
#     endif()
#   endif()
# endif()
