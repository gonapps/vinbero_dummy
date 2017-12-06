add_library(tucube_dummy MODULE tucube_dummy.c)
set_target_properties(tucube_dummy PROPERTIES PREFIX "")
set_target_properties(tucube_dummy PROPERTIES PUBLIC_HEADER "tucube_IDummy.h")
install(TARGETS tucube_dummy DESTINATION lib/tucube)
