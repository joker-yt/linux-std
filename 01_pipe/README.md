01 pipe
====

- int pipe(int fd[2]
	- fd[0] ... for read
	- fd[1] ... for write
	- if no error occur, return 0
-	unistd.h
-	基本的にはProcessは超えられない
	-	ただし、pipe生成した後のforkはOK。fork前に作成したディスクリプタは共有されるから
	-	超える場合はFIFOを使う
-	readはpipeが空の場合は待ち続ける
---
01 pipe Q.
====
1. forkした親子プロセス間で簡単なメッセージ通信（一方通行でOK）
2. procfsを使用して/proc/[pid]/fd/[desc]でread pipeに書き込む（実はprocファイル指定でもどのディスクリプタかが分かれば書ける）
