set_extra_dirs_lib(DPP dpp)
find_library(DPP_LIBRARY
  NAMES "dpp"
  HINTS ${HINTS_DPP_LIBDIR}
  PATHS ${PATHS_DPP_LIBDIR}
  ${CROSSCOMPILING_NO_CMAKE_SYSTEM_PATH})

set(DPP_COPY_FILES)

#if(DPP_FOUND)
set(DPP_COPY_FILES 
	"${EXTRA_DPP_LIBDIR}/dpp.dll" 
	"${EXTRA_DPP_LIBDIR}/libsodium.dll" 
	"${EXTRA_DPP_LIBDIR}/opus.dll" 
	"${EXTRA_DPP_LIBDIR}/zlib1.dll")
set(DPP_LIBRARIES ${DPP_LIBRARY})
#endif()
