#pragma  once

#include "scan_config.hpp"
#include <optional>

std::optional<ScanConfig> parse_arguments(
    int argc,
    char* argv[]
);
