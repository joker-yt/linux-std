---
05.IPC(msgsnd/msgrcv)
===
- ツール
  - ipcs, ipcrm, ipcmk
- bufferの基本は

```
struct st_msgbuf {
  long mtype;     /* message type, must be > 0 */
  char mtext[32]; /* message data */
};

int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,
               int msgflg);
```
- msgszはmtext部のサイズ。st_msgbufではないので注意
- msgtypはmtypeを示し、msgrcvのparamで取得するmsg種別を指定できる
  - msgflgにMSG_EXCEPTが指定されている場合、mtypeが等しくないものをreadする
  （いづれもqueueの先頭）


---

05.IPC(msgsnd/msgrcv) Q.
===
1.通常にメッセージを送信、受信する
2.queueがすでに存在している時(現象が見れていない)
3.送信側が先にsndするパターン、受信側が先にrcvに入るパターン
4.msgtypの使い方。queueから特定のメッセージのみ読み出す
4.msgctl
5.(POSIX)mq_open...

http://www.turbolinux.co.jp/solution_service/library/features/c_magazine/vol_05.html
- ---
