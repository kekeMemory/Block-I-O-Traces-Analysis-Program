#!/bin/bash
#
# This script will only run on a computer using the IP
# address: 166.104.30.170.
#
# This auto-generated script was downloaded from http://iotta.snia.org.
# It will download the 27 subtraces of YCSB RocksDB SSD
#
echo "Downloading the 27 subtraces of YCSB RocksDB SSD" 1>&2
cookies=$(mktemp)
cat >> $cookies << 'EOF'
# Netscape HTTP Cookie File
# http://curl.haxx.se/rfc/cookie_spec.html
# This file was generated by iotta.snia.org! Edit at your own risk.

.iotta.snia.org	TRUE	/	FALSE	0	infodigest	4d1e7b8c29cf7987cf517cd9ba6e62081b3d5b91
.iotta.snia.org	TRUE	/	FALSE	0	legal	true
.iotta.snia.org	TRUE	/	FALSE	0	id	898498
.iotta.snia.org	TRUE	/	FALSE	0	user_ip	166.104.30.170
EOF

if which wget >/dev/null 2>&1; then
  useWGET=true
elif which curl >/dev/null 2>&1; then
  useCURL=true
else
  echo "Couldn't find either wget or curl. Please install one of them" 1>&2
  exit 1
fi

checkForError() {
  delete=false
  if $useWGET && (( $1 == 2 || $1 == 3 || $1 == 5 || $1 == 7 || $1 == 8 ))
  then
    delete=true
  elif $useWGET && [ ! -s "$2" ]
  then
    delete=true
  elif $useCURL && (( $1 == 22 || $1 == 36 ))
  then
    delete=true
  fi
}

downloadFile() {
  file=$1
  id=$2
  url="http://server2.iotta.snia.org/traces/block-io/$id/download?type=file&sType="
  if $useWGET; then
    wget -q --content-on-error --load-cookies=$cookies -O "$file" -c "$url""wget"
    error=$?
  elif $useCURL; then
    curl -s -f -b $cookies -o "$file" -C - -L "$url""curl"
    error=$?
    if [ $error -eq 22 ]; then
      curl -s -b $cookies -o "$file" -C - -L "$url""curl"
    fi
  fi

  if [ $error -eq 0 ]; then
    echo "Finished Downloading $file"
  else
    checkForError $error "$file"
    downloadLink="http://iotta.snia.org/downloaderinfos/new?trace_id=28568&type=subtracesViaScript&sType=&came_from=iotta.snia.org"
    if $delete; then
      echo "There was an error downloading the file ($file)"
      if grep "internal iotta error: ip mismatch" "$file" > /dev/null; then
        echo "This script will only run on a computer with the IP address 166.104.30.170."
        grep "Your current IP address is" "$file"
        echo "Please use the following link to fill out the download license form with your current IP address and run the new script:"
        echo "$downloadLink"
      elif grep "internal iotta error: invalid cookie" "$file" > /dev/null; then
        echo "Invalid cookie, please use the following link to fill out the form and try again:"
        echo "$downloadLink"
      fi
      rm -f "$file"
    else
      echo "$file was only partially downloaded."
    fi
    echo "Stopping..."
    exit 1
  fi
}

downloadFile "ssdtrace-00.gz" 28571
downloadFile "ssdtrace-01.gz" 28574
downloadFile "ssdtrace-02.gz" 28577
downloadFile "ssdtrace-03.gz" 28580
downloadFile "ssdtrace-04.gz" 28583
downloadFile "ssdtrace-05.gz" 28586
downloadFile "ssdtrace-06.gz" 28589
downloadFile "ssdtrace-07.gz" 28592
downloadFile "ssdtrace-08.gz" 28595
downloadFile "ssdtrace-09.gz" 28598
downloadFile "ssdtrace-10.gz" 28601
downloadFile "ssdtrace-11.gz" 28604
downloadFile "ssdtrace-12.gz" 28607
downloadFile "ssdtrace-13.gz" 28610
downloadFile "ssdtrace-14.gz" 28613
downloadFile "ssdtrace-15.gz" 28616
downloadFile "ssdtrace-16.gz" 28619
downloadFile "ssdtrace-17.gz" 28622
downloadFile "ssdtrace-18.gz" 28625
downloadFile "ssdtrace-19.gz" 28628
downloadFile "ssdtrace-20.gz" 28631
downloadFile "ssdtrace-21.gz" 28634
downloadFile "ssdtrace-22.gz" 28637
downloadFile "ssdtrace-23.gz" 28640
downloadFile "ssdtrace-24.gz" 28643
downloadFile "ssdtrace-25.gz" 28646
downloadFile "ssdtrace-26.gz" 28649

echo "Finished All Downloads"
rm -f $cookies