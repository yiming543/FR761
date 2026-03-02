# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\default.cmf"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\default.hex"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\default.hxl"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\default.mum"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\default.o"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\default.sdb"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\default.sym"
  )
endif()
