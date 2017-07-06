04 pthread Q.
=======
1. 待ち合わせ。メインがthread生成し、生成したthreadの終了を待って終了
2. mutex lockを取得して、他方がlockされていることを確認
3. mutexとcond signalを使い、同期を取る
4. threadを生成し、両方でlock、その中でループするだけ。lockを取得できた方はloopに入り、他方はlockを獲得できずにdeadlockすることを確認
5. condを使い、条件待ち終了待ち。2つのthread間、複数のthreadで待ち、他はmainからのboardcast待ち。mainはjoinですべてのthreadの終了を待つ
6. 先にsignalを出した場合のcond待ち。condは即抜けするのか？...しない！待ち続ける
7. waitのtimeout設定
