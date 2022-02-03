cmake_minimum_required(VERSION 3.20)
message(STATUS "+++ Start CMake ${CMAKE_CURRENT_SOURCE_DIR}!")

set(_COMPLETE_INSTALL OFF)

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

# the internal used directories:
set(_INCLUDE_OUTPUT ${OUTPUT}/include)
set(_DATA_INPUT ${PROJECTGROUP_SOURCE_DIR}/Data)
set(_DATA_OUTPUT ${OUTPUT}/Data)
# set(_ICONS_OUTPUT ${_DATA_OUTPUT}/icons-test)
set(_ICONS_OUTPUT ${_DATA_OUTPUT}/icons)
set(_GRAPHICS_OUTPUT ${_DATA_OUTPUT}/graphics)

include(CMakeSource.cmake)

set (BIN_FILES
   AUTHORS.gz
   COPYING.gz
   NEWS.txt.gz
   Data/other/egm96s.dem
)

file(GLOB ICON_FILES        "icons/*.svg")
# file(GLOB GRAPHIC_FILES     "graphics/*.svg")
set(GRAPHIC_FILES )
# list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/dialog_title.svg")
# list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/launcher.svg")
list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/logo.svg")
list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/progress_border.svg")
list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/title.svg")


## if(MSVC) ## escape for '$' character is different! Why?
##     set (PERCENT_CHAR "%%")
##     set (DOLLAR_CHAR "\$")
## else()
##     set (PERCENT_CHAR "\%")
##     set (DOLLAR_CHAR "$$")
## endif()

if("${PERCENT_CHAR}" STREQUAL "")
    message(FATAL_ERROR PERCENT_CHAR not used: Stop!)
endif() 


message(STATUS "Zip-App: '${ZIP_APP}'")

