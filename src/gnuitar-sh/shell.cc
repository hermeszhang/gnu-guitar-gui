#include <gnuitar-sh/shell.h>

#ifndef GNUITAR_PROMPT
#define GNUITAR_PROMPT "gnuitar-sh: "
#endif

#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */

namespace Gnuitar {

Shell::Shell(void) noexcept
{
  input = stdin;
  output = stdout;
  error = stderr;

  driver_manager.update_libs ();
}

Shell::~Shell(void)
{

}

int
Shell::add_effect(void) noexcept
{
  return 0;
}

void
Shell::help(void) noexcept
{
  fprintf(output, "commands:\n");
  fprintf(output, " add-effect\n");
  fprintf(output, " help\n");
  fprintf(output, " list-effects\n");
  fprintf(output, " quit\n");
  fprintf(output, " exit (same as quit)\n");
}

void
Shell::list_effects(void) noexcept
{
  auto effect_list = driver_manager.get_effect_list ();
  for (const auto& effect_name : effect_list)
    fprintf(output, "  %s\n", effect_name.c_str ());
}

int
Shell::loop(void) noexcept
{
  for (;;)
    {

      fprintf (output, "%s", GNUITAR_PROMPT);

      std::string cmd;
      auto err = readline (cmd);
      if (err != 0)
        return err;

      if (cmd == "add-effect")
        add_effect ();
      else if (cmd == "help")
        help ();
      else if (cmd == "list-effects")
        list_effects ();
      else if (cmd == "start")
        start ();
      else if (cmd == "stop")
        stop ();
      else if ((cmd == "quit") || (cmd == "exit"))
        break;
      else
        log_error ("unknown command '" + cmd + "'");
    }

  return 0;
}

int
Shell::readline(std::string& line) noexcept
{
  int c;

  for (;;)
    {
      c = fgetc(input);
      if (c == EOF)
        return -1;
      else if (c == '\n')
        break;
      line.push_back(c);
    }

  return 0;
}

int
Shell::prompt(const std::string& field_name, std::string& field) noexcept
{
  fprintf(output, "%s: ", field_name.c_str());
  return readline(field);
}

int
Shell::start(void) noexcept
{
  driver_manager.start_driver ();
  return 0;
}

int
Shell::stop(void) noexcept
{
  driver_manager.stop_driver ();
  return 0;
}

void
Shell::log_error(const std::string& msg)
{
#ifdef _WIN32
  auto console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  if (console_handle != NULL)
    SetConsoleTextAttribute(console_handle, FOREGROUND_RED);
#endif /* _WIN32 */
  fprintf(error, "error");
#ifdef _WIN32
  if (console_handle != NULL)
    SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#endif /* _WIN32 */
  fprintf(error, ": %s\n", msg.c_str());
}

} /* namespace Gnuitar */

