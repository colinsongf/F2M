/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.
*/

/* 
Copyright (c) 2016 by contributors.
Author: Chao Ma (mctt90@gmail.com)

This file is the implementation of common.h.
*/

#include "src/common/common.h"

#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>


#include <assert.h>
#if __unix__
#include <sys/time.h>
#endif

#include <stdexcept>
#include <string>

/* -----------------------------------------------------------------------------
 * The basic meachism of Logger is as follows:                                  *
 *                                                                              *
 *  - LOG(severity) defines a Logger instance, which records the severity.      *
 *                                                                              *
 *  - LOG(severity) then invokes Logger::Start(), which invokes Logger::Stream  *
 *    to choose an output stream, outputs a message head into the stream and    *
 *    flush.                                                                    *
 *                                                                              *
 *  - The std::ostream reference is returned by LoggerStart(), passed to        *
 *    user-speific output operators (<<), which writes the log message boby.    *
 *                                                                              *
 *  - When the Logger instance is destructed, the destructor appends flush.     * 
 *    If severity is FATAL, the destructor causes SEGFAULT and core dump.       *
 *                                                                              *
 * It is important to flush in Logger::Start() after outputing message          *
 * head because the time when the destructor is invoked  depends on how/where   *
 * the caller code defines the Logger instance.                                 *
 *                                                                              *
 * If the caller code crashes before the Logger instance is properly            *
 * destructed, the destructor might not have the chance to append its flush     *
 * flags.                                                                       *
 *                                                                              *
 * Without flush in Logger::Start(), this may cause the lose of the last        *
 * few messages.                                                                *
 *                                                                              *
 * However, given flush in Start(), program crashing between invocations to     *
 * Logger::Start() and destructor only causes the lose of the last message      *
 * body, while the message head will be there.                                  *
 * -----------------------------------------------------------------------------
 */

std::ofstream Logger::info_log_file_;
std::ofstream Logger::warn_log_file_;
std::ofstream Logger::erro_log_file_;

void InitializeLogger(const std::string& info_log_filename,
                      const std::string& warn_log_filename,
                      const std::string& erro_log_filename) {
  Logger::info_log_file_.open(info_log_filename.c_str());
  Logger::warn_log_file_.open(warn_log_filename.c_str());
  Logger::erro_log_file_.open(erro_log_filename.c_str());
}

/*static*/
std::ostream& Logger::GetStream(LogSeverity severity) {
  return (severity == INFO) ?
      (info_log_file_.is_open() ? info_log_file_ : std::cout) :
      (severity == WARNING ?
       (warn_log_file_.is_open() ? warn_log_file_ : std::cerr) :
       (erro_log_file_.is_open() ? erro_log_file_ : std::cerr));
}

/*static*/
std::ostream& Logger::Start(LogSeverity severity,
                            const std::string& file,
                            int line,
                            const std::string& function) {
  time_t tm;
  time(&tm);
  char time_string[128];
  ctime_r(&tm, time_string);
  return GetStream(severity) << time_string
                             << " " << file << ":" << line
                             << " (" << function << ") " << std::flush;
}

Logger::~Logger() {
  GetStream(severity_) << "\n" << std::flush;

  if (severity_ == FATAL) {
    info_log_file_.close();
    warn_log_file_.close();
    erro_log_file_.close();
    abort();
  }
}

/* -----------------------------------------------------------------------------
 * Implementation of the Hash functions                                         *
 * -----------------------------------------------------------------------------
 */

unsigned int RSHash(const std::string& str) {
  unsigned int b    = 378551;
  unsigned int a    = 63689;
  unsigned int hash = 0;

  for (std::size_t i = 0; i < str.length(); i++) {
    hash = hash * a + str[i];
    a = a * b;
  }
  return hash;
}
/* End Of RS Hash Function */

unsigned int JSHash(const std::string& str) {
  unsigned int hash = 1315423911;

  for (std::size_t i = 0; i < str.length(); i++) {
    hash ^= ((hash << 5) + str[i] + (hash >> 2));
  }
  return hash;
}
/* End Of JS Hash Function */

