#include <gnuitar-qt/controller.h>

#include <QApplication>
#include <QDebug>

#ifdef __linux__
#include <unistd.h>
#endif /* __linux__ */

#ifdef __SSE__
#include <xmmintrin.h>
#endif

#ifdef __SSE3__
#include <pmmintrin.h>
#endif /* __SSE3__ */

int
main(int argc, char *argv[])
{

#ifdef __linux__

  /* increase the priority of the process for lower latency */

  int max_priority = sched_get_priority_max(SCHED_FIFO);

  struct sched_param p;

  p.sched_priority = max_priority / 2;

  if (sched_setscheduler (0, SCHED_FIFO, &p))
    {
      const char * dbgMsg = "warning: unable to set realtime priority (needs root privileges)";
      qDebug() << dbgMsg;
    }

    /* We might have been running to this point as setuid root program.
     * Switching to real user id. */

  if (setuid (getuid ()) != 0)
    {
      const char * dbgMsg = "error: unable to drop root privileges";
      qDebug() << dbgMsg;
      return EXIT_FAILURE;
    }

#endif /* __linux__ */

  /* Some FPU flags for faster performance. It is likely that
   * -ffast-math already generates code to turn these on, though. */

#ifdef __SSE__
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
#endif /* __SSE__ */

#ifdef __SSE3__
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
#endif /* __SSE3__ */

  QApplication a(argc, argv);

  Gnuitar::Qt::Controller controller;

  return a.exec();
}

