#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PDFHummus::Zlib" for configuration "Debug"
set_property(TARGET PDFHummus::Zlib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PDFHummus::Zlib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/Zlib.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS PDFHummus::Zlib )
list(APPEND _IMPORT_CHECK_FILES_FOR_PDFHummus::Zlib "${_IMPORT_PREFIX}/lib/Zlib.lib" )

# Import target "PDFHummus::FreeType" for configuration "Debug"
set_property(TARGET PDFHummus::FreeType APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PDFHummus::FreeType PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/FreeType.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS PDFHummus::FreeType )
list(APPEND _IMPORT_CHECK_FILES_FOR_PDFHummus::FreeType "${_IMPORT_PREFIX}/lib/FreeType.lib" )

# Import target "PDFHummus::LibAesgm" for configuration "Debug"
set_property(TARGET PDFHummus::LibAesgm APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PDFHummus::LibAesgm PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/LibAesgm.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS PDFHummus::LibAesgm )
list(APPEND _IMPORT_CHECK_FILES_FOR_PDFHummus::LibAesgm "${_IMPORT_PREFIX}/lib/LibAesgm.lib" )

# Import target "PDFHummus::LibJpeg" for configuration "Debug"
set_property(TARGET PDFHummus::LibJpeg APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PDFHummus::LibJpeg PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/LibJpeg.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS PDFHummus::LibJpeg )
list(APPEND _IMPORT_CHECK_FILES_FOR_PDFHummus::LibJpeg "${_IMPORT_PREFIX}/lib/LibJpeg.lib" )

# Import target "PDFHummus::LibTiff" for configuration "Debug"
set_property(TARGET PDFHummus::LibTiff APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PDFHummus::LibTiff PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C;CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/LibTiff.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS PDFHummus::LibTiff )
list(APPEND _IMPORT_CHECK_FILES_FOR_PDFHummus::LibTiff "${_IMPORT_PREFIX}/lib/LibTiff.lib" )

# Import target "PDFHummus::LibPng" for configuration "Debug"
set_property(TARGET PDFHummus::LibPng APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PDFHummus::LibPng PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/LibPng.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS PDFHummus::LibPng )
list(APPEND _IMPORT_CHECK_FILES_FOR_PDFHummus::LibPng "${_IMPORT_PREFIX}/lib/LibPng.lib" )

# Import target "PDFHummus::PDFWriter" for configuration "Debug"
set_property(TARGET PDFHummus::PDFWriter APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PDFHummus::PDFWriter PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/PDFWriter.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS PDFHummus::PDFWriter )
list(APPEND _IMPORT_CHECK_FILES_FOR_PDFHummus::PDFWriter "${_IMPORT_PREFIX}/lib/PDFWriter.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
