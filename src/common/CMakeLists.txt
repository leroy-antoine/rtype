add_library(rtype_common INTERFACE)
add_library(rtype::common ALIAS rtype_common)
target_include_directories(rtype_common INTERFACE include)