unsigned int PJWHash(const std::string& str) {
  unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
  unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
  unsigned int OneEighth     = (unsigned int)(BitsInUnsignedInt / 8);
  unsigned int HighBits =
      (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
  unsigned int hash              = 0;
  unsigned int test              = 0;

  for (std::size_t i = 0; i < str.length(); i++) {
    hash = (hash << OneEighth) + str[i];

    if ((test = hash & HighBits) != 0) {
      hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
  }

  return hash;
}
/* End Of  P. J. Weinberger Hash Function */

unsigned int ELFHash(const std::string& str) {
  unsigned int hash = 0;
  unsigned int x    = 0;

  for (std::size_t i = 0; i < str.length(); i++) {
    hash = (hash << 4) + str[i];
    if ((x = hash & 0xF0000000L) != 0) {
      hash ^= (x >> 24);
    }
    hash &= ~x;
  }

  return hash;
}
/* End Of ELF Hash Function */

unsigned int BKDRHash(const std::string& str) {
  unsigned int seed = 131;  // 31 131 1313 13131 131313 etc..
  unsigned int hash = 0;

  for (std::size_t i = 0; i < str.length(); i++) {
    hash = (hash * seed) + str[i];
  }

  return hash;
}
/* End Of BKDR Hash Function */

unsigned int SDBMHash(const std::string& str) {
  unsigned int hash = 0;

  for (std::size_t i = 0; i < str.length(); i++) {
    hash = str[i] + (hash << 6) + (hash << 16) - hash;
  }

  return hash;
}
/* End Of SDBM Hash Function */

unsigned int DJBHash(const std::string& str) {
  unsigned int hash = 5381;

  for (std::size_t i = 0; i < str.length(); i++) {
    hash = ((hash << 5) + hash) + str[i];
  }

  return hash;
}
/* End Of DJB Hash Function */

unsigned int DEKHash(const std::string& str) {
  unsigned int hash = static_cast<unsigned int>(str.length());

  for (std::size_t i = 0; i < str.length(); i++) {
    hash = ((hash << 5) ^ (hash >> 27)) ^ str[i];
  }

  return hash;
}
/* End Of DEK Hash Function */

unsigned int BPHash(const std::string& str) {
  unsigned int hash = 0;
  for (std::size_t i = 0; i < str.length(); i++) {
    hash = hash << 7 ^ str[i];
  }

  return hash;
}
/* End Of BP Hash Function */

unsigned int FNVHash(const std::string& str) {
  const unsigned int fnv_prime = 0x811C9DC5;
  unsigned int hash = 0;
  for (std::size_t i = 0; i < str.length(); i++) {
    hash *= fnv_prime;
    hash ^= str[i];
  }

  return hash;
}
/* End Of FNV Hash Function */

unsigned int APHash(const std::string& str) {
  unsigned int hash = 0xAAAAAAAA;

  for (std::size_t i = 0; i < str.length(); i++) {
    hash ^= ((i & 1) == 0) ? ((hash <<  7) ^ str[i] * (hash >> 3)) :
            (~((hash << 11) + (str[i] ^ (hash >> 5))));
  }

  return hash;
}
/* End Of AP Hash Function */

/* -----------------------------------------------------------------------------
 * Implementation of the SplitString                                            *
 * -----------------------------------------------------------------------------
 */

/* In most cases, delim contains only one character.  In this case, we
 * use CalculateReserveForVector to count the number of elements
 * should be reserved in result vector, and thus optimize SplitStringUsing. */

static int CalculateReserveForVector(const string& full, const char* delim) {
  int count = 0;
  if (delim[0] != '\0' && delim[1] == '\0') {
    // Optimize the common case where delim is a single character.
    char c = delim[0];
    const char* p = full.data();
    const char* end = p + full.size();
    while (p != end) {
      if (*p == c) {  // This could be optimized with hasless(v,1) trick.
        ++p;
      } else {
        while (++p != end && *p != c) {
          // Skip to the next occurence of the delimiter.
        }
        ++count;
      }
    }
  }
  return count;
}

void SplitStringUsing(const string& full,
                      const char* delim,
                      vector<string>* result) {
  CHECK(delim != NULL);
  CHECK(result != NULL);
  result->reserve(CalculateReserveForVector(full, delim));
  back_insert_iterator< vector<string> > it(*result);
  SplitStringToIteratorUsing(full, delim, it);
}

void SplitStringToSetUsing(const string& full,
                           const char* delim,
                           set<string>* result) {
  CHECK(delim != NULL);
  CHECK(result != NULL);
  simple_insert_iterator<set<string> > it(result);
  SplitStringToIteratorUsing(full, delim, it);
}

/* -----------------------------------------------------------------------------
 * Implementation of the StringPrintf                                           *
 * This code comes from the re2 project host on Google Code                     *
 * (http://code.google.com/p/re2/), in particular, the following source file    *
 * http://code.google.com/p/re2/source/browse/util/stringprintf.cc              *
 * -----------------------------------------------------------------------------
 */

static void StringAppendV(string* dst, const char* format, va_list ap) {
  // First try with a small fixed size buffer
  char space[1024];

  // It's possible for methods that use a va_list to invalidate
  // the data in it upon use.  The fix is to make a copy
  // of the structure before using it and use that copy instead.
  va_list backup_ap;
  va_copy(backup_ap, ap);
  int result = vsnprintf(space, sizeof(space), format, backup_ap);
  va_end(backup_ap);

  if ((result >= 0) && (result < sizeof(space))) {
    // It fit
    dst->append(space, result);
    return;
  }

  // Repeatedly increase buffer size until it fits
  int length = sizeof(space);
  while (true) {
    if (result < 0) {
      // Older behavior: just try doubling the buffer size
      length *= 2;
    } else {
      // We need exactly "result+1" characters
      length = result + 1;
    }
    char* buf = new char[length];

    // Restore the va_list before we use it again
    va_copy(backup_ap, ap);
    result = vsnprintf(buf, length, format, backup_ap);
    va_end(backup_ap);

    if ((result >= 0) && (result < length)) {
      // It fit
      dst->append(buf, result);
      delete[] buf;
      return;
    }
    delete[] buf;
  }
}

string StringPrintf(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  string result;
  StringAppendV(&result, format, ap);
  va_end(ap);
  return result;
}

void SStringPrintf(string* dst, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  dst->clear();
  StringAppendV(dst, format, ap);
  va_end(ap);
}

void StringAppendF(string* dst, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  StringAppendV(dst, format, ap);
  va_end(ap);
}

/* -----------------------------------------------------------------------------
 * Implementation of the ConditionVariable                                      *
 * This is a copy from :                                                        *
 *   http://tc-svn.tencent.com/setech/setech_infrastructure_rep/Infra_proj/     *
 *   trunk/src/common/system/concurrency/condition_variable.cpp                 *
 * -----------------------------------------------------------------------------
 */

#ifdef _WIN32

ConditionVariable::ConditionVariable() {
  m_hCondition = ::CreateEvent(NULL, FALSE, FALSE, NULL);
  m_nWaitCount = 0;
  assert(m_hCondition != NULL);
}

ConditionVariable::~ConditionVariable() {
  ::CloseHandle(m_hCondition);
}

void ConditionVariable::Wait(Mutex* inMutex) {
  inMutex->Unlock();
  m_nWaitCount++;
  DWORD theErr = ::WaitForSingleObject(m_hCondition, INFINITE);
  m_nWaitCount--;
  assert((theErr == WAIT_OBJECT_0) || (theErr == WAIT_TIMEOUT));
  inMutex->Lock();

  if (theErr != WAIT_OBJECT_0)
    throw std::runtime_error("ConditionVariable::Wait");
}

bool ConditionVariable::Wait(Mutex* inMutex, int inTimeoutInMilSecs) {
  inMutex->Unlock();
  m_nWaitCount++;
  DWORD theErr = ::WaitForSingleObject(m_hCondition, inTimeoutInMilSecs);
  m_nWaitCount--;
  assert((theErr == WAIT_OBJECT_0) || (theErr == WAIT_TIMEOUT));
  inMutex->Lock();

  if (theErr == WAIT_OBJECT_0)
    return true;
  else if (theErr == WAIT_TIMEOUT)
    return false;
  else
    throw std::runtime_error("ConditionVariable::Wait");
}

void ConditionVariable::Signal() {
  if (!::SetEvent(m_hCondition))
    throw std::runtime_error("ConditionVariable::Signal");
}

void ConditionVariable::Broadcast() {
  // There doesn't seem like any more elegant way to
  // implement Broadcast using events in Win32.
  // This will work, it may generate spurious wakeups,
  // but condition variables are allowed to generate
  // spurious wakeups
  unsigned int waitCount = m_nWaitCount;
  for (unsigned int x = 0; x < waitCount; x++) {
    if (!::SetEvent(m_hCondition))
      throw std::runtime_error("ConditionVariable::Broadcast");
  }
}

#elif defined __unix__ || defined __APPLE__

void ConditionVariable::CheckError(const char* context, int error) {
  if (error != 0) {
    std::string msg = context;
    msg += " error: ";
    msg += strerror(error);
    throw std::runtime_error(msg);
  }
}

ConditionVariable::ConditionVariable() {
  pthread_condattr_t cond_attr;
  pthread_condattr_init(&cond_attr);
  int ret = pthread_cond_init(&m_hCondition, &cond_attr);
  pthread_condattr_destroy(&cond_attr);
  CheckError("ConditionVariable::ConditionVariable", ret);
}

ConditionVariable::~ConditionVariable() {
  pthread_cond_destroy(&m_hCondition);
}

void ConditionVariable::Signal() {
  CheckError("ConditionVariable::Signal",
             pthread_cond_signal(&m_hCondition));
}

void ConditionVariable::Broadcast() {
  CheckError("ConditionVariable::Broadcast",
             pthread_cond_broadcast(&m_hCondition));
}

void ConditionVariable::Wait(Mutex* inMutex) {
  CheckError("ConditionVariable::Wait",
             pthread_cond_wait(&m_hCondition, &inMutex->m_Mutex));
}

bool ConditionVariable::Wait(Mutex* inMutex, int inTimeoutInMilSecs) {
  if (inTimeoutInMilSecs < 0) {
    Wait(inMutex);  // wait forever
    return true;
  }

  // get current absolate time
  struct timeval tv;
  gettimeofday(&tv, NULL);

  // add timeout
  tv.tv_sec += inTimeoutInMilSecs / 1000;
  tv.tv_usec += (inTimeoutInMilSecs % 1000) * 1000;

  int million = 1000000;
  if (tv.tv_usec >= million) {
    tv.tv_sec += tv.tv_usec / million;
    tv.tv_usec %= million;
  }

  // convert timeval to timespec
  struct timespec ts;
  ts.tv_sec = tv.tv_sec;
  ts.tv_nsec = tv.tv_usec * 1000;
  int error = pthread_cond_timedwait(&m_hCondition, &inMutex->m_Mutex, &ts);

  if (error == ETIMEDOUT)
    return false;
  else
    CheckError("ConditionVariable::Wait", error);
  return true;
}

#endif