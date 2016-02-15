add_library(hyperloglog SHARED lua_hyperloglog.c redis_hyperloglog.c)
set_target_properties(hyperloglog PROPERTIES PREFIX "" OUTPUT_NAME "hyperloglog")

# Install module
install(TARGETS hyperloglog LIBRARY DESTINATION ${TARANTOOL_LIBDIR}/${PROJECT_NAME}/)
