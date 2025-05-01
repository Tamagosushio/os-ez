set -xue
# QEMUはコンピュータの挙動をソフトウェアで再現するエミュレータ型仮想化ソフトウェア
QEMU=qemu-system-riscv32
# clangのパス
CC=clang
# -g3 -> デバッグ情報を最大出力
# --target=riscv32-unknown-elf -> 32bitRISC-V用にコンパイル
# -fno-stack-protector -> スタック保護機能を無効化
# -ffreestanding -> ホスト環境(開発環境)の標準ライブラリを使わせない
# -nostdlib -> 標準ライブラリをリンクさせない
# -Wl,-Tkernel.ld -> リンカスクリプトを指定
# -Wl,-Map=kernel.map -> マップファイル(リンカによる配置結果)を出力
CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf -fno-stack-protector -ffreestanding -nostdlib"

OBJCOPY=/usr/bin/llvm-objcopy
# シェルをビルド
$CC $CFLAGS -Wl,-Tuser.ld -Wl,-Map=shell.map -o shell.elf shell.c user.c common.c
$OBJCOPY --set-section-flags .bss=alloc,contents -O binary shell.elf shell.bin # ビルドした実行ファイルを生バイナリに変換
$OBJCOPY -Ibinary -Oelf32-littleriscv shell.bin shell.bin.o # 生バイナリの実行イメージをC言語に埋め込める形式に変換
# カーネルをビルド
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
  kernel.c common.c shell.bin.o
# -machine virt -> 仮想マシンとして起動
# -bios default -> デフォルトBIOS(OpenSBI)を使用
# -nographic -> QEMUをウィンドウなしで起動
# -serial mon:stdio -> QEMUの標準入出力を仮想マシンのシリアルポートに接続
# --no-reboot -> クラッシュしても再起動させない
$QEMU -machine virt -bios default --nographic -serial mon:stdio --no-reboot \
  -kernel kernel.elf