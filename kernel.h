#pragma once
#include "common.h"

// カーネルが続行不能なエラーに遭遇(カーネルパニック)時に実行
// ソースファイル名と行番号を表示
#define PANIC(fmt, ...)                                                   \
  do{                                                                     \
    printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    while(1){}                                                            \
  }while(0) // 行末尾の'\'で改行コードをエスケープ
// do-whileで囲うのは、if文と組み合わせたときに意図しない動作につながるのを防ぐため
// ##__VA_ARGS__は、可変長引数が空のときに直前の','を削除してくれるコンパイラ拡張機能

#define PROCS_MAX 8 // 最大プロセス数
#define PROC_UNUSED 0 // 未使用のプロセス管理構造体
#define PROC_RUNNABLE 1 // 実行可能なプロセス

struct process{
  int pid; // プロセスID
  int state; // プロセスの状態(PROC_UNSED or PROC_RUNNABLE)
  vaddr_t sp; // 実行中タスクを一時停止し、別のタスクに切り替えて実行再開する(コンテキストスイッチ)時のスタックポインタ
  uint8_t stack[8192]; // カーネルスタック
};

struct sbiret{
  long error;
  long value;
};

struct trap_frame {
  uint32_t ra;
  uint32_t gp;
  uint32_t tp;
  uint32_t t0;
  uint32_t t1;
  uint32_t t2;
  uint32_t t3;
  uint32_t t4;
  uint32_t t5;
  uint32_t t6;
  uint32_t a0;
  uint32_t a1;
  uint32_t a2;
  uint32_t a3;
  uint32_t a4;
  uint32_t a5;
  uint32_t a6;
  uint32_t a7;
  uint32_t s0;
  uint32_t s1;
  uint32_t s2;
  uint32_t s3;
  uint32_t s4;
  uint32_t s5;
  uint32_t s6;
  uint32_t s7;
  uint32_t s8;
  uint32_t s9;
  uint32_t s10;
  uint32_t s11;
  uint32_t sp;
} __attribute__((packed));

#define READ_CSR(reg)                                     \
  ({                                                      \
    unsigned long __tmp;                                  \
    __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp;                                                \
  })

#define WRITE_CSR(reg, value)                               \
  do{                                                       \
    uint32_t __tmp = (value);                               \
    __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp)); \
  }while(0)
  