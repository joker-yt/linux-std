02 fifo
=========
### int mknod(const char*, mode_t, dev_t)
### int mkfifo(const char *pathname, mode_t mode)
- すでに存在しているか、存在していればそれはFIFOかどうか、チェック
- read()を呼んでおくとそこで受信待ち状態となる

---

02 fifo Q.
===
- FIFOを生成してプロセス間で通信する(プロセスはforkでOK)

---
