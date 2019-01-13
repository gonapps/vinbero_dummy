add_library(vinbero_dummy MODULE vinbero_dummy.c)
target_link_libraries(vinbero_dummy vinbero_common)
set_target_properties(vinbero_dummy PROPERTIES PREFIX "")
set_target_properties(vinbero_dummy PROPERTIES PUBLIC_HEADER "vinbero_interface_DUMMY.h")
install(TARGETS vinbero_dummy
        LIBRARY DESTINATION lib/vinbero
        PUBLIC_HEADER DESTINATION include/vinbero)
