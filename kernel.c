typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

// リンカスクリプト内で定義されている各シンボルを宣言
// 領域の先頭アドレスを返すように配列
extern char __bss[], __bss_end[], __stack_top[];

void *memset(void *buf, char c, size_t n){
  uint8_t *p = (uint8_t *)buf;
  while(n--) *p++ = c;
  return buf;
}

void kernel_main(void){
  // .bss領域を0で初期化する
  memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);
  for(;;);
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
