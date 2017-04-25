#include "amp.h"

#include <iostream>

namespace
{

void loop (std::istream& in, std::ostream& out)
{
  float in_signal;
  float out_signal;
  struct GnuitarAmp *amp;

  amp = gnuitar_amp_init ();
  if (amp == nullptr)
    return;

  while (!in.eof ())
    {
      in.read ((char *)(&in_signal), sizeof (in_signal));
      out_signal = gnuitar_amp_process_signal (amp, in_signal);
      out.write ((const char *)(&out_signal), sizeof (out_signal));
    }

  free (amp);
}

} /* namespace */

int
main (void)
{
  loop (std::cin, std::cout);
  return EXIT_SUCCESS;
}

