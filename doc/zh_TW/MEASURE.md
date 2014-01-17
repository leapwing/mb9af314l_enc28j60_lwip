## 速度量測 ##

### 1. ICMP echo ###
在命令列視窗中執行 `ping` 指令，如果 TCPIP stack 設定正確，所傳送的ICMP 請求後，就會回應ICMP 請求。
![ping_win](https://lh5.googleusercontent.com/-BUewehekZ70/UtjVhmBp5zI/AAAAAAAAAVk/abGxFswLtP4/w669-h278-no/mb9af314l_enc28j60_ping_ip_windows.jpg)  
▲ ping in windows  
![ping_lunix](https://lh6.googleusercontent.com/-x3xb3adZ0Es/UtjVgyndfMI/AAAAAAAAAW8/SCLmvxZM7Z8/w667-h324-no/mb9af314l_enc28j60_ping_ip_linux.jpg)  
▲ ping in linux  

### 2. TCP/UDP server echo ###
另一測試網路流量的工具 [echo server][ECHOPING]，指令 `echoping` 是相似於 ping 的功能，但是是使用OSI 第四層。啟用 TCP 和 UDP port 7 來發回傳入的數據包。目的是要測試接收與發送工作之協議。  
![TCP_echoping](https://lh3.googleusercontent.com/-qp3htGvxvk0/UtjVdzNCp3I/AAAAAAAAAWs/12FJJ-9CQvo/w667-h270-no/mb9af314l_enc28j60_echoping_tcp_linux.jpg)  
▲ echoping with TCP  
![UDP_echoping](https://lh5.googleusercontent.com/-VDvo0eT2XMU/UtjVd-4TDEI/AAAAAAAAAW0/Vj6LQTdWLt0/w667-h198-no/mb9af314l_enc28j60_echoping_udp_linux.jpg)   
▲ echoping with UDP   

### 3. NetIO ###
免費的網路性能基準測試工具 [NetIO][NETIO]，在命令列執行指令。在lwipopts.h 設定有幾種不同參數下的得到結果：

![netio_1](https://lh3.googleusercontent.com/-2YqIz__uhag/UtjVgF1DPPI/AAAAAAAAAVY/3llVemTHD64/w669-h278-no/mb9af314l_enc28j60_netio_s2_536.jpg)   
▲ TCP\_MSS：536 ，TCP\_SND\_BUF： 2 * TCP\_MSS  

![netio_2](https://lh4.googleusercontent.com/-XW30H9dFJ_Y/UtjVggDOLRI/AAAAAAAAAVQ/gwvz3Lv_7Wk/w669-h278-no/mb9af314l_enc28j60_netio_s4_536.jpg)  
▲ TCP\_MSS：536 ，TCP\_SND\_BUF： 4 * TCP\_MSS  

![netio_3](https://lh6.googleusercontent.com/-F2z0nYOvVd4/UtjVfwRlH6I/AAAAAAAAAUw/8PNlX4ypGV0/w669-h278-no/mb9af314l_enc28j60_netio_s2_1500.jpg)  
▲ TCP\_MSS：1500 ，TCP\_SND\_BUF： 2 * TCP\_MSS  

![netio_4](https://lh4.googleusercontent.com/-CO9U48mpwWY/UtjVgfvRK4I/AAAAAAAAAU8/y7eu-mws3Jg/w669-h278-no/mb9af314l_enc28j60_netio_s4_1500.jpg)  
▲ TCP\_MSS：1500 ，TCP\_SND\_BUF： 4 * TCP\_MSS  

ENC28J60無COE(Checksum Offload Engine)的效果。  

### 4. Iperf ###
免費網路頻寬測試工具 [Iperf][IPERF]，`iperf` 常用於量測 TCP/UDP的吞吐量工具，可以作為Client/Server 端的測試。  
![iperf_clinet](https://lh3.googleusercontent.com/-8i-QQvy7zqU/UtjVendOY9I/AAAAAAAAAUQ/_5komwdpHbk/w669-h358-no/mb9af314l_enc28j60_ipref_client.jpg)    
▲ iperf client  
![iperf_server](https://lh3.googleusercontent.com/-z3CzmF31y-c/UtjVe6HvNfI/AAAAAAAAAU0/UHq3qWdnsSM/w669-h358-no/mb9af314l_enc28j60_ipref_server.jpg)  
▲ iperf server  
 
### 5. HTTP 網頁瀏覽器   
  
HTTP 網頁瀏覽器：   
這裡我們使用兩種網頁的效能測試方式。  
第一種方式：  
預設頁面 index.html 是將存在記憶體的固定內容傳向瀏覽器的靜態網頁，包含一些 JavaScript 程式碼， 並允許網頁包含動態內容，需定時要求運行在背景中的小數據文件和改變HTML代碼中個別值，這種技術叫做AJAX，也就是不需要重新載入整個頁面來改變數據。AJAX 允許建立複雜的 Web 應用項目，類似桌面應用程式。  
第二種方式：  
另一頁面 simple.shtml 是一個靜態網頁，不需要可動態建立的 JavaScript，即是要改變數據需要每次重新載入。  
Webserver 是 LwIP 的 contrib-package 內的一部分，依文件擴展副檔名來決定是動態(.html)還是靜態(.shtml)。  
這兩個頁面都包含bigpicture.jpg 約100多KB的大圖形 ，連結到 `bigpicture.jpg?<number>` 是指要阻止網頁瀏覽器使用快取記憶體存取，安全起見，可以完全禁用瀏覽器快取。  

**Screenshot**  
![demo_1](https://lh6.googleusercontent.com/-bVWokD-j00k/UtjVfMwL02I/AAAAAAAAAU4/dA4XB3QyDwM/w607-h659-no/mb9af314l_enc28j60_lwip_demo1.jpg)  
▲ AJAX-enable demo webpage    

![demo_2](https://lh3.googleusercontent.com/-RjI8artzSNU/UtjVfe_OFaI/AAAAAAAAAUo/D_Glndz3YuM/w723-h659-no/mb9af314l_enc28j60_lwip_demo2.jpg)  
▲ sample webpage without JavaScript  

### 6. HTTP Wget ###
在命令列視窗中執行 [wget][WGET] 指令，它會顯示有關傳輸速度及運行時間的統計數據，重點在於下載所需花費的時間，不需理會內容，所以使用選項 `--output-document=/dev/null` ，可重複幾次測試，以獲得理想的統計數據。  
![wget](https://lh5.googleusercontent.com/-tHrrxe_1Qsg/UtjVhjVxqbI/AAAAAAAAAVg/6Fi5okB9FQw/w669-h454-no/mb9af314l_enc28j60_wget_bigjpg.jpg)  
▲ speed measurement with wget    

### 7. HTTP Curl ###
在命令列視窗中執行 [curl][CURL] 指令，它類似 `wget` 但顯示統計信息是不同，多種程式測試才能得到典型的平均值。  
![curl](https://lh3.googleusercontent.com/-FZ6b7mUg-xU/UtjVd4exgmI/AAAAAAAAAUU/LB9mDT6lYSg/w669-h422-no/mb9af314l_enc28j60_curl_bigpicture.jpg)  
▲ speed measurement with curl  


[ECHOPING]:http://echoping.sourceforge.net/ "echoping"
[NETIO]: http://www.ars.de/ars/ars.nsf/docs/netio "NETIO  network throughput benchmark"
[IPERF]: http://sourceforge.net/projects/iperf/ "iperf"
[WGET]: http://sourceforge.net/projects/wget/ "wget"
[CURL]: http://sourceforge.net/projects/curl/ "curl"