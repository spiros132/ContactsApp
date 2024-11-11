add_subdirectory(Menu)

add_subdirectory(Contacts)

target_link_libraries(${PROJECT_NAME} ContactsLib MenuLib)