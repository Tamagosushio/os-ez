#pragma once

// カーネルが続行不能なエラーに遭遇(カーネルパニック)時に実行
// ソースファイル名と行番号を表示
#define PANIC(fmt, ...)                                                   \
  do{                                                                     \
    printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    while(1){}                                                            \
  }while(0) // 行末尾の'\'で改行コードをエスケープ
// do-whileで囲うのは、if文と組み合わせたときに意図しない動作につながるのを防ぐため
// ##__VA_ARGS__は、可変長引数が空のときに直前の','を削除してくれるコンパイラ拡張機能

struct sbiret{
  long error;
  long value;
};