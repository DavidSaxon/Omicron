set(TESTS_SRC
    ../TestsMain.cpp

    ../omicron/api/common/attribute/Attribute_TestSuite.cpp
    ../omicron/api/common/attribute/Int32Attribute_TestSuite.cpp
)

# build the tests executable
add_executable(tests ${TESTS_SRC})

# link libraries to the tests
target_link_libraries(tests
    omicron_api
    arcanecore_test
    arcanecore_collate
    arcanecore_config
    arcanecore_json
    arcanecore_log
    arcanecore_io
    arcanecore_base
    dl
)
