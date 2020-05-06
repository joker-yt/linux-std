# inet with openssl
- how to create materials
1. 秘密鍵の作成  
\> openssl genrsa 2024 > server.key
2. 証明書署名要求(CSR / Certificate Signing Request)の作成  
\> openssl req -new -key server.key > server.csr
3. サーバ証明書の作成  
\> openssl x509 -req -days 3650 -signkey server.key < server.csr > server.crt
## reference
- [OpenSSL](https://www.openssl.org/)
- [OpenSSL API によるセキュア・プログラミング](https://www.ibm.com/developerworks/jp/linux/library/l-openssl/)  
- [OpenSSL 入門](https://weblabo.oscasierra.net/openssl//)
- [OpenSSLコマンドによる公開鍵暗号、電子署名の方法](https://qiita.com/kunichiko/items/3c0b1a2915e9dacbd4c1)
- [オレオレ証明書をopensslで作る（詳細版）](https://ozuma.hatenablog.jp/entry/20130511/1368284304)
- [OpenSSL API によるセキュア・プログラミング](https://www.ibm.com/developerworks/jp/linux/library/l-openssl/index.html?ca=drs-)
- [HTTP/2 クライアント実装サンプル (TLS版)](https://www.nslabs.jp/http2-client-implementation-sample-tls-version.rhtml)
- [openssl s_client コマンドでウェブサーバーの SSL/TLS 対応状況を診断する](https://laboradian.com/try-openssl-s_client-command/)
- [理解してるつもりの SSL/TLS でも、もっと理解したら面白かった話](https://tkengo.github.io/blog/2015/12/01/https-details/)
- [Cipher Suite Definitions(in RFC)](https://tools.ietf.org/html/rfc5246#appendix-C)
## sample
\>openssl s_client -connect www.example.com:443  

## todo
socket -> tls-ization
fd = socket();
SSL_set_fd(,fd)

SSLv23_client_method

PEM?
 OpenSSL ディストリビューションに付属の信頼できる証明書は、ソース・コード・アーカイブに含まれる「TrustStore.pem」

pemとはただのファイル書式が決まった入れ物のことで、証明書、鍵をいくつでも含めることができるからです。
pem の書式はシンプルで、任意の証明書、鍵をbase64エンコードして以下のBEGIN/END行で挟んでつなげただけのテキストファイルです。

証明書の中身（期限、フィンガープリント）を確認したい場合、このコマンドを使います。
証明書単体のcrtファイルも同じ方法で確認できます。
openssl x509 -text -fingerprint -noout -in "ファイル名.pem"


pemファイル内の秘密鍵や、秘密鍵単体の.keyファイルの中身を表示するには openssl rsa コマンドを使います。
openssl rsa -text -noout -in "ファイル名.pem"

https://qiita.com/kunichiko/items/3c0b1a2915e9dacbd4c1

BIO?


openssl command
https://www.openssl.org/docs/man1.0.2/man1/openssl-dgst.html

## troubleshooting
- errno
  - [ssl err no](https://boringssl.googlesource.com/boringssl/+/master/include/openssl/ssl.h)