set(SCRIPT_FILES )
set(C_FILES)  # Reset to empty...
if(ON) # MSVC)  # mit minGW geht das nicht??
  if(WIN32)  # bei Linux klappt etwas nich!!!
    foreach(bin_file ${BIN_FILES})
       get_filename_component(c_file ${bin_file} NAME)
       set(c_file ${OUTPUT_FOLDER}/Data/${c_file}.c)
       # set(c_file ${_DATA_OUTPUT}/${c_file}.c)
       # simular add_custom_command - but not the same ;-(
       if(${bin_file} MATCHES "\.gz$")
          string(REGEX REPLACE  "\.gz$"  "" txt_file ${bin_file})
          message(STATUS "MATCHES!!!! ${bin_file}  --- ${txt_file}")
          set(zip_file  "${OUTPUT_FOLDER}/Data/temp/${bin_file}")
          # set(zip_file  "${_DATA_OUTPUT}/temp/${bin_file}")
          set (bin_file "${zip_file}")
          message(STATUS "OUTPUT: ${bin_file} --> ${c_file}")
          add_custom_command(OUTPUT ${c_file}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${_DATA_OUTPUT}
    #       if (${bin_file} MATCHES "\.gz$")
                COMMAND echo "${ZIP_APP} a ${zip_file} ${txt_file}"
                COMMAND ${ZIP_APP} a ${zip_file}   ${txt_file}
    #       endif()
           COMMAND echo "${PYTHON_APP} tools/python/bin2c.py  ${bin_file}  ${_DATA_OUTPUT}"
           COMMAND ${PYTHON_APP} tools/python/bin2c.py  ${bin_file}  ${_DATA_OUTPUT}
           WORKING_DIRECTORY  ${PROJECTGROUP_SOURCE_DIR}
           COMMENT TextFile: ${bin_file}, 
           DEPENDS ${OUTPUT_FOLDER}/Data
        )
        else()
          message(STATUS "UNMATCHES!!!! ${bin_file}  --- ${txt_file}")
          message(STATUS "OUTPUT: ${bin_file} --> ${c_file}")
          add_custom_command(OUTPUT ${c_file}
             COMMAND echo "${PYTHON_APP} tools/python/bin2c.py  ${bin_file}  ${_DATA_OUTPUT}"
             COMMAND ${CMAKE_COMMAND} -E make_directory ${_DATA_OUTPUT}
             COMMAND ${PYTHON_APP} tools/python/bin2c.py  ${bin_file}  ${_DATA_OUTPUT}
             WORKING_DIRECTORY  ${PROJECTGROUP_SOURCE_DIR}
             COMMENT TextFile: ${bin_file}, 
             DEPENDS ${OUTPUT_FOLDER}/Data
          )
        endif()
        list(APPEND C_FILES ${c_file})
    endforeach()
    endif(WIN32)
    
   message(STATUS "Image-App: '${BMP_CONVERT_APP}' ")
    
    # set(XCSOAR_VERSION "7.26+x")
    configure_file(${_DATA_INPUT}/graphics/title.svg ${OUTPUT_FOLDER}/Data/temp/title.svg)
    # message(STATUS "configure_file(${_DATA_INPUT}/graphics/title.svg ==>  ${OUTPUT_FOLDER}/Data/temp/title.svg)")
    # message(FATAL_ERROR Stop!)
    
    if (_COMPLETE_INSTALL)
        set(GRAPHIC_BITMAPS
           ${_GRAPHICS_OUTPUT}/logo_160.bmp
           ${_GRAPHICS_OUTPUT}/logo_80.bmp
           ${_GRAPHICS_OUTPUT}/launcher_640.bmp
           ${_GRAPHICS_OUTPUT}/launcher_640_1.bmp
           ${_GRAPHICS_OUTPUT}/launcher_640_2.bmp
           ${_GRAPHICS_OUTPUT}/title_320.bmp
           ${_GRAPHICS_OUTPUT}/title_110.bmp
        )
    #    add_custom_command(OUTPUT GraphicBitmaps  # ${GRAPHIC_BITMAPS}    # "${_GRAPHICS_OUTPUT}/title_320.bmp"

        add_custom_command(OUTPUT ${GRAPHIC_BITMAPS}
               COMMENT "Convert graphics!"
               COMMAND ${CMAKE_COMMAND} -E make_directory ${_GRAPHICS_OUTPUT}
               COMMAND ${BMP_CONVERT_APP} -size 160 Data/graphics/logo.svg bmp3:${_GRAPHICS_OUTPUT}/logo_160.bmp
               COMMAND ${BMP_CONVERT_APP}  -size 80 Data/graphics/logo.svg bmp3:${_GRAPHICS_OUTPUT}/logo_80.bmp
               COMMAND ${BMP_CONVERT_APP}  -background white -layers flatten +matte +dither -compress none -type optimize -colors 256  Data/graphics/launcher.svg bmp3:${_GRAPHICS_OUTPUT}/launcher_640.bmp
               COMMAND ${BMP_CONVERT_APP}  -crop "50${PERCENT_CHAR}x100${PERCENT_CHAR}" -scene 1  ${_GRAPHICS_OUTPUT}/launcher_640.bmp "bmp3:${_GRAPHICS_OUTPUT}/launcher_640_${PERCENT_CHAR}d.bmp"
               COMMAND ${BMP_CONVERT_APP}  Data/graphics/progress_border.svg bmp3:${_GRAPHICS_OUTPUT}/progress_border.bmp
               COMMAND ${BMP_CONVERT_APP}  -size 110x110 ${_DATA_OUTPUT}/temp/title.svg bmp3:${_GRAPHICS_OUTPUT}/title_110.bmp 
               COMMAND ${BMP_CONVERT_APP}  -size 320x320 ${_DATA_OUTPUT}/temp/title.svg bmp3:${_GRAPHICS_OUTPUT}/title_320.bmp
               
               # COMMAND ${BMP_CONVERT_APP}  -size 320 -background white -layers flatten +matte +dither -compress none -type optimize -colors 256  Data/graphics/title.svg bmp3:${_GRAPHICS_OUTPUT}/title_320.bmp
               # WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
               DEPENDS ${_DATA_INPUT}/graphics/logo.svg
                       ${_DATA_INPUT}/graphics/progress_border.svg
                       ${_DATA_INPUT}/graphics/launcher.svg
                       ${OUTPUT_FOLDER}/Data/temp/title.svg
               WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
        )
        endif()
    
    set(ICON_BITMAPS )

