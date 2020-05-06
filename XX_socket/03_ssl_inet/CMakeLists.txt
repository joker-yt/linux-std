add_subdirectory(client)
add_subdirectory(server)

install(FILES etc/server.crt etc/server.key etc/server.csr
        DESTINATION bin)
