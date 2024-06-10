

#pragma once

#include "ParseMetadata.hpp"
#include "TSVParser.hpp"
#include "helper/cli_arguments.hpp"

namespace threads {

ParseResult multiThreadedParsers(CommandLineArguments &&arguments,
                                 const ParseOptions &options,
                                 std::uint32_t nproc);

ParseResult singleThreadedParser(CommandLineArguments &&arguments,
                                 const ParseOptions &options);

} // namespace threads
