#include <cstdlib>

#include <iostream>
#include <fstream>

#include <gnuitar-ampc/circuit.h>
#include <gnuitar-ampc/source-writer.h>
#include <gnuitar-ampc/spice-parser.h>
#include <gnuitar-ampc/exception.h>

namespace
{

void
log (const Gnuitar::AmpC::Exception& exception)
{
  std::cerr << "uncaught exception occured" << std::endl;
  std::cerr << "  " << exception.what () << std::endl;
}

void
log (const std::string& filename, const Gnuitar::AmpC::SyntaxError& syntax_error)
{
  std::cerr << filename;
  std::cerr << ":" << syntax_error.get_line ();
  std::cerr << ":" << syntax_error.get_column ();
  std::cerr << std::endl;
  std::cerr << "  " << syntax_error.what () << std::endl;
}

void
log (const std::string& filename, const Gnuitar::AmpC::UnexpectedToken& unexpected_token)
{
  std::cerr << filename;
  std::cerr << ":" << unexpected_token.get_line ();
  std::cerr << ":" << unexpected_token.get_column ();
  std::cerr << std::endl;

  auto expected = unexpected_token.get_expected ();
  if (expected != nullptr)
    std::cerr << "  expected: " << expected << std::endl;
}

bool
compile (const std::string& filename)
{
  std::ifstream infile (filename);
  if (!infile.good ())
    {
      std::cerr << "failed to open " << filename << std::endl;
      return false;
    }

  Gnuitar::AmpC::SpiceLexer lexer (infile);

  Gnuitar::AmpC::SpiceParser parser (lexer);

  Gnuitar::AmpC::Circuit circuit;

  try
    {
      parser.read (circuit);
    }
  catch (const Gnuitar::AmpC::UnexpectedToken& unexpected_token)
    {
      log (filename, unexpected_token);
    }
  catch (const Gnuitar::AmpC::SyntaxError& syntax_error)
    {
      log (filename, syntax_error);
    }
  catch (const Gnuitar::AmpC::Exception& exception)
    {
      log (exception);
    }

  Gnuitar::AmpC::SourceWriter source_writer;

  try
    {
      circuit.accept (source_writer);
    }
  catch (const Gnuitar::AmpC::Exception& exception)
    {
      log (exception);
    }


  std::ofstream outfile (filename + ".c");
  if (!outfile.good ())
    {
      std::cerr << "failed to open " << filename << " for writing" << std::endl;
      return false;
    }

  source_writer.write (outfile);

  return true;
}

} /* namespace */

int main (int argc, const char **argv)
{
  if (argc == 1)
    {
      std::cerr << "no input files" << std::endl;
      return EXIT_FAILURE;
    }

  for (auto i = 1; i < argc; i++)
    {
      if (!compile(argv[i]))
        return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

