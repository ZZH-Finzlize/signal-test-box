/*
@file: log.h
@author: ZZH
@time: 2022-05-09 13:37:54
@info: 日志模块
*/
#pragma once

#include <QDebug>

#define DEBUG_OUTPUT qDebug
#define DEBUG_LEVEL(pre, mod, level, fmt, ...) DEBUG_OUTPUT(pre "[" #level "]" "[" #mod "]" fmt "\r\n", ##__VA_ARGS__)

#define COMP_LOG(fmt, ...) DEBUG_LEVEL("", component, log, ": " fmt, ##__VA_ARGS__)
#define COMP_INFO(fmt, ...) DEBUG_LEVEL("", component, info, "[%s]: " fmt, __func__, ##__VA_ARGS__)
#define COMP_WARNING(fmt, ...) DEBUG_LEVEL("\r\n", component, warning, "[%s:%d]: " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define COMP_ERROR(fmt, ...) DEBUG_LEVEL("\r\n\r\n", component, error, "[%s:%d][%s]: " fmt "\r\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define UI_LOG(fmt, ...) DEBUG_LEVEL("", ui, log, ": " fmt, ##__VA_ARGS__)
#define UI_INFO(fmt, ...) DEBUG_LEVEL("", ui, info, "[%s]: " fmt, __func__, ##__VA_ARGS__)
#define UI_WARNING(fmt, ...) DEBUG_LEVEL("\r\n", ui, warning, "[%s:%d]: " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define UI_ERROR(fmt, ...) DEBUG_LEVEL("\r\n\r\n", ui, error, "[%s:%d][%s]: " fmt "\r\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

