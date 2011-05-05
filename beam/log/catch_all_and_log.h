#pragma once

#include <cstdlib>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/optional.hpp>
#include <boost/utility/result_of.hpp>
#include "logger.h"

#define BEAM_CATCH_ALL_AND_LOG                      \
  do { std::string BEAM_CATCH_ALL_AND_LOG_msg; try

#define BEAM_CATCH_ALL_AND_LOG_END                                      \
  catch (...) {                                                         \
    boost::current_exception_diagnostic_information().swap(             \
        BEAM_CATCH_ALL_AND_LOG_msg);                                    \
  }                                                                     \
  if (!BEAM_CATCH_ALL_AND_LOG_msg.empty())                              \
    try {                                                               \
      LOG(FATAL) << "UNCAUGHT EXCEPTION\n" << BEAM_CATCH_ALL_AND_LOG_msg; \
    } catch (::beam::log::fatal_log) {                                  \
    } catch (...) {                                                     \
      std::exit(1);                                                     \
    }                                                                   \
  } while (0)

#if !defined(BEAM_NO_SHORT_MACRO) && !defined(BEAM_NO_SHORT_LOG_MACRO)
# define CATCH_ALL_AND_LOG BEAM_CATCH_ALL_AND_LOG
# define CATCH_ALL_AND_LOG_END BEAM_CATCH_ALL_AND_LOG_END
#endif

namespace beam {
namespace log {

template <typename F>
boost::optional<typename boost::result_of<F()>::type> catch_all_and_log(F f) {
  BEAM_CATCH_ALL_AND_LOG {
    return f();
  } BEAM_CATCH_ALL_AND_LOG_END;
  return boost::none;
}

}  // namespace log
}  // namespace beam
