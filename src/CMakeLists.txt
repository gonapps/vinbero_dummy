add_library(tucube_dummy MODULE tucube_dummy.c)
set_target_properties(tucube_dummy PROPERTIES PREFIX "")
install(TARGETS tucube_dummy DESTINATION lib/tucube)
