# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\production\\default-production.cmf"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\production\\default-production.hex"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\production\\default-production.hxl"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\production\\default-production.mum"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\production\\default-production.o"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\production\\default-production.sdb"
  "D:\\yiming\\00work\\FR761_V05_test2.X\\out\\FR761\\production\\default-production.sym"
  )
endif()
