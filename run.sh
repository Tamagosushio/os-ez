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
# カーネルをビルド
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
  kernel.c common.c
# -machine virt -> 仮想マシンとして起動
# -bios default -> デフォルトBIOS(OpenSBI)を使用
# -nographic -> QEMUをウィンドウなしで起動
# -serial mon:stdio -> QEMUの標準入出力を仮想マシンのシリアルポートに接続
# --no-reboot -> クラッシュしても再起動させない
$QEMU -machine virt -bios default --nographic -serial mon:stdio --no-reboot \
  -kernel kernel.elf