/* カーネルのエントリポイント(開始地点)をboot関数に指定 */
ENTRY(boot)

SECTIONS {
  /* ベースアドレス指定 */
  /* . は現在のアドレスを表す変数 */
  . = 0x80200000;
  /* .text.bootセクションを先頭 */
  .text :{ /* コード領域 */
    KEEP(*(.text.boot));
    *(.text .text.*); /* .textと.text.から始まる名前の全セクションを配置 */
  }
  .rodata : ALIGN(4){ /* 定数データ領域(読み取り専用) */
    *(.rodata .rodata.*);
  }
  .data : ALIGN(4){ /* 読み書き可能(読み書き可) */
    *(.data .data.*);
  }
  .bss : ALIGN(4){ /* 初期値0のdata領域 */
    __bss = .;
    *(.bss .bss.* .sbss .sbss.*);
    __bss_end = .;
  }
  . = ALIGN(4); /* 4バイトでメモリアライメント */
  . += 128 * 1024; /* 128KB進める */
  __stack_top = .; /* __stack_topシンボルに現在アドレスを割り当て */
}
