#include "kernel.h"
#include "common.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

// リンカスクリプト内で定義されている各シンボルを宣言
// 領域の先頭アドレスを返すように配列
extern char __bss[], __bss_end[], __stack_top[];

// OpenSBIの呼び出し
struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long fid, long eid){
  // 指定レジスタに値を入れる
  register long a0 __asm__("a0") = arg0;
  register long a1 __asm__("a1") = arg1;
  register long a2 __asm__("a2") = arg2;
  register long a3 __asm__("a3") = arg3;
  register long a4 __asm__("a4") = arg4;
  register long a5 __asm__("a5") = arg5;
  register long a6 __asm__("a6") = fid;
  register long a7 __asm__("a7") = eid;
  // ecall命令 -> CPUの実行モードをカーネル用からOpenSBI用に切り替わり、OpenSBIの処理ハンドラが呼び出される
  __asm__ __volatile__(
    "ecall"
    : "=r"(a0), "=r"(a1)
    : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
    : "memory"
  );
  return (struct sbiret){.error = a0, .value = a1};
}

void putchar(char ch){
  // long sbi_console_putchar(int ch) 機能を使用
  sbi_call(ch, 0, 0, 0, 0, 0, 0, 1);
}

void kernel_main(void){
  // .bss領域を0で初期化する
  memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);
  // for(;;);
  // const char *s = "\n\nHello world!\n";
  // for(int i = 0; s[i] != '\0'; i++){
  //   putchar(s[i]);
  // }
  // printf("\n\nHello %s\n", "world!");
  // printf("1 + 2 = %d, %x", 1+2, 0x1234abcd);
  PANIC("booted!");
  printf("unreachable here!\n");
  for(;;) __asm__ __volatile__("wfi");
}

__attribute__((section(".text.boot"))) // .text.bootセクションに配置
__attribute__((naked)) // 関数の本文前後に余計なコードを生成させない
void boot(void){
  __asm__ __volatile__(
    "mv sp, %[stack_top]\n"
    "j kernel_main\n"
    :
    : [stack_top] "r" (__stack_top)
  ); // __stack_topにスタックポインタを設定、kernel_mainへジャンプ
}
