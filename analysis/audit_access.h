// Force-include for the diagnostic build only; keep MSVC access-name mangling consistent.
#include <array>
#include <cstdint>
#include <filesystem>
#include <iosfwd>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <windows.h>
#define private public
#include "application_state.h"
#undef private
