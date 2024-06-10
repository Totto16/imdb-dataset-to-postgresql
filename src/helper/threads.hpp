

#pragma once

#include <string>

#include "ParseMetadata.hpp"
#include "TSVParser.hpp"
#include "helper/cli_arguments.hpp"

namespace threads {

ParseResult multiThreadedParsers(CommandLineArguments &&arguments,
                                 const ParseOptions &options, int nproc);

ParseResult singleThreadedParser(CommandLineArguments &&arguments,
                                 const ParseOptions &options);

} // namespace threads
