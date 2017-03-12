#ifndef GNUITAR_AMPC_SPICE_TOKEN_H
#define GNUITAR_AMPC_SPICE_TOKEN_H

#include <string>

namespace Gnuitar
{

namespace AmpC
{

class SpiceLexer;

class SpiceToken final
{
  std::string data;
public:
  enum class Type
  {
    integer,
    real_number,
    identifier,
    symbol
  }; /* enum class Type */
private:
  Type type;
public:
  SpiceToken (void) noexcept;
  SpiceToken (const SpiceToken& spice_token) noexcept;
  SpiceToken (SpiceToken&& spice_token) noexcept;
  ~SpiceToken (void);
  Type get_type (void) const noexcept;
  const std::string& get_data (void) const noexcept;
  bool get_integer (long *value) const noexcept;
  bool get_real_number (float *value) const noexcept;
  SpiceToken& operator = (const SpiceToken& spice_token) noexcept;
protected:
  friend SpiceLexer;
  /** adds a character to the token */
  void add_char (char c) noexcept;
  /** clears the characters allocated by the token */
  void clear (void) noexcept;
  void set_type (Type type) noexcept;
  void set_data (const std::string& data) noexcept;
  void set_data (std::string&& data) noexcept;
}; /* class SpiceToken */

} /* namespace AmpC */

} /* namespace Gnuitar */

#endif /* GNUITAR_AMPC_SPICE_TOKEN_H */

