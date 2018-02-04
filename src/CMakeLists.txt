add_library(vinbero_dummy MODULE vinbero_dummy.c)
set_target_properties(vinbero_dummy PROPERTIES PREFIX "")
install(TARGETS vinbero_dummy DESTINATION lib/vinbero)
