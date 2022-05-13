#include <cfenv>
#include <csignal>

void exit_with_wa(int sig) {
  std::cout << "123456789!" << std::endl;
  std::exit(0);
};

// https://en.cppreference.com/w/cpp/utility/program/signal
void RE_to_WA() {
  std::signal(SIGSEGV, exit_with_wa);
  std::signal(SIGABRT, exit_with_wa);
  std::signal(SIGFPE, exit_with_wa);  // zero division
}

// Enables trapping of floating-point exceptions.
// https://en.cppreference.com/w/cpp/numeric/fenv/FE_exceptions
// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/control87-controlfp-control87-2
void WA_to_RE() {
#if defined(_GNU_SOURCE)
  feenableexcept(FE_ALL_EXCEPT);
#elif defined(_WIN32)
  _controlfp(_EM_INVALID, _MCW_EM);
#endif
}
