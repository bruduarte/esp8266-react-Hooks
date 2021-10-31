#ifndef MYERROR
#define MYERROR

typedef unsigned int ErrorType;

#define RET_OK    (0)
#define RET_NOK   (1)

// errors for ConfigManager
#define ERR_OPEN_CONFIG_FILE                    (0X1000)
#define ERR_JSON_CONFIG_FILE                    (0X1001)
#define ERR_CONFIG_NOT_FOUND                    (0X1002)
#define ERR_CONFIG_EMPTY_KEY_VALUE              (0X1003)

#endif