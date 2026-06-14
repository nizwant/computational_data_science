# Compile and start a local test server before tests run.
# The server listens on 127.0.0.1:2137.

server_bin <- file.path(tempdir(), "test_server")
server_pid <- NULL

# Compile the server from package sources
src_dir <- system.file(".", package = "PeerChat")
# When running via R CMD check, sources are in the package root
pkg_root <- Sys.getenv("TESTTHAT_PKG_ROOT", getwd())
src_files <- file.path(pkg_root, "src")

if (!dir.exists(src_files)) {
  # Fallback: find source relative to test dir
  src_files <- normalizePath(file.path(dirname(dirname(getwd())), "src"))
}

if (dir.exists(src_files)) {
  compile_cmd <- sprintf(
    "cc -o %s %s/server.c %s/c_common.c -I%s -USERVER_IP -DSERVER_IP='\"127.0.0.1\"'",
    server_bin, src_files, src_files, src_files
  )
  system(compile_cmd)

  if (file.exists(server_bin)) {
    # Start server in background
    server_pid <- sys::exec_background(server_bin)
    Sys.sleep(0.5)  # give server time to bind

    # Point client at localhost
    Sys.setenv(PEERCHAT_SERVER_IP = "127.0.0.1")
    message("Test server started (PID: ", server_pid, ")")
  } else {
    message("WARNING: Could not compile test server, skipping integration tests")
  }
} else {
  message("WARNING: Source directory not found, skipping integration tests")
}

server_is_running <- function() {
  !is.null(server_pid) && file.exists(server_bin)
}

# Teardown: kill the server after all tests
withr::defer(
  {
    if (!is.null(server_pid)) {
      tools::pskill(server_pid, signal = tools::SIGTERM)
      message("Test server stopped (PID: ", server_pid, ")")
    }
    Sys.unsetenv("PEERCHAT_SERVER_IP")
  },
  teardown_env()
)
