#ifndef GNUITAR_AMPC_VISITOR_H
#define GNUITAR_AMPC_VISITOR_H

#include <gnuitar-ampc/capacitor.h>
#include <gnuitar-ampc/resistor.h>
#include <gnuitar-ampc/voltage.h>

namespace Gnuitar
{

namespace AmpC
{

class Visitor
{
public:
  virtual ~Visitor (void);
  virtual void visit (const Resistor& resistor) noexcept = 0;
  virtual void visit (const Capacitor& capacitor) noexcept = 0;
  virtual void visit (const Voltage& voltage) noexcept = 0;
}; /* class Visitor */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_VISITOR_H */

