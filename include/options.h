/*
 * Helper module to parse and store command line arguments.
 */

#ifndef OPTIONS_H
#define OPTIONS_H

struct options 
{
  bool debug, step_run;
  std::string source_file;
};

options parse_opts(int argc, char *argv[])
{
  options opts;
  if (argc < 2 || argc > 4)
    throw std::string("Invalid number of arguments!");

  for (int i = 2; i < argc; ++i)
  {
    auto arg = std::string(argv[i]);

    if (arg == "--step-run")
      opts.step_run = true;
    else if (arg == "--debug")
      opts.debug = true;
    else
      throw std::string("Invalid argument " + arg + "!");
  }

  opts.source_file = argv[1];

  return opts;
}

#endif
