#ifndef __LOG_H__
#define __LOG_H__

#include "../protocols/protocol.h"
#include "./device.h"

typedef enum _log_level_e {
  LOG_DEBUG = 0x0,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
} log_level_e;

#pragma pack(1)
typedef struct _log_protocol_t {
  protocol_header_t header;
  char buf[100];
} log_protocol_t;
#pragma pack(0)

typedef struct _log_t {
  device_t* dev;
} log_t;

int log_init(device_t* dev);
void log_write(const char* fmt, log_level_e level, const char* file, int line,
               const char* func, ...);
void _printf(const char* fmt, ...);

#if defined(LOG_LEVEL_DEBUG)
#define debug(fmt, ...) \
  log_write(fmt, LOG_DEBUG, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define debug(fmt, ...) \
  {                     \
  }
#endif

#if defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO)
#define info(fmt, ...) \
  log_write(fmt, LOG_INFO, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define info(fmt, ...) \
  {                    \
  }
#endif

#if defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO) || \
    defined(LOG_LEVEL_WARN)
#define warn(fmt, ...) \
  log_write(fmt, LOG_WARN, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define warn(fmt, ...) \
  {                    \
  }
#endif

#if defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO) || \
    defined(LOG_LEVEL_WARN) || defined(LOG_LEVEL_ERROR)
#define error(fmt, ...) \
  log_write(fmt, LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define error(fmt, ...) \
  {                     \
  }
#endif

#endif