# # # macro(testm svg_file)
# # #             string(REPLACE ".svg" ".bmp" bmp_file ${icon_file2})
# # #             string(REPLACE ".svg" ".png" png_file ${icon_file2})
# # #             string(REPLACE ".svg" "_160.bmp" bmp160_file ${icon_file2})
# # #             string(REPLACE ".svg" "_160.png" png160_file ${icon_file2})
# # #             # string(REPLACE ".svg" "_HD.png" png_hd_file ${icon_file2})
# # #             string(REPLACE ".svg" "_alpha.png" png_file_alpha ${icon_file2})
# # #             string(REPLACE ".svg" "_rgb.png" png_file_rgb ${icon_file2})
# # # 
# # #                COMMAND ${INKSCAPE_APP} ${_DATA_INPUT}/icons/${icon_file2} --export-dpi=96 --export-type=PNG --export-overwrite --export-filename="${_ICONS_OUTPUT}/${png_file}"
# # # 
# # #                COMMAND ${BMP_CONVERT_APP} ${_ICONS_OUTPUT}/${png_file} -alpha Extract -colors 8 ${_ICONS_OUTPUT}/${png_file_alpha}
# # #                # "-alpha Off" == "+matte"
# # #                COMMAND ${BMP_CONVERT_APP} ${_ICONS_OUTPUT}/${png_file} -background gray -flatten -alpha Off +dither -colors 64  ${_ICONS_OUTPUT}/${png_file_rgb}
# # # 
# # #                COMMAND ${BMP_MONTAGE_APP} -tile 2x1 -geometry +0+0 ${_ICONS_OUTPUT}/${png_file_alpha} ${_ICONS_OUTPUT}/${png_file_rgb} -depth 8 ${_ICONS_OUTPUT}/${bmp_file}
# # #                COMMAND ${CMAKE_COMMAND} -E remove -f ${_ICONS_OUTPUT}/${png_file_alpha}
# # #                COMMAND ${CMAKE_COMMAND} -E remove -f ${_ICONS_OUTPUT}/${png_file_rgb}
# # # endmacro()

    
#    $(SVG_NOALIAS_ICONS): $(DATA)/icons/%.svg: build/svg_preprocess.xsl Data/icons/%.svg | $(DATA)/icons/dirstamp
#	@$(NQ)echo "  XSLT    $@"
#	$(Q)xsltproc --nonet --stringparam DisableAA_Select "MASK_NOAA_" --output $@ $^

    file(GLOB ICON_FILES  ${_DATA_INPUT}/icons/*.svg)
##    file(GLOB ICON_FILES  ${_DATA_INPUT}/icons/alt_land*.svg) # for test it only with few icons!
    foreach(icon_file ${ICON_FILES})
            string(REPLACE "${_DATA_INPUT}/icons" ${_ICONS_OUTPUT} icon_file2 ${icon_file})
#            string(REPLACE "${_DATA_INPUT}/icons" ${_ICONS_OUTPUT} svg_file ${icon_file})
            # set(icon_file2 ${_ICONS_OUTPUT}/${icon_file2})
            string(REPLACE ".svg" ".bmp" bmp_file ${icon_file2})
            string(REPLACE ".svg" ".png" png_file ${icon_file2})
            string(REPLACE ".svg" "_160.bmp" bmp160_file ${icon_file2})
            string(REPLACE ".svg" "_160.png" png160_file ${icon_file2})
            # string(REPLACE ".svg" "_HD.png" png_hd_file ${icon_file2})
            string(REPLACE ".svg" "_alpha.png" png_file_alpha ${icon_file2})
            string(REPLACE ".svg" "_rgb.png" png_file_rgb ${icon_file2})
            # string(REPLACE ".svg" "_160_alpha.png" png160_file_alpha ${icon_file2})
            # string(REPLACE ".svg" "_160_rgb.png" png160_file_rgb ${icon_file2})
            set(BMP_CONVERT_PARAM +dither -compress none -type optimize -colors 256 )
     #       if (MSVC) # in other targets not possible to add this files to the project???
if (_COMPLETE_INSTALL)
            list(APPEND ICON_BITMAPS "${_ICONS_OUTPUT}/${bmp_file}")
     #       endif()
            add_custom_command(OUTPUT "${_ICONS_OUTPUT}/${bmp_file}"
               COMMENT File aus 'icons': ${bmp_file}
               COMMAND ${CMAKE_COMMAND} -E make_directory ${_ICONS_OUTPUT}
               COMMAND ${CMAKE_COMMAND} -E copy ${icon_file} ${icon_file2}
               # ??? COMMAND ${XSLTPROC_APP} --nonet --stringparam DisableAA_Select "MASK_NOAA_" --output  ${PROJECTGROUP_SOURCE_DIR}/build/svg_preprocess.xsl  ${icon_file}  # COMMAND ${INKSCAPE_APP} ${icon_file} --export-dpi=96 --export-type="PNG" --export-overwrite --export-background=0xffffff --export-background-opacity=0.0 --export-filename="${_ICONS_OUTPUT}/${png_hd_file}"
               COMMAND ${INKSCAPE_APP} ${icon_file2} --export-dpi=124 --export-type=PNG --export-overwrite --export-filename="${png_file}"

               COMMAND ${BMP_CONVERT_APP} ${png_file} -alpha Extract -colors 8 ${png_file_alpha}
               # "-alpha Off" == "+matte"
               COMMAND ${BMP_CONVERT_APP} ${png_file} -background gray -flatten -alpha Off +dither -colors 64  ${png_file_rgb}

               COMMAND ${BMP_MONTAGE_APP} -tile 2x1 -geometry +0+0 ${png_file_alpha} ${png_file_rgb} -depth 8 ${bmp_file}
#               # COMMAND ${BMP_CONVERT_APP} ${icon_file} ${BMP_CONVERT_PARAM} bmp3:${bmp_file}
               COMMAND ${CMAKE_COMMAND} -E remove -f ${png_file_alpha}
               COMMAND ${CMAKE_COMMAND} -E remove -f ${png_file_rgb}
# # #              testm(${icon_file2})                        

               COMMAND ${INKSCAPE_APP} ${icon_file2} --export-dpi=200 --export-type=PNG --export-overwrite --export-filename=${png160_file}
               COMMAND ${BMP_CONVERT_APP} ${png160_file} -alpha Extract -colors 8 ${png_file_alpha}
               # "-alpha Off" == "+matte"
               COMMAND ${BMP_CONVERT_APP} ${png160_file} -background gray -flatten -alpha Off +dither -colors 64  ${png_file_rgb}
               COMMAND ${BMP_MONTAGE_APP} -tile 2x1 -geometry +0+0 ${png_file_alpha} ${png_file_rgb} -depth 8 ${bmp160_file}
               
               COMMAND ${CMAKE_COMMAND} -E remove -f ${png_file_alpha}
               COMMAND ${CMAKE_COMMAND} -E remove -f ${png_file_rgb}

               MAIN_DEPENDENCY ${icon_file}
               DEPENDS ${icon_file}
               WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
            )
endif()
        endforeach()
       
    # pure cmd line in console with output in console (bypass: > resource.h):
    # perl -ne "print \"#define $1 $2\n\" if /^MAKE_RESOURCE\((\w+), (\d+)\);/;" [path-to-xcsoar]/XCSoar/src/Resources.hpp
    
## moved to WinMSVC and so on!
if("${DOLLAR_CHAR}" STREQUAL "")
    message(FATAL_ERROR DOLLAR_CHAR not used: Stop!)
endif() 
    set (PERL_CMD ${PERL_APP} -ne \"print \\\"\#define ${DOLLAR_CHAR}1 ${DOLLAR_CHAR}2\\n\\\" )
    string(APPEND PERL_CMD "if /^MAKE_RESOURCE\\\(\(\\w+\), \(\\d+\)\\\)") 
    string(APPEND PERL_CMD "\;/\;\"") 
    
    set(PERL_INPUT ${PROJECTGROUP_SOURCE_DIR}/src/Resources.hpp)
    set(RESOURCE_OUT ${_INCLUDE_OUTPUT}/resource.h)
    message(STATUS PERL_CMD = "${PERL_CMD} ${PERL_INPUT} >${RESOURCE_OUT}")
    # message(FATAL_ERROR Stop!)
    
    
    add_custom_command( # TARGET ${TARGET_NAME} PRE_BUILD
               OUTPUT ${RESOURCE_OUT}
               COMMAND ${CMAKE_COMMAND} -E make_directory ${_INCLUDE_OUTPUT} 
               COMMAND echo ${PERL_CMD} ${PERL_INPUT} -- ${RESOURCE_OUT}
               COMMAND      ${PERL_CMD} ${PERL_INPUT}  >${RESOURCE_OUT}
               # COMMAND      ${PERL_CMD} ${PERL_INPUT}
               ## >${RESOURCE_OUT}
               # COMMAND      ${PERL_CMD} ${PERL_INPUT} >res2.h
               # DEPENDS src/Resources.hpp # ${_INCLUDE_OUTPUT}/dirstamp
               # DEPENDS ${PERL_INPUT} 
               DEPENDS ${PROJECTGROUP_SOURCE_DIR}/src/Resources.hpp
               WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
               COMMENT ==== 
        )
    
    #  if(1)
    if (ON) # MSVC) # in other targets not possible to add this files to the project???
       source_group("Bitmaps" FILES  ${ICON_BITMAPS}  ${GRAPHIC_BITMAPS})
    endif()
    source_group("Icons" FILES  ${ICON_FILES})
    source_group("Graphics" FILES  ${GRAPHIC_FILES})
    
    ## add_custom_command(TARGET ${TARGET_NAME} PRE_LINK
    set(INPUTEVENTS_CPP
               ${_INCLUDE_OUTPUT}/InputEvents_Char2GCE.cpp
               # ${_INCLUDE_OUTPUT}/InputEvents_Char2GCE.cpp
               ${_INCLUDE_OUTPUT}/InputEvents_Char2NE.cpp
               ${_INCLUDE_OUTPUT}/InputEvents_Text2Event.cpp
               ${_INCLUDE_OUTPUT}/InputEvents_Text2GCE.cpp
               ${_INCLUDE_OUTPUT}/InputEvents_Text2NE.cpp
               ${_INCLUDE_OUTPUT}/InputEvents_default.cpp
    )
    # add_custom_command(OUTPUT  ${_INCLUDE_OUTPUT}/InputEvents_Char2GCE.cpp  # ${INPUTEVENTS_CPP}
    add_custom_command(OUTPUT  InputFiles #  ${INPUTEVENTS_CPP}
           COMMENT Create InputEvents_*.cpp!
           COMMAND ${CMAKE_COMMAND} -E make_directory ${_INCLUDE_OUTPUT} 
           COMMAND perl tools/Char2GCE.pl    src/Input/InputQueue.hpp >${_INCLUDE_OUTPUT}/InputEvents_Char2GCE.cpp
           COMMAND perl tools/Char2NE.pl     src/Input/InputQueue.hpp >${_INCLUDE_OUTPUT}/InputEvents_Char2NE.cpp
           COMMAND perl tools/Text2Event.pl  src/Input/InputEvents.hpp >${_INCLUDE_OUTPUT}/InputEvents_Text2Event.cpp
           COMMAND perl tools/Text2GCE.pl    src/Input/InputQueue.hpp >${_INCLUDE_OUTPUT}/InputEvents_Text2GCE.cpp
           COMMAND perl tools/Text2NE.pl     src/Input/InputQueue.hpp >${_INCLUDE_OUTPUT}/InputEvents_Text2NE.cpp
           COMMAND perl tools/xci2cpp.pl     Data/Input/default.xci  >${_INCLUDE_OUTPUT}/InputEvents_default.cpp
           DEPENDS ../src/Input/InputQueue.hpp 
                   ../src/Input/InputEvents.hpp
                   ${_DATA_INPUT}/Input/default.xci
           WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
    )
    
    # add_custom_command( OUTPUT ${OUTPUT_FOLDER}/include/Status_defaults.cpp
    add_custom_command( OUTPUT DefaultStatus
    # add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
       # COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECTGROUP_SOURCE_DIR}/${_INCLUDE_OUTPUT}
       COMMAND ${CMAKE_COMMAND} -E make_directory ${_INCLUDE_OUTPUT}
       COMMAND perl tools/xcs2cpp.pl    Data/Status/default.xcs >${OUTPUT_FOLDER}/include/Status_defaults.cpp
       DEPENDS ${_DATA_INPUT}/Status/default.xcs
       WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
    )
    
    list(APPEND INPUTEVENTS_CPP ${OUTPUT_FOLDER}/include/Status_defaults.cpp)
    
    # list(APPEND C_FILES ${_INCLUDE_OUTPUT}/Status_defaults.cpp)

    list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/logo.svg")
    list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/progress_border.svg")
    list(APPEND GRAPHIC_FILES   "${_DATA_INPUT}/graphics/title.svg")
    
    include_directories(${_DATA_INPUT})
endif()

add_library(${TARGET_NAME}  ${XCSOAR_LIB_TYPE}
   ${C_FILES}
   ${HEADER_FILES}
   ${RESOURCE_OUT}
   # GraphicBitmaps  # the role...
   ${GRAPHIC_BITMAPS}
   ${GRAPHIC_FILES}
   ${ICON_BITMAPS}
   ${ICON_FILES}
   ${PROJECTGROUP_SOURCE_DIR}/tools/Char2GCE.pl
   # ${INPUTEVENTS_CPP}
   InputFiles    # role for input files
   DefaultStatus # role for status defaults
   ${_INCLUDE_OUTPUT}/resource.h
)

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE C)

# target_include_directories(Data ${_DATA_INPUT})

## 2022-09-06 add_custom_command(OUTPUT ${OUTPUT_FOLDER}/Data
## 2022-09-06 # add_custom_command(TARGET ${TARGET_NAME} PRE_LINK
## 2022-09-06 # add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
## 2022-09-06        COMMENT "Make dir '${_DATA_OUTPUT}/icons'"
## 2022-09-06 #       COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_FOLDER}/Data
## 2022-09-06        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_FOLDER}/include
## 2022-09-06        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_FOLDER}/Data
## 2022-09-06        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_FOLDER}/Data/graphics
## 2022-09-06        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_FOLDER}/Data/icons
## 2022-09-06        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_FOLDER}/Data/temp
## 2022-09-06        WORKING_DIRECTORY ${PROJECTGROUP_SOURCE_DIR}
## 2022-09-06 )
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Data
                                                #  ENABLE_EXPORTS TRUE
)

add_dependencies(${TARGET_NAME} util)

