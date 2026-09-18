cp  profile   /etc 
mkdir   /usr/local/openssl
mkdir   /usr/local/curl
tar   -xvf   curl-arm.tar.gz        -C   /usr/local/curl/
tar   -xvf  openssl-arm.tar.gz      -C   /usr/local/openssl/
mv     /usr/local/curl/curl-arm/*      /usr/local/curl/
mv     /usr/local/openssl/openssl-arm/*    /usr/local/openssl/
echo  'export PATH=/usr/local/openssl/bin:$PATH'  >>   /etc/profile  
echo  'export PATH=/usr/local/curl/bin:$PATH'  >>   /etc/profile  
echo  'export LD_LIBRARY_PATH=/usr/local/openssl/lib:$LD_LIBRARY_PATH '  >>   /etc/profile  
echo  'export LD_LIBRARY_PATH=/usr/local/curl/lib:$LD_LIBRARY_PATH '  >>   /etc/profile                    
source /etc/profile  # 立即